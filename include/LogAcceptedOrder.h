#ifndef LOG_ACCEPTED_ORDER_H
#define LOG_ACCEPTED_ORDER_H

#include "LogLimitOrder.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>

class LogAcceptedOrder : public LogLimitOrder {
public:
    const int _dpRun;

    LogAcceptedOrder(
        int dpRun,
        int64_t time,
        int64_t id,
        int64_t initialId,
        int64_t start,
        int64_t cancel,
        int64_t delivery,
        LimitOrder::Type type,
        int price,
        int volume,
        bool partial,
        int partialVolume
    );

    static std::string epochToLocalDateTime(int64_t epochMillis);
    static std::string epochToLocalDateTimeMS(int64_t epochMillis);
};

#endif // LOG_ACCEPTED_ORDER_H