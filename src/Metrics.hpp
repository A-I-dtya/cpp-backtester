#pragma once

#include<cmath>
#include<vector>

#include "Engine.hpp"

struct Metrics {
    double total_return_pct = 0.0;
    double max_drawdown_pct = 0.0;
    double sharpe = 0.0;
};

inline Metrics calculate_stats(const std::vector<Engine::EquityPoint>& curve){
    Metrics stats;

    if (curve.size() < 2) return stats;

    const double start = curve.front().value; 
    const double end = curve.back().value;
    
    stats.total_return_pct = 100 * (end - start)/ start;

    double peak = curve.front().value;
    double max_dd = 0.0;

    for(const auto& it : curve){
        if(it.value > peak) peak = it.value;
        const double dd = (peak - it.value) / peak;

        max_dd = std::max(max_dd,dd);
    }
    stats.max_drawdown_pct = 100 * max_dd;

    std::vector<double> returns;

    returns.reserve(curve.size() - 1);

    for(std::size_t i = 1; i < curve.size(); ++i){
        const double prev = curve[i-1].value;
        const double cur = curve[i].value;

        if(prev > 0.0) returns.push_back((cur - prev)/prev);
    }

    if (returns.size() < 2)
        return stats;

    double sum = 0.0;
    for(double r: returns)sum+=r;
    const double mean = sum / returns.size();

    double sq_sum = 0.0;
    for(double r: returns){
        double diff = r - mean;
        sq_sum += diff*diff;
    }

    const double variance = sq_sum/(returns.size() -1);

    const double stddev = std::sqrt(variance);

    if(stddev > 0.0){
        stats.sharpe = (mean/stddev) * std::sqrt(252.0);
    }

    return stats;
}