#include <iomanip>
#include <iostream>
#include <memory>

#include "Bar.hpp"
#include "CliArgs.hpp"
#include "CsvReader.hpp"
#include "Engine.hpp"
#include "EquityCurveWriter.hpp"
#include "Metrics.hpp"
#include "Portfolio.hpp"
#include "strategies/SmaCrossoverStrategy.hpp"
#include "strategies/BuyAndHoldStrategy.hpp"

std::unique_ptr<Strategy> make_strategy(const CliArgs& args) {
    if (args.strategy_name == "bh"){
        return std::make_unique<BuyAndHoldStrategy>(args.symbol, args.target_cash);
    }
    if (args.strategy_name == "sma")
    {
        return std::make_unique<SmaCrossoverStrategy>(
            args.symbol, args.short_period, args.long_period, args.target_cash);
    }
    throw std::runtime_error("Unknown strategy: " + args.strategy_name +
                             " (valid: 'bh', 'sma')");
}
int main(int argc, char *argv[])
{
    try
    {
        CliArgs args = parse_cli(argc, argv);
        if (args.show_help)
        {
            print_help();
            return 0;
        }

        std::vector<Bar> bars = read_csv(args.data_path);
        std::cout << "Loaded " << bars.size() << " bars from " << args.data_path << "\n";
        std::cout << "Strategy: " << args.strategy_name << " on " << args.symbol << "\n\n";

        Portfolio portfolio(args.starting_cash);
        std::unique_ptr<Strategy> strategy = make_strategy(args);
        Engine engine(*strategy, portfolio, bars, args.symbol);

        std::cout << std::fixed << std::setprecision(2);
        engine.run();

        const Metrics metrics = calculate_stats(engine.equity_curve());
        write_equity_curve(args.output_path, engine.equity_curve());

        std::cout << "\n=== Results ===\n";
        std::cout << "Orders emitted: " << engine.total_orders() << "\n";
        std::cout << "Fills executed: " << engine.total_fills() << "\n";
        std::cout << "Fills rejected: " << engine.total_rejects() << "\n";
        std::cout << "Final position: " << portfolio.position(args.symbol) << " shares\n";
        std::cout << "Final cash:     $" << portfolio.cash() << "\n";
        std::cout << "Final equity:   $" << engine.equity_curve().back().value << "\n";
        std::cout << "\n=== Metrics ===\n";
        std::cout << "Total return:   " << metrics.total_return_pct << "%\n";
        std::cout << "Max drawdown:   " << metrics.max_drawdown_pct << "%\n";
        std::cout << "Sharpe ratio:   " << metrics.sharpe << "\n";
        std::cout << "\nEquity curve written to: " << args.output_path << "\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}