#pragma once

#include <string>

#include "../Strategy.hpp"
#include "../indicators/SimpleMovingAverage.hpp"

class SmaCrossoverStrategy : public Strategy
{
public:
    // target_dollars: how much $ to deploy per trade (not share count).
    SmaCrossoverStrategy(const std::string &symbol,
                         std::size_t short_period,
                         std::size_t long_period,
                         double target_dollars)
        : symbol_(symbol),
          short_sma_(short_period),
          long_sma_(long_period),
          target_dollars_(target_dollars) {}

    std::vector<Order> on_bar(const Bar &bar) override
    {
        short_sma_.update(bar.close);
        long_sma_.update(bar.close);

        if (!short_sma_.ready() || !long_sma_.ready())
            return {};

        const double s = short_sma_.value();
        const double l = long_sma_.value();
        const bool short_above = s > l;

        std::vector<Order> orders;

        if (initialized_)
        {
            if (short_above && !prev_short_above_ && !holding_)
            {
                // Golden cross: size the buy based on current price.
                const int qty = static_cast<int>(target_dollars_ / bar.close);
                if (qty > 0)
                {
                    orders.push_back({symbol_, qty});
                    held_qty_ = qty;
                    holding_ = true;
                }
            }
            else if (!short_above && prev_short_above_ && holding_)
            {
                // Death cross: sell exactly what we own.
                orders.push_back({symbol_, -held_qty_});
                held_qty_ = 0;
                holding_ = false;
            }
        }

        prev_short_above_ = short_above;
        initialized_ = true;
        return orders;
    }

private:
    std::string symbol_;
    SimpleMovingAverage short_sma_;
    SimpleMovingAverage long_sma_;
    double target_dollars_;

    bool prev_short_above_ = false;
    bool initialized_ = false;
    bool holding_ = false;
    int held_qty_ = 0;
};