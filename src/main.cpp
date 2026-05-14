#include <iomanip>
#include <iostream>

#include "Bar.hpp"
#include "CsvReader.hpp"
#include "Engine.hpp"
#include "EquityCurveWriter.hpp"
#include "Metrics.hpp"
#include "Portfolio.hpp"
#include "strategies/BuyAndHoldStrategy.hpp"

int main(int argc, char *argv[])
{
    const std::string path = (argc > 1) ? argv[1] : "data/AAPL.csv";
    const std::string symbol = "AAPL";
    const std::string output_path = "equity_curve.csv";

    try
    {
        std::vector<Bar> bars = read_csv(path);
        std::cout << "Loaded " << bars.size() << " bars from " << path << "\n\n";

        Portfolio portfolio(10000000.0);
        BuyAndHoldStrategy strategy(symbol, 9500000.0);
        Engine engine(strategy, portfolio, bars, symbol);

        std::cout << std::fixed << std::setprecision(2);
        engine.run();

        const Metrics metrics = calculate_stats(engine.equity_curve());
        write_equity_curve(output_path, engine.equity_curve());

        std::cout << "\n=== Results ===\n";
        std::cout << "Orders emitted: " << engine.total_orders() << "\n";
        std::cout << "Fills executed: " << engine.total_fills() << "\n";
        std::cout << "Fills rejected:  " << engine.total_rejects() << "\n";
        std::cout << "Final position: " << portfolio.position(symbol) << " shares\n";
        std::cout << "Final cash:     $" << portfolio.cash() << "\n";
        std::cout << "Final equity:   $" << engine.equity_curve().back().value << "\n";
        std::cout << "\n=== Metrics ===\n";
        std::cout << "Total return:   " << metrics.total_return_pct << "%\n";
        std::cout << "Max drawdown:   " << metrics.max_drawdown_pct << "%\n";
        std::cout << "Sharpe ratio:   " << metrics.sharpe<< "\n";
        std::cout << "\nEquity curve written to: " << output_path << "\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}