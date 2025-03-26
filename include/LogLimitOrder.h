#ifndef LOGLIMITORDER_H
#define LOGLIMITORDER_H

#include <string>
#include <sstream>
#include <chrono>
#include <iomanip>
#include "LimitOrder.h"

class LogLimitOrder {
public:
    const int64_t time;
    const int64_t id;
    const int64_t initialId;
    const int64_t start;
    const int64_t cancel;
    const int64_t delivery;
    const LimitOrder::Type type;
    const int price;
    const int volume;
    const bool partial;
    const int partialVolume;

    LogLimitOrder(int64_t time, int64_t id, int64_t initialId, int64_t start, int64_t cancel, int64_t delivery, LimitOrder::Type type, int price, int volume, bool partial, int partialVolume);

    // Add a virtual destructor
    virtual ~LogLimitOrder() = default;

private:
    static std::string epochToDateTime(int64_t epochMillis);
};

#endif // LOGLIMITORDER_H