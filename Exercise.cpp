#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <algorithm>

// 保存每个 symbol 的统计数据
struct TradeStats {
    long long lastTimestamp = 0;
    long long maxTimeGap = 0;
    long long totalVolume = 0;
    long long weightedPriceSum = 0;
    int maxPrice = 0;
    bool firstTrade = true;
};

int main() {
    std::ifstream file("input.csv");  // 需要修改名称为单次输入的文件名
    if (!file) {
        std::cerr << "Error: Could not open 'input'" << std::endl;
        return 1;
    }
    
    std::map<std::string, TradeStats> tradeData;
    std::string line;
    
    // 读取 CSV 数据：<TimeStamp>,<Symbol>,<Quantity>,<Price>
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::istringstream lineStream(line);
        std::string token;
        
        if (!std::getline(lineStream, token, ',')) continue;
        long long timestamp = std::stoll(token);
        
        std::string symbol;
        if (!std::getline(lineStream, symbol, ',')) continue;
        
        if (!std::getline(lineStream, token, ',')) continue;
        int quantity = std::stoi(token);
        
        if (!std::getline(lineStream, token, ',')) continue;
        int price = std::stoi(token);
        
        TradeStats &stats = tradeData[symbol];
        if (stats.firstTrade) {
            stats.lastTimestamp = timestamp;
            stats.firstTrade = false;
        } else {
            long long gap = timestamp - stats.lastTimestamp;
            stats.maxTimeGap = std::max(stats.maxTimeGap, gap);
            stats.lastTimestamp = timestamp;
        }
        
        stats.totalVolume += quantity;
        stats.weightedPriceSum += static_cast<long long>(quantity) * price;
        stats.maxPrice = std::max(stats.maxPrice, price);
    }
    
    file.close();
    
    std::ofstream outputFile("output.csv");  // 需要修改名称为单次输出的文件名
    if (!outputFile) {
        std::cerr << "Error: Could not open 'output' for writing" << std::endl;
        return 1;
    }
    
    // 输出 CSV 格式数据：<symbol>,<MaxTimeGap>,<Volume>,<WeightedAveragePrice>,<MaxPrice>
    for (const auto &pair : tradeData) {
        const std::string &symbol = pair.first;
        const TradeStats &stats = pair.second;
        int weightedAvgPrice = static_cast<int>(stats.weightedPriceSum / stats.totalVolume);
        
        outputFile << symbol << ","
                   << stats.maxTimeGap << ","
                   << stats.totalVolume << ","
                   << weightedAvgPrice << ","
                   << stats.maxPrice << "\n";
    }
    
    outputFile.close();
    return 0;
}
