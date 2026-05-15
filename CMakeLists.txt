cmake_minimum_required(VERSION 3.20)

project(cpp_backtester
    VERSION 0.1.0
    DESCRIPTION "A simple C++ backtester for OHLCV stock data"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(backtester
    src/main.cpp
    src/Bar.hpp
    src/Order.hpp
    src/Strategy.hpp
    src/Portfolio.hpp
    src/Engine.hpp
    src/Metrics.hpp
    src/CsvReader.hpp
    src/CliArgs.hpp
    src/EquityCurveWriter.hpp
    src/indicators/SimpleMovingAverage.hpp
    src/strategies/DoNothingStrategy.hpp
    src/strategies/BuyAndHoldStrategy.hpp
    src/strategies/SmaCrossoverStrategy.hpp
)


target_compile_options(backtester PRIVATE
    -Wall -Wextra -Wpedantic
)