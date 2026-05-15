#pragma once 

#include<cstddef>
#include<deque>

class SimpleMovingAverage{
public:
    explicit SimpleMovingAverage(std::size_t period): period_(period) {}
    
    void update(double price){
        window_.push_back(price);
        sum_+=price;
        if(window_.size() > period_){
            sum_ -= window_.front();
            window_.pop_front();
        }
    }

    bool ready() const { return window_.size() ==   period_; }
    double value() const {return ready()? sum_/period_: 0.0; }

private:
    std::size_t period_;
    std::deque<double> window_;
    double sum_ = 0.0;
};