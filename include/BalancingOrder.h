#ifndef BAL_ORDER_H
#define BAL_ORDER_H

#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>

class BalancingOrder {
public:
    int dpRun; // Number of the DP run
    int64_t time; // Current Time
    int64_t hour; // Delivery hour
    double volume; // Volume of the order < 0: we had to buy balancing energy

    BalancingOrder(int dpRun, int64_t time, int64_t hour, double volume);
    
    static std::string epochToDateTime(int64_t epochMillis);
    static std::string epochToDateTimeMS(int64_t epochMillis);
};

#endif // BAL_ORDER_H