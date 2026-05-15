#pragma once

#include <string>
#include <unordered_map>

class Portfolio
{
public:
    explicit Portfolio(double starting_cash)
        : cash_(starting_cash), starting_cash_(starting_cash) {}

    double cash() const { return cash_; }

    int position(const std::string &symbol) const
    {
        auto it = positions_.find(symbol);
        return (it == positions_.end()) ? 0 : it->second;
    }

    bool apply_fill(const std::string &symbol, int qty, double price)
    {
        if (qty > 0)
        {
            const double cost = qty * price;
            if (cost > cash_)
                return false;
        }
        else if (qty < 0)
        {

            if (position(symbol) < -qty)
                return false;
        }
        else
        {
            // Zero-qty orders are malformed.
            return false;
        }

        cash_ -= qty * price;
        positions_[symbol] += qty;
        return true;
    }

    double equity(const std::string &symbol, double mark_price) const
    {
        return cash_ + position(symbol) * mark_price;
    }

    double starting_cash() const { return starting_cash_; }

private:
    double cash_;
    double starting_cash_;
    std::unordered_map<std::string, int> positions_;
};