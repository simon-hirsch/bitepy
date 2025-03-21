import pandas as pd
import numpy as np
from zipfile import ZipFile
import os
from tqdm import tqdm

try:
    from ._bite import Simulation_cpp
except ImportError as e:
    raise ImportError(
        "Failed to import _bite module. Ensure that the C++ extension is correctly built and installed."
    ) from e

class Data:
    def __init__(self):
        pass

    def _read_id_table_2020(self, timestamp, datapath):
        year = timestamp.strftime("%Y")
        month = timestamp.strftime("%m")
        datestr = "Continuous_Orders_DE_"+timestamp.strftime("%Y%m%d")
        
        # get file name of zip-file and csv file within zip file
        file_list = os.listdir(f"{datapath}/{year}/{month}")
        zip_file_name = [i for i in file_list if datestr in i][0]
        csv_file_name = zip_file_name[:-4]

        # read data
        zip_file = ZipFile(f"{datapath}/{year}/{month}/"+zip_file_name)
        df = (pd.read_csv(zip_file.open(csv_file_name), sep=";", decimal=".")
            .drop_duplicates(subset = ["Order ID", "Initial ID", "Action code", "Validity time", "Price", "Quantity"]) # drop in particular the duplicate orders with action code 'C'
            .loc[lambda x: x["Is User Defined Block"]==0] # no blocks
            .loc[lambda x: (x["Product"]=="Intraday_Hour_Power") | (x["Product"]=="XBID_Hour_Power")] # EPEX and XBID
            .loc[lambda x: (x["Action code"]=="A") | (x["Action code"]=="D") | (x["Action code"]=="C") | (x["Action code"]=="I")]
            .drop(["Delivery area", "Execution restriction", "Market area", "Parent ID", "Delivery End", "Currency", "Product", "isOTC", "Is User Defined Block", "Unnamed: 20", "RevisionNo", "Entry time"], axis=1)
            .rename({"Order ID":"order",
                "Initial ID": "initial",
                "Delivery Start": "start",
                "Side" : "side",
                "Price" : "price",
                "Volume" : "volume",
                "Validity time" : "validity",
                "Action code" : "action",
                "Transaction Time" : "transaction",
                "Quantity" : "quantity"
                }, axis=1)
            .assign(start=lambda x: pd.to_datetime(x.start, format="%Y-%m-%dT%H:%M:%SZ")) # convert validity time to datetime
            .assign(validity=lambda x: pd.to_datetime(x.validity, format="%Y-%m-%dT%H:%M:%SZ")) # convert validity time to datetime
            .assign(transaction=lambda x: pd.to_datetime(x.transaction, format="%Y-%m-%dT%H:%M:%S.%fZ")) # convert transaction time to datetime
            )

        # Delete all orders that are visibly iceberg orders (i.e., have one message with I somewhere)
        # ToDo: That is probably not a good idea
        iceberg_IDs = df.loc[df["action"] == "I", "initial"].unique() # get all iceberg IDs
        df = df.loc[~df["initial"].isin(iceberg_IDs)] # delete all orders with iceberg IDs

        # Use all order with action code 'C' (change of quantity) and make new orders out of it
        # ToDo: Is there a more elegant way of doing this? This works reasonably well if there are no orders with many quantity changes.
        # Attention: This assumes that the messages are ordered correctly (which they seemed to be)
        change_messages = df[df["action"] == "C"].drop_duplicates(subset = ["order"], keep = "first") # get all change messages and drop duplicates (keep the first one)  there might be more than one change message for one orderID (ie. if multiple changes for an add message exist)
        not_added = change_messages[~(change_messages["order"].isin(df.loc[df["action"] == "A", "order"]))] # get all change messages rows of df, that are not added (i.e., there is no order with action code 'A' with the same order ID); no "not_added" entries should exist at the beginning
        change_messages = change_messages[~(change_messages["order"].isin(not_added["order"]))] # delete those change messages that are not added, ie. not "A"

        # display all rows of df with duplicate "order" value of at least 3 equal order values
        # display first 1000 rows
        # display(df[df.duplicated(subset = ["order"], keep = False)].sort_values("order").head(1000))
        
        change_exists = change_messages.shape[0] > 0
        change_counter = 0
        while change_exists:
            # get last line (by transaction time) with action code 'A' that belongs to the change (may be the single last change line) and then change the validity date correspondingly
            indexer_messA_with_change = df[(df["order"].isin(change_messages["order"])) & (df["action"] == "A")].sort_values("transaction").groupby("order").tail(1).index # get indices of all "A" messages of df and whose order number is in change_messages; only take the last entry of each group (there should be only one entry)

            df["df_index_copy"] = df.index # copy the index to preserve after merge below
            merged = pd.merge(change_messages, df.loc[indexer_messA_with_change], on = 'order') 
            df.loc[merged["df_index_copy"].to_numpy(), "validity"] = merged["transaction_x"].to_numpy() # change the validity date of the add messages in df to the transaction date of the change messages


            # change the action code of the change message to "A", ie. add so that it can be processed in the next iteration; ie. change the "C" message to an "A" message
            df.loc[df.index.isin(change_messages.index), "action"] = "A"
            df.drop("df_index_copy", axis=1, inplace=True) # remove this column for the next iteration

            
            # redo precedure
            change_messages = df[df["action"] == "C"].drop_duplicates(subset = ["order"], keep = "first")
            not_added = change_messages[~(change_messages["order"].isin(df.loc[df["action"] == "A", "order"]))]
            change_messages = change_messages[~(change_messages["order"].isin(not_added["order"]))]
            change_exists = change_messages.shape[0] > 0
            change_counter += 1
        #print("Size of largest change stack: ", change_counter, flush=True)

        # take all orders with action code 'D' (delete) and use the transaction date of those as the new validity time of the orders with the same order ID
        cancel_messages = df[df["action"] == "D"]
        
        # delete those cancel_messages that belong to orders that are not previously added (ToDo: Why do such messages exist?)
        not_added = cancel_messages[~(cancel_messages["order"].isin(df.loc[df["action"] == "A", "order"]))]
        cancel_messages = cancel_messages[~(cancel_messages["order"].isin(not_added["order"]))]
        
        indexer_messA_with_cancel = df[(df["order"].isin(cancel_messages["order"])) & (df["action"] == "A")].sort_values("transaction").groupby("order").tail(1).index
        df["df_index_copy"] = df.index
        merged = pd.merge(cancel_messages, df.loc[indexer_messA_with_cancel], on = 'order')
        df.loc[merged["df_index_copy"].to_numpy(), "validity"] = merged["transaction_x"].to_numpy()

        df = df.loc[lambda x: ~(x["action"] == "D")] # delete all cancel messages (they are not needed anymore)
        
        # drop columns "Order ID" and "Action" and "df_index_copy"
        df = df.drop(["order", "action", "df_index_copy"], axis=1) # drop the action and order columns as they are not needed anymore (only add messages are left)

        # reorder columns
        newOrder = ["initial", "side", "start", "transaction", "validity", "price", "quantity"]
        df = df[newOrder]
        # Convert all strings in the "side" column to uppercase
        df['side'] = df['side'].str.upper()

        df["start"] = df["start"].dt.tz_localize('UTC').dt.strftime('%Y-%m-%dT%H:%M:%SZ')
        df["transaction"] = df["transaction"].dt.tz_localize('UTC').dt.strftime('%Y-%m-%dT%H:%M:%S.%f').str[:-3]+'Z'
        df["validity"] = df["validity"].dt.tz_localize('UTC').dt.strftime('%Y-%m-%dT%H:%M:%S.%f').str[:-3]+'Z'
        
        return df

    def _read_id_table_2021(self, timestamp, datapath):
        year = timestamp.strftime("%Y")
        month = timestamp.strftime("%m")
        datestr = "Continuous_Orders-DE-"+timestamp.strftime("%Y%m%d")
        
        # get file name of zip-file and csv file within zip file
        file_list = os.listdir(f"{datapath}/{year}/{month}")
        zip_file_name = [i for i in file_list if datestr in i][0]
        csv_file_name = zip_file_name[:-4]

        # read data
        zip_file = ZipFile(f"{datapath}/{year}/{month}/"+zip_file_name)
        df = (pd.read_csv(zip_file.open(csv_file_name), sep=",", decimal=".", skiprows = 1)
            .drop_duplicates(subset = ["OrderId", "InitialId", "ActionCode", "ValidityTime", "Price", "Quantity"]) # drop in particular the duplicate orders with action code 'C'
            .loc[lambda x: x["UserDefinedBlock"]=="N"] # no blocks
            .loc[lambda x: (x["Product"]=="Intraday_Hour_Power") | (x["Product"]=="XBID_Hour_Power")] # EPEX and XBID Hourly Products
            .loc[lambda x: (x["ActionCode"]=="A") | (x["ActionCode"]=="D") | (x["ActionCode"]=="C") | (x["ActionCode"]=="I")] # only add, delete, change, and iceberg orders
            .drop(["LinkedBasketId", "DeliveryArea", "ParentId", "DeliveryEnd", "Currency", "Product", "UserDefinedBlock", "RevisionNo", "ExecutionRestriction", "CreationTime", "QuantityUnit", "Volume", "VolumeUnit"], axis=1) # drop columns that are not needed
            .rename({"OrderId":"order",
                "InitialId": "initial",
                "DeliveryStart": "start",
                "Side" : "side",
                "Price" : "price",
                "Volume" : "volume",
                "ValidityTime" : "validity",
                "ActionCode" : "action",
                "TransactionTime" : "transaction",
                "Quantity" : "quantity"
                }, axis=1)
                    .assign(start=lambda x: pd.to_datetime(x.start, format="%Y-%m-%dT%H:%M:%SZ")) # convert validity time to datetime
                    .assign(validity=lambda x: pd.to_datetime(x.validity, format="%Y-%m-%dT%H:%M:%SZ")) # convert validity time to datetime
                    .assign(transaction=lambda x: pd.to_datetime(x.transaction, format="%Y-%m-%dT%H:%M:%S.%fZ")) # convert transaction time to datetime
            )
        # Delete all orders that are visibly iceberg orders (i.e., have one message with I somewhere)
        # ToDo: That is probably not a good idea
        iceberg_IDs = df.loc[df["action"] == "I", "initial"].unique() # get all iceberg IDs
        df = df.loc[~df["initial"].isin(iceberg_IDs)] # delete all orders with iceberg IDs

        # Use all order with action code 'C' (change of quantity) and make new orders out of it
        # ToDo: Is there a more elegant way of doing this? This works reasonably well if there are no orders with many quantity changes.
        # Attention: This assumes that the messages are ordered correctly (which they seemed to be)
        change_messages = df[df["action"] == "C"].drop_duplicates(subset = ["order"], keep = "first") # get all change messages and drop duplicates (keep the first one)  there might be more than one change message for one orderID (ie. if multiple changes for an add message exist)
        not_added = change_messages[~(change_messages["order"].isin(df.loc[df["action"] == "A", "order"]))] # get all change messages rows of df, that are not added (i.e., there is no order with action code 'A' with the same order ID); no "not_added" entries should exist at the beginning
        change_messages = change_messages[~(change_messages["order"].isin(not_added["order"]))] # delete those change messages that are not added, ie. not "A"

        # display all rows of df with duplicate "order" value of at least 3 equal order values
        # display first 1000 rows
        # display(df[df.duplicated(subset = ["order"], keep = False)].sort_values("order").head(1000))

        
        
        change_exists = change_messages.shape[0] > 0
        change_counter = 0
        while change_exists:
            # get last line (by transaction time) with action code 'A' that belongs to the change (may be the single last change line) and then change the validity date correspondingly
            indexer_messA_with_change = df[(df["order"].isin(change_messages["order"])) & (df["action"] == "A")].sort_values("transaction").groupby("order").tail(1).index # get indices of all "A" messages of df and whose order number is in change_messages; only take the last entry of each group (there should be only one entry)

            df["df_index_copy"] = df.index # copy the index to preserve after merge below
            merged = pd.merge(change_messages, df.loc[indexer_messA_with_change], on = 'order') 
            df.loc[merged["df_index_copy"].to_numpy(), "validity"] = merged["transaction_x"].to_numpy() # change the validity date of the add messages in df to the transaction date of the change messages


            # change the action code of the change message to "A", ie. add so that it can be processed in the next iteration; ie. change the "C" message to an "A" message
            df.loc[df.index.isin(change_messages.index), "action"] = "A"
            df.drop("df_index_copy", axis=1, inplace=True) # remove this column for the next iteration

            
            # redo precedure
            change_messages = df[df["action"] == "C"].drop_duplicates(subset = ["order"], keep = "first")
            not_added = change_messages[~(change_messages["order"].isin(df.loc[df["action"] == "A", "order"]))]
            change_messages = change_messages[~(change_messages["order"].isin(not_added["order"]))]
            change_exists = change_messages.shape[0] > 0
            change_counter += 1
        #print("Size of largest change stack: ", change_counter, flush=True)

        # take all orders with action code 'D' (delete) and use the transaction date of those as the new validity time of the orders with the same order ID
        cancel_messages = df[df["action"] == "D"]
        
        # delete those cancel_messages that belong to orders that are not previously added (ToDo: Why do such messages exist?)
        not_added = cancel_messages[~(cancel_messages["order"].isin(df.loc[df["action"] == "A", "order"]))]
        cancel_messages = cancel_messages[~(cancel_messages["order"].isin(not_added["order"]))]
        
        indexer_messA_with_cancel = df[(df["order"].isin(cancel_messages["order"])) & (df["action"] == "A")].sort_values("transaction").groupby("order").tail(1).index
        df["df_index_copy"] = df.index
        merged = pd.merge(cancel_messages, df.loc[indexer_messA_with_cancel], on = 'order')
        df.loc[merged["df_index_copy"].to_numpy(), "validity"] = merged["transaction_x"].to_numpy()

        df = df.loc[lambda x: ~(x["action"] == "D")] # delete all cancel messages (they are not needed anymore)
        
        # drop columns "Order ID" and "Action" and "df_index_copy"
        df = df.drop(["order", "action", "df_index_copy"], axis=1) # drop the action and order columns as they are not needed anymore (only add messages are left)

        df["start"] = df["start"].dt.tz_localize('UTC').dt.strftime('%Y-%m-%dT%H:%M:%SZ')
        df["transaction"] = df["transaction"].dt.tz_localize('UTC').dt.strftime('%Y-%m-%dT%H:%M:%S.%f').str[:-3]+'Z'
        df["validity"] = df["validity"].dt.tz_localize('UTC').dt.strftime('%Y-%m-%dT%H:%M:%S.%f').str[:-3]+'Z'
        
        return df

    def parse_market_data(self, sd: str, ed: str, marketdatapath: str, savepath: str, verbose: bool=True):
        """
        Parameters
        ----------
        sd : str
            Start date in the format "YYYY-MM-DD".

        ed : str
            End date in the format "YYYY-MM-DD".

        marketdatapath : str
            Path to the market data folder, as given by EPEX. The folder should contain subfolders for each year, which in turn contain subfolders for each month. We expect the zipped raw LOB data in these subfolders, in the daily structure provided by EPEX.
        
        savepath : str
            Path where the parsed data CSVs should be saved.

        verbose : bool
            If True, print progress messages.

        Processing Steps
        -------
        1. Sequentially load and pre-process all EPEX market Data, day after day.
        2. Processed data is stored in UTC time format, sorted by transaction time.
        3. Each CSV file contains the order book data for a single day. E.g., data in orderbook_2021-01-01 contains all orders placed on 2021-01-01 (UTC time).
        """

        if not os.path.exists(savepath):
            os.makedirs(savepath)

        start_date = pd.Timestamp(sd)
        end_date = pd.Timestamp(ed)
    
        if start_date > end_date:
            raise ValueError("Error: Start date is after end date.")
        if start_date.year < 2020:
            raise ValueError("Error: Years before 2020 are not supported.")
        dates = pd.date_range(start_date,end_date,freq="D")
        df1 = pd.DataFrame()
        df2 = pd.DataFrame()
        
        with tqdm(total=len(dates), desc="Loading and saving CSV data", ncols=100, disable=not verbose) as pbar:
            for dt1 in dates:
                pbar.set_description(f"Currently loading and saving date {str(dt1.date())} ... ")
                df1 = df2
                df2 = pd.DataFrame()
                dt2 = dt1 + pd.Timedelta(days=1)
                if df1.empty:
                    if dt1.year == 2020:
                        df1 = self._read_id_table_2020(dt1, marketdatapath)
                    elif dt1.year >= 2021:
                        df1 = self._read_id_table_2021(dt1, marketdatapath)
                    else:
                        raise ValueError("Error: Year not >= 2020")
                if dt2 <= end_date:
                    if dt2.year == 2020:
                        df2 = self._read_id_table_2020(dt2, marketdatapath)
                    elif dt2.year >= 2021:
                        df2 = self._read_id_table_2021(dt2, marketdatapath)
                    else:
                        raise ValueError("Error: Year not >= 2020")
        
                df = pd.concat([df1,df2])
                df = df.sort_values(by='transaction')
                df['transaction_date'] = pd.to_datetime(df['transaction']).dt.date  # Extract date part
                grouped = df.groupby('transaction_date')
                
                save_date = dt1.date()
                group = grouped.get_group(save_date)
                daily_filename = f"{savepath}orderbook_{save_date}.csv"
                group.drop(columns='transaction_date').sort_values(by='transaction').fillna("").to_csv(daily_filename)
                pbar.update(1)
        
        print("\nWriting CSV data completed.")

    def create_bins_from_csv(self, csv_list: list, save_path: str, verbose: bool=True):
        """
        Parameters
        ----------

        csv_list : list
            List of paths to CSV files containing the pre-processed order book data.

        save_path : str
            Path where the binary files should be saved under the same name as the CSV files.

        verbose : bool
            If True, print progress messages.

        Processing Steps
        -------

        1. Sequentially load the CSV files and convert them to binary files.
        2. Binary files are saved in the same directory as the CSV files.
        3. This enables much quicker loading of the data at runtime.
        """

        if not os.path.exists(save_path):
            os.makedirs(save_path)

        _sim = Simulation_cpp()
        with tqdm(total=len(csv_list), desc="Writing Binaries", ncols=100, disable=not verbose) as pbar:
            for csv_file_path in csv_list:
                filename = os.path.basename(csv_file_path)
                bin_file_path = os.path.join(save_path, filename.replace(".csv", ".bin"))
                pbar.set_description(f"Currently saving binary {bin_file_path.split("/")[-1]} ... ")
                _sim.writeOrderBinFromCSV(csv_file_path, bin_file_path)
                pbar.update(1)
        print("\nWriting Binaries completed.")

