#pragma once

#include <string>

#include "../Strategy.hpp"

class BuyAndHoldStrategy : public Strategy
{
public:
    BuyAndHoldStrategy(const std::string &symbol, int quantity)
        : symbol_(symbol), quantity_(quantity) {}

    std::vector<Order> on_bar(const Bar & /*bar*/) override
    {
        if (already_bought_)
            return {};
        already_bought_ = true;
        return {Order{symbol_, quantity_}};
    }

private:
    std::string symbol_;
    int quantity_;
    bool already_bought_ = false;
};#pragma once

#include <string>

#include "../Strategy.hpp"

class BuyAndHoldStrategy : public Strategy {
public:
    BuyAndHoldStrategy(const std::string& symbol, int quantity)
        : symbol_(symbol), quantity_(quantity) {}

    std::vector<Order> on_bar(const Bar& /*bar*/) override {
        if (already_bought_) return {};
        already_bought_ = true;
        return { Order{ symbol_, quantity_ } };
    }

private:
    std::string symbol_;
    int         quantity_;
    bool        already_bought_ = false;
};