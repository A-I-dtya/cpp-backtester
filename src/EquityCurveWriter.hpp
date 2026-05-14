#pragma once

#include<fstream>
#include<stdexcept>
#include<string>
#include<vector>

#include "Engine.hpp"

inline void write_equity_curve(const std::string& path, const std::vector<Engine :: EquityPoint>& curve){
    std::ofstream out(path);
    if(!out.is_open()){
        throw std::runtime_error("Could Not open file: " + path);
    }
    out<< "Date,Equity\n";
    for(const auto& it:curve){
        out << it.date << "," << it.value <<"\n";
    }
}