#pragma once

#include <iostream>
#include <vector>

#include "Bar.hpp"
#include "Portfolio.hpp"
#include "Strategy.hpp"

class Engine {
public:
    Engine(Strategy& strategy, Portfolio& portfolio,
           const std::vector<Bar>& bars, const std::string& symbol)
        : strategy_(strategy), portfolio_(portfolio),
          bars_(bars), symbol_(symbol) {}

    void run() {
        std::cout << "Starting backtest over " << bars_.size() << " bars...\n";
        std::cout << "Starting cash: $" << portfolio_.starting_cash() << "\n\n";

        std::size_t total_orders = 0;
        std::size_t total_fills  = 0;

        // Loop to bars_.size() - 1 because fills happen on the NEXT bar's open.
        for (std::size_t i = 0; i + 1 < bars_.size(); ++i) {
            const Bar& today    = bars_[i];
            const Bar& tomorrow = bars_[i + 1];

            std::vector<Order> orders = strategy_.on_bar(today);
            total_orders += orders.size();

            // Match each order at tomorrow's open.
            for (const Order& order : orders) {
                portfolio_.apply_fill(order.symbol, order.quantity, tomorrow.open);
                ++total_fills;
            }
        }

        // Final equity uses the last bar's close as the mark price.
        const double final_equity = portfolio_.equity(symbol_, bars_.back().close);
        const double pnl          = final_equity - portfolio_.starting_cash();
        const double pct_return   = 100.0 * pnl / portfolio_.starting_cash();

        std::cout << "Backtest complete.\n";
        std::cout << "Orders emitted: " << total_orders << "\n";
        std::cout << "Fills executed: " << total_fills  << "\n";
        std::cout << "Final position: " << portfolio_.position(symbol_) << " shares\n";
        std::cout << "Final cash:     $" << portfolio_.cash() << "\n";
        std::cout << "Final equity:   $" << final_equity << "\n";
        std::cout << "PnL:            $" << pnl
                  << " (" << pct_return << "%)\n";
    }

private:
    Strategy&             strategy_;
    Portfolio&            portfolio_;
    const std::vector<Bar>& bars_;
    std::string           symbol_;
};