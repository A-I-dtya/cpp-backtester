#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "Bar.hpp"
#include "Portfolio.hpp"
#include "Strategy.hpp"

class Engine
{
public:
    Engine(Strategy &strategy, Portfolio &portfolio,
           const std::vector<Bar> &bars, const std::string &symbol)
        : strategy_(strategy), portfolio_(portfolio),
          bars_(bars), symbol_(symbol) {}

    struct EquityPoint
    {
        std::string date;
        double value;
    };

    void run()
    {
        total_orders_ = 0;
        total_fills_ = 0;
        total_rejects_ = 0;
        equity_curve_.clear();
        equity_curve_.reserve(bars_.size());

        std::cout << "Starting backtest over " << bars_.size() << " bars...\n";
        std::cout << "Starting cash: $" << portfolio_.starting_cash() << "\n\n";

        for (std::size_t i = 0; i + 1 < bars_.size(); ++i)
        {
            const Bar &today = bars_[i];
            const Bar &tomorrow = bars_[i + 1];

            std::vector<Order> orders = strategy_.on_bar(today);
            total_orders_ += orders.size();

            for (const Order &order : orders)
            {
                const bool filled = portfolio_.apply_fill(
                    order.symbol, order.quantity, tomorrow.open);

                if (filled)
                    ++total_fills_;
                else
                    ++total_rejects_;
            }

            const double eq = portfolio_.equity(symbol_, today.close);
            equity_curve_.push_back({today.date, eq});
        }

        const Bar &last = bars_.back();
        equity_curve_.push_back({last.date, portfolio_.equity(symbol_, last.close)});
    }

    const std::vector<EquityPoint> &equity_curve() const { return equity_curve_; }
    std::size_t total_orders() const { return total_orders_; }
    std::size_t total_fills() const { return total_fills_; }
    std::size_t total_rejects() const { return total_rejects_; }

private:
    Strategy &strategy_;
    Portfolio &portfolio_;
    const std::vector<Bar> &bars_;
    std::string symbol_;

    std::vector<EquityPoint> equity_curve_;
    std::size_t total_orders_ = 0;
    std::size_t total_fills_ = 0;
    std::size_t total_rejects_ = 0;
};