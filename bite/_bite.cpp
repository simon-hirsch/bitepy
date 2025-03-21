#include <pybind11/pybind11.h>
#include <pybind11/stl.h>          // for automatic conversion of STL containers
// #include <pybind11/numpy.h>        // if you need NumPy arrays
#include <pybind11/chrono.h>       // if you need chrono conversions
// #include <pybind11/eigen.h>     // if you use Eigen, etc.

#include "cpp-source-submodule/bite/src/Simulation.h"

namespace py = pybind11;

PYBIND11_MODULE(_bite, m) {
    m.doc() = "pybind11 wrapper for the Simulation C++ code";
    // Params class
    // **Expose SimulationParameters class**
    py::class_<SimulationParameters>(m, "SimulationParameters")
        // Constructor with parameter file path
        // .def(py::init<const std::string&>(), py::arg("paramFilePath"))
        // Constructor with default values
        .def(py::init<>())

        // **Getter and Setter as properties**
        .def_property("storageMax",
                      &SimulationParameters::getStorageMaxPy,   // Getter
                      &SimulationParameters::setStorageMaxPy)   // Setter
        .def_property("startMonth",
                        &SimulationParameters::getStartMonthPy,
                        &SimulationParameters::setStartMonthPy)
        .def_property("endMonth",
                        &SimulationParameters::getEndMonthPy,
                        &SimulationParameters::setEndMonthPy)
        .def_property("startDay",
                        &SimulationParameters::getStartDayPy,
                        &SimulationParameters::setStartDayPy)
        .def_property("endDay",
                        &SimulationParameters::getEndDayPy,
                        &SimulationParameters::setEndDayPy)
        .def_property("startHour",
                        &SimulationParameters::getStartHourPy,
                        &SimulationParameters::setStartHourPy)
        .def_property("endHour",
                        &SimulationParameters::getEndHourPy,
                        &SimulationParameters::setEndHourPy)
        .def_property("startYear",
                        &SimulationParameters::getStartYearPy,
                        &SimulationParameters::setStartYearPy)
        .def_property("endYear",
                        &SimulationParameters::getEndYearPy,
                        &SimulationParameters::setEndYearPy)
        .def_property("dpFreq",
                        &SimulationParameters::getDpFreqPy,
                        &SimulationParameters::setDpFreqPy)
        .def_property("lossIn",
                        &SimulationParameters::getLossInPy,
                        &SimulationParameters::setLossInPy)
        .def_property("lossOut",
                        &SimulationParameters::getLossOutPy,
                        &SimulationParameters::setLossOutPy)
        .def_property("linDegCost",
                        &SimulationParameters::getLinDegCostPy,
                        &SimulationParameters::setLinDegCostPy)
        .def_property("tradingFee",
                        &SimulationParameters::getTradingFeePy,
                        &SimulationParameters::setTradingFeePy)
        .def_property("withdrawMax",
                        &SimulationParameters::getWithdrawMaxPy,
                        &SimulationParameters::setWithdrawMaxPy)
        .def_property("injectMax",
                        &SimulationParameters::getInjectMaxPy,
                        &SimulationParameters::setInjectMaxPy)
        .def_property("runType",
                        &SimulationParameters::getRunTypePy,
                        &SimulationParameters::setRunTypePy)
        .def_property("tempDir",
                        &SimulationParameters::getTempDirPy,
                        &SimulationParameters::setTempDirPy)
        .def_property("resultsDir",
                        &SimulationParameters::getResultsDirPy,
                        &SimulationParameters::setResultsDirPy)
        .def_property("cmaesParamPath",
                        &SimulationParameters::getCmaesParamPathPy,
                        &SimulationParameters::setCmaesParamPathPy)
        .def_property("numStorStates",
                        &SimulationParameters::getNumStorStatesPy,
                        &SimulationParameters::setNumStorStatesPy)
        .def_property("stoRoundDec",
                        &SimulationParameters::getStoRoundDecPy,
                        &SimulationParameters::setStoRoundDecPy)
        .def_property("pingDelay",
                        &SimulationParameters::getPingDelayPy,
                        &SimulationParameters::setPingDelayPy)
        .def_property("fixedSolveTime",
                        &SimulationParameters::getFixedSolveTimePy,
                        &SimulationParameters::setFixedSolveTimePy)
        .def_property("checkProfit",
                        &SimulationParameters::getCheckProfitPy,
                        &SimulationParameters::setCheckProfitPy)
        .def_property("checkLOExec",
                        &SimulationParameters::getCheckLOExecPy,
                        &SimulationParameters::setCheckLOExecPy)
        .def_property("useSliding",
                        &SimulationParameters::getUseSlidingPy,
                        &SimulationParameters::setUseSlidingPy)
        .def_property("foreHorizonStart",
                        &SimulationParameters::getForeHorizonStartPy,
                        &SimulationParameters::setForeHorizonStartPy)
        .def_property("foreHorizonEnd",
                        &SimulationParameters::getForeHorizonEndPy,
                        &SimulationParameters::setForeHorizonEndPy)
        .def_property("bidAskPenalty",
                        &SimulationParameters::getBidAskPenaltyPy,
                        &SimulationParameters::setBidAskPenaltyPy)
        
        // Method to print parameters
        .def("printParameters", &SimulationParameters::printParameters);


    // Expose the Simulation class
    py::class_<Simulation>(m, "Simulation_cpp")
        // constructor
        // .def(py::init<const std::string &>(),
        //      py::arg("param_path"))
        .def(py::init<>())
        // Expose 'params' as a property
        .def_property("params", 
                      [](Simulation &self) -> SimulationParameters& { return self.params; },  // getter
                      [](Simulation &self, SimulationParameters &new_params) { self.params = new_params; }) // setter
        // method to run
        .def("run",
            &Simulation::run,
            py::arg("isLastDataset"),
            "Run the simulation. 'isLast' indicates if this is the final run.")

        .def("addOrderQueueFromPandas", &Simulation::addOrderQueueFromPandas)
        .def("addOrderQueueFromBin", &Simulation::addOrderQueueFromBin)
        .def("writeOrderBinFromPandas", &Simulation::writeOrderBinFromPandas)
        .def("writeOrderBinFromCSV", &Simulation::writeOrderBinFromCSV)

        .def("loadForecastMapFromCSV", &Simulation::loadForecastMapFromCSV)
        .def("loadForecastMapFromPandas", &Simulation::loadForecastMapFromPandas)

        .def("loadParamMapFromCSV", &Simulation::loadParamMapFromCSV)

        // method to get results
        .def("printSimFinishStats", &Simulation::printSimFinishStats)

        // returnReward as double
        .def("returnReward", [](Simulation &self) {
            return self.returnReward();
        })

        .def("getLogs", [](Simulation &self) {
            // C++ -> Python
            auto decRecord = self.getDecisionData();
            py::list decisionRec;
            for (const auto &record : decRecord) {
                // Create a Python dictionary to store the record
                py::dict pyRecord;
                pyRecord["hour"] = record.hour;
                pyRecord["storage"] = record.storage;
                pyRecord["position"] = record.position;
                // pyRecord["final_reward"] = record.finalReward;
                pyRecord["real_reward"] = record.realReward;
                pyRecord["real_reward_no_deg"] = record.realRewardNoDeg;
                // Append the record to the results list
                decisionRec.append(pyRecord);
            }

            auto priceRecord = self.getPriceData();
            py::list priceRec;
            for (const auto &record : priceRecord) {
                // Create a Python dictionary to store the record
                py::dict pyRecord;
                pyRecord["hour"] = record.hour;
                pyRecord["low"] = record.low;
                pyRecord["high"] = record.high;
                pyRecord["last"] = record.last;
                pyRecord["wavg"] = record.wavg;
                pyRecord["id3"] = record.id3;
                pyRecord["id1"] = record.id1;
                pyRecord["volume"] = record.volume;
                priceRec.append(pyRecord);
            }

            py::list accOrderList;
            for (const auto &record : self.getAccOrders()) {
                // Create a Python dictionary to store the record
                py::dict pyRecord;

                pyRecord["dp_run"] = record._dpRun;
                pyRecord["time"] = LogAcceptedOrder::epochToLocalDateTimeMS(record.time);
                pyRecord["id"] = record.id;
                pyRecord["initial_id"] = record.initialId;
                pyRecord["start"] = LogAcceptedOrder::epochToLocalDateTimeMS(record.start);
                pyRecord["cancel"] = LogAcceptedOrder::epochToLocalDateTimeMS(record.cancel);
                pyRecord["delivery"] = LogAcceptedOrder::epochToLocalDateTime(record.delivery);
                pyRecord["type"] = record.type == LimitOrder::Type::Buy ? "Buy" : "Sell";
                pyRecord["price"] = record.price / 100.0;
                pyRecord["volume"] = record.volume / 10.0;
                pyRecord["partial"] = record.partial;
                pyRecord["partial_volume"] = record.partialVolume / 10.0;
                accOrderList.append(pyRecord);
            }

            py::list execOrderList;
            for (const auto &record : self.getExOrders()) {
                // Create a Python dictionary to store the record
                py::dict pyRecord;

                pyRecord["dp_run"] = record.dpRun;
                pyRecord["time"] = ExecMarketOrder::epochToDateTimeMS(record.time);
                pyRecord["last_solve_time"] = ExecMarketOrder::epochToDateTimeMS(record.lastSolveTime);
                pyRecord["hour"] = ExecMarketOrder::epochToDateTime(record.hour);
                pyRecord["reward"] = record.reward / 1000.0;
                pyRecord["reward_incl_deg_costs"] = record.rewardInclDegCosts / 1000.0;
                pyRecord["volume"] = record.volume / 10.0;
                pyRecord["type"] = record.type == LimitOrder::Type::Buy ? "Buy" : "Sell";
                pyRecord["final_pos"] = record.finalPos / 10.0;
                pyRecord["final_stor"] = record.finalStor / 10.0;
                // pyRecord["prae_final_pos"] = record.praeFinalPos / 10.0;
                // pyRecord["prae_final_stor"] = record.praeFinalStor / 10.0;
                // pyRecord["prae_init_storage"] = record.praeInitStorage / 10.0;
                execOrderList.append(pyRecord);
            }

            py::list foreOrderList;
            for (const auto &record : self.getForeOrders()) {
                // Create a Python dictionary to store the record
                py::dict pyRecord;

                pyRecord["dp_run"] = record.dpRun;
                pyRecord["time"] = ForeLogOrder::epochToDateTimeMS(record.time);
                pyRecord["last_solve_time"] = ForeLogOrder::epochToDateTimeMS(record.lastSolveTime);
                pyRecord["hour"] = ForeLogOrder::epochToDateTime(record.hour);
                pyRecord["reward"] = record.reward / 1000.0;
                pyRecord["volume"] = record.volume / 10.0;
                pyRecord["volume_previous"] = record.volumePrevious / 10.0;
                foreOrderList.append(pyRecord);
            }

            py::list removedOrdersList;
            for (const auto &record : self.getRemOrders()) {
                // Create a Python dictionary to store the record
                py::dict pyRecord;

                pyRecord["dp_run"] = record.dpRun;
                pyRecord["time"] = ExecMarketOrder::epochToDateTimeMS(record.time);
                pyRecord["last_solve_time"] = ExecMarketOrder::epochToDateTimeMS(record.lastSolveTime);
                pyRecord["hour"] = ExecMarketOrder::epochToDateTime(record.hour);
                pyRecord["reward"] = record.reward / 1000.0;
                pyRecord["reward_incl_deg_costs"] = record.rewardInclDegCosts / 1000.0;
                pyRecord["volume"] = record.volume / 10.0;
                pyRecord["type"] = record.type == LimitOrder::Type::Buy ? "Buy" : "Sell";
                pyRecord["final_pos"] = record.finalPos / 10.0;
                pyRecord["final_stor"] = record.finalStor / 10.0;
                // pyRecord["prae_final_pos"] = record.praeFinalPos / 10.0;
                // pyRecord["prae_final_stor"] = record.praeFinalStor / 10.0;
                // pyRecord["prae_init_storage"] = record.praeInitStorage / 10.0;
                removedOrdersList.append(pyRecord);
            }

            py::list balOrderList;
            for (const auto &record : self.getBalOrders()) {
                // Create a Python dictionary to store the record
                py::dict pyRecord;

                pyRecord["dp_run"] = record.dpRun;
                pyRecord["time"] = BalancingOrder::epochToDateTimeMS(record.time);
                pyRecord["hour"] = BalancingOrder::epochToDateTime(record.hour);
                pyRecord["volume"] = record.volume / 10.0;
                balOrderList.append(pyRecord);
            }
            // Return the results to Python
            return py::make_tuple(decisionRec, priceRec, accOrderList, execOrderList, foreOrderList, removedOrdersList, balOrderList);
        })

        .def("return_vol_price_pairs", [](Simulation &self, const bool last, const int frequency, const std::vector<int>& volumes) {
            py::list vol_price_list;
            std::map<int64_t, std::map<int64_t, std::map<int, std::pair<int,int>>>> priceVolMap = self.return_vol_price_pairs(last, frequency, volumes);
            
            for (const auto& [currTime, innerMap] : priceVolMap) {
                for (const auto& [delHour, innerMap2] : innerMap) {
                    for (const auto& [volume, price] : innerMap2) {
                        py::dict pyRecord;
                        pyRecord["current_time"] = ExecMarketOrder::epochToDateTimeMS(currTime);
                        pyRecord["delivery_hour"] = ExecMarketOrder::epochToDateTime(delHour);
                        pyRecord["volume"] = volume / 10.0;
                        pyRecord["price_full"] = price.first / 1000.0;
                        pyRecord["worst_accepted_price"] = price.second / 100.0;
                        vol_price_list.append(pyRecord);
                    }
                }
            }

            return vol_price_list;
        }, py::arg("last"), py::arg("frequency"), py::arg("volumes"),
        "Returns a list of dictionaries with volume and price pairs.");
}