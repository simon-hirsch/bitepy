#ifndef SIMULATION_H
#define SIMULATION_H

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <utility>

#include "SimulationParameters.h"

// The public API is wrapped in a namespace.
namespace simulation {

// -----------------------------------------------------------------------------
// Minimal public types for returning simulation data
// (These could be further simplified if you want to hide more internal details.)
struct DecisionRecord {
    int64_t hour;
    double storage;
    double position;
    double finalReward;
    double realReward;
    double realRewardNoDeg;
};

struct PriceChart {
    int64_t hour;
    double low;
    double high;
    double last;
    double wavg;
    double id3;
    double id1;
    double volume;
};

// -----------------------------------------------------------------------------
// Public API for the simulation engine.
class Simulation {
public:
    Simulation();
    ~Simulation();

    // The public simulation parameters
    SimulationParameters params;

    // Main functions exposed to Python:
    void run(bool lastSet);

    // Methods for handling order queues:
    void addOrderQueueFromPandas(const std::vector<int64_t>& ids,
                                 const std::vector<int64_t>& initials,
                                 const std::vector<std::string>& sides,
                                 const std::vector<std::string>& starts,
                                 const std::vector<std::string>& transactions,
                                 const std::vector<std::string>& validities,
                                 const std::vector<double>& prices,
                                 const std::vector<double>& quantities);

    void addOrderQueueFromBin(const std::string& pathName);
    void writeOrderBinFromPandas(const std::string& pathName,
                                 const std::vector<int64_t>& ids,
                                 const std::vector<int64_t>& initials,
                                 const std::vector<std::string>& sides,
                                 const std::vector<std::string>& starts,
                                 const std::vector<std::string>& transactions,
                                 const std::vector<std::string>& validities,
                                 const std::vector<double>& prices,
                                 const std::vector<double>& quantities);
    void writeOrderBinFromCSV(const std::string& pathName, const std::string& saveName);

    // Forecast and parameter map loading
    void loadForecastMapFromPandas(const std::vector<std::string>& delivery_times,
                                   const std::vector<std::string>& placement_times,
                                   const std::vector<double>& buy_price,
                                   const std::vector<double>& sell_price);
    void loadForecastMapFromCSV(const std::string& path);
    void loadParamMapFromCSV(const std::string& path);

    // Methods to retrieve simulation results
    void printOrderQueue() const;
    void printParameters() const;
    int64_t getNumSolves() const;
    void printSimFinishStats() const;
    double returnReward() const;

    std::vector<DecisionRecord> getDecisionData() const;
    std::vector<PriceChart> getPriceData() const;
    std::vector<std::string> getAccOrders() const;
    std::vector<std::string> getExOrders() const;
    std::vector<std::string> getForeOrders() const;
    std::vector<std::string> getRemOrders() const;
    std::vector<std::string> getBalOrders() const;

    // Clock and elapsed time functions
    void startClock();
    double getElapsedTimeInSeconds() const;

    // Returns a nested map with volume–price pairs
    std::map<int64_t, std::map<int64_t, std::map<int, std::pair<int, int>>>>
    return_vol_price_pairs(bool last, int frequency, const std::vector<int>& volumes);

private:
    // Hide all internal data and helper functions.
    struct Impl;
    Impl* impl;
};

} // namespace simulation

#endif // SIMULATION_H