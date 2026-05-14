#pragma once

#include<vector>

#include "Bar.hpp"
#include "Order.hpp"

class Strategy{
public:
    virtual ~Strategy() = default;
    virtual std::vector<Order> on_bar(const Bar& bar) = 0;
};