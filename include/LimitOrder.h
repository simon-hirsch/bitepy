#ifndef LIMITORDER_H
#define LIMITORDER_H

#include <iostream>
#include <string>

class LimitOrder {
public:
    enum class Type {
        Sell,
        Buy
    };

    int64_t id;
    int64_t initialId;
    int64_t start;
    int64_t cancel;
    int64_t delivery;
    Type type;
    int32_t price;
    int32_t volume;

    // LimitOrder(int64_t id, int64_t initialId, Type type, int64_t start, int64_t cancel, int64_t delivery, int32_t price, int32_t volume);
    LimitOrder() = default;
};

#endif // LIMITORDER_H