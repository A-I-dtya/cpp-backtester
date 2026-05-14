#pragma once

#include <unordered_map>
#include <string>

class Portfolio {
public:
    explicit Portfolio(double starting_cash)
        :cash_(starting_cash), starting_cash_(starting_cash){}

    double cash() const{return cash_;}

    int position(const std::string& symbol)const{
        auto it = positions_.find(symbol);
        return (it == positions_.end())? 0:it->second;
    }

    void apply_fill(const std::string& symbol, int qty, double price){
        cash_ -= qty*price;
        positions_[symbol] += qty;
    }

    double equity(const std::string &symbol, double mark_price) const {
        return cash_ + position(symbol) * mark_price;
    }

    double starting_cash() const { return starting_cash_; }

private:
    double cash_;
    double starting_cash_;
    std::unordered_map<std::string, int> positions_;
};