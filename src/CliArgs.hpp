#pragma once

#include<iostream>
#include<stdexcept>
#include <string>

struct CliArgs{
    std::string data_path = "data/AAPL.csv";
    std::string symbol = "AAPL";
    std::string output_path = "equity_curve.csv";
    std::string strategy_name = "bh"; 
    double starting_cash = 10000000.0;
    double target_cash = 9500000.0; 
    std::size_t short_period = 50;
    std::size_t long_period = 200;
    bool show_help = false;
};

inline void print_help(){
    std::cout << "Usage: backtester\n"
                 "\n"
                 "Options\n"
                 "  --data PATH        Path to OHLCV CSV (default: data/AAPL.csv)\n"
                 "  --symbol SYMBOL    Asset symbol (default: AAPL)\n"
                 "  --output PATH      Equity curve output file (default: equity_curve.csv)\n"
                 "  --strategy NAME    Strategy: 'bh' or 'sma' (default: bh)\n"
                 "  --cash AMOUNT      Starting cash (default: 10000000)\n"
                 "  --target AMOUNT    Dollars to deploy per trade (default: 9500000)\n"
                 "  --short N          Short SMA period (default: 50)\n"
                 "  --long N           Long SMA period (default: 200)\n"
                 "  --help             Show this help and exit\n";
}

inline CliArgs parse_cli(int argc, char* argv[]){
    CliArgs args;

    auto next = [&](int& i, const std::string& flag) ->std::string{
        if(i+1>=argc){
            throw std::runtime_error("Missing value for " + flag);
        }
        return argv[++i];
    };
    
    for (int i = 1; i < argc; ++i)
    {
        const std::string arg = argv[i];

        if (arg == "--help")
        {
            args.show_help = true;
        }
        else if (arg == "--data")
        {
            args.data_path = next(i, arg);
        }
        else if (arg == "--symbol")
        {
            args.symbol = next(i, arg);
        }
        else if (arg == "--output")
        {
            args.output_path = next(i, arg);
        }
        else if (arg == "--strategy")
        {
            args.strategy_name = next(i, arg);
        }
        else if (arg == "--cash")
        {
            args.starting_cash = std::stod(next(i, arg));
        }
        else if (arg == "--target")
        {
            args.target_cash = std::stod(next(i, arg));
        }
        else if (arg == "--short")
        {
            args.short_period = std::stoul(next(i, arg));
        }
        else if (arg == "--long")
        {
            args.long_period = std::stoul(next(i, arg));
        }
        else
        {
            throw std::runtime_error("Unknown argument: " + arg);
        }
    }

    return args;
}