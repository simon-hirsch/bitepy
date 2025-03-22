# Data Preprocessing

Our `Data` class allows users to read-in raw zipped LOB Data from EPEX (2020 and later), process them accordingly and save each trading day as a separate CSV file. All Data is ultimately stored in UTC timezone format.
We show and test this for German Market Data of the years 2020 and 2021, specifically using the 1h products of the continuous intraday market, but this can easily be adapted to other regions or other products.
Inputs to the parsing function simply are the `start-day` and `end-day` of the data we want to parse, plus the `path` to the zipped EPEX market data.

::: bite.Data