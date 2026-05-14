#pragma once

#include <string>

#include "../Strategy.hpp"

class BuyAndHoldStrategy : public Strategy {
public:
    BuyAndHoldStrategy(const std::string& symbol, double cash_to_use)
        : symbol_(symbol), cash_to_use_(cash_to_use) {}

    std::vector<Order> on_bar(const Bar& bar) override {
        if (already_bought_) return {};
        already_bought_ = true;
        const int qty = static_cast<int>(cash_to_use_ / bar.close);
        if (qty <= 0)
            return {};
        return { Order{ symbol_, qty } };
    }

private:
    std::string symbol_;
    double      cash_to_use_;
    bool        already_bought_ = false;
};