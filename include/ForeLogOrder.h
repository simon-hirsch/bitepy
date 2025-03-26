#ifndef FORE_LOG_ORDER_H
#define FORE_LOG_ORDER_H

#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include "LimitOrder.h"

class ForeLogOrder {
public:
    int dpRun; // Number of the DP run
    int64_t time; // Current Time
    int64_t lastSolveTime; // Time of last solve
    int64_t hour; // Delivery hour
    int reward; // Price of the order without degradation costs and trading fees
    int volume; // Volume of the order (>0 buy, <0 sell)
    int volumePrevious;

    ForeLogOrder(int dpRun, int64_t time, int64_t lastSolveTime, int64_t hour, int reward, int volume, int volumePrevious);

    static std::string epochToDateTime(int64_t epochMillis);
    static std::string epochToDateTimeMS(int64_t epochMillis);
};

#endif // FORE_LOG_ORDER_H