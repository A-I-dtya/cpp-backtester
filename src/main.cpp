#include <iostream>
#include <iomanip>

#include "Bar.hpp"
#include "CsvReader.hpp"
#include "Engine.hpp"
#include "Portfolio.hpp"
#include "strategies\BuyAndHoldStrategy.hpp"

int main(int argc, char *argv[])
{
    std::string path = (argc > 1)?argv[1]: "data/AAPL.csv";
    const std::string symbol = "AAPL";

    try {
        std::vector<Bar> bars = read_csv(path);
        std::cout << "Loaded " << bars.size() << " bars from " << path  << "\n\n";

        Portfolio portfolio(10000000.0);
        BuyAndHoldStrategy strategy(symbol, 100);
        Engine engine(strategy, portfolio, bars, symbol);

        std::cout << std::fixed << std::setprecision(2);
        engine.run();
    } catch (const std::exception& e){
        std::cerr << "Error" << e.what() << "\n";
        return 1;
    }
    return 0;
}