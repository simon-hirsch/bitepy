#ifndef EXEC_MARKET_ORDER_H
#define EXEC_MARKET_ORDER_H

#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include "LimitOrder.h"

class ExecMarketOrder {
public:
    int dpRun; // Number of the DP run
    int64_t time; // Current Time
    int64_t lastSolveTime; // Time of last solve
    int64_t hour; // Delivery hour
    int reward; // Price of the order without degradation costs and trading fees
    int rewardInclDegCosts; // Price of the order including degradation costs and trading fees
    int volume; // Volume of the order (>0 buy, <0 sell)
    LimitOrder::Type type; 
    int finalPos;
    double finalStor;
    int praeFinalPos;
    double praeFinalStor;
    double praeInitStorage;

    ExecMarketOrder(int dpRun, int64_t time, int64_t lastSolveTime, int64_t hour, int reward, int rewardInclDegCosts, int volume, LimitOrder::Type type, 
                    int finalPos, double finalStor, int praeFinalPos, double praeFinalStor, double praeInitStorage);

    static std::string epochToDateTime(int64_t epochMillis);
    static std::string epochToDateTimeMS(int64_t epochMillis);
};

#endif // EXEC_MARKET_ORDER_H