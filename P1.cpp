
#include <string>
#include <cstdint>
#include <iostream>

// static polymorphism
// 性能更高，没有虚函数表的额外开销
struct MarketDataA {
    char instrumentId[20];  //股票代码
    double askPrice1;       //申卖价一
    uint64_t askVolume1;    //申卖量一
    double bidPrice1;       //申买价一
    uint64_t bidVolume1;    //申买量一
    double askPrice2;       //申卖价二
    uint64_t askVolume2;    //申卖量二
    double bidPrice2;       //申买价二
    uint64_t bidVolume2;    //申买量二
    double askPrice3;       //申卖价三
    uint64_t askVolume3;    //申卖量三
    double bidPrice3;       //申买价三
    uint64_t bidVolume3;    //申买量三
    double askPrice4;       //申卖价四
    uint64_t askVolume4;    //申卖量四
    double bidPrice4;       //申买价四
    uint64_t bidVolume4;    //申买量四
    double askPrice5;       //申卖价五
    uint64_t askVolume5;    //申卖量五
    double bidPrice5;       //申买价五
    uint64_t bidVolume5;    //申买量五
    // 其余字段
    // .......
    std::string GetCode() const {
        return instrumentId;
    }

    double GetAskPrice() const {
        return askPrice1;
    }

    double GetBidPrice() const {
        return bidPrice1;
    }
};

struct MarketDataB {
    std::string code;       //股票代码
    double askPrice[5];     //申卖价
    uint64_t askVolume[5];  //申卖量
    double bidPrice[5];     //申买价
    uint64_t bidVolume[5];  //申买量
    // 其余字段
    // .......
    std::string GetCode() const {
        return code;
    }

    double GetAskPrice() const {
        return askPrice[0];
    }

    double GetBidPrice() const {
        return bidPrice[0];
    }
};

template <typename T>
void printMidPrice(const T& data) {
    std::cout << "symbol: " << data.GetCode()
        << ", mid price: " << (data.GetAskPrice() + data.GetBidPrice()) / 2
        << std::endl;
}

/*
// dynamic polymorphism
struct IMarketData {
    virtual ~IMarketData() = default;
    virtual std::string GetCode() const = 0;
    virtual double GetAskPrice() const = 0;
    virtual double GetBidPrice() const = 0;
};

struct MarketDataA: IMarketData {
    char instrumentId[20];  //股票代码
    double askPrice1;       //申卖价一
    uint64_t askVolume1;    //申卖量一
    double bidPrice1;       //申买价一
    uint64_t bidVolume1;    //申买量一
    double askPrice2;       //申卖价二
    uint64_t askVolume2;    //申卖量二
    double bidPrice2;       //申买价二
    uint64_t bidVolume2;    //申买量二
    double askPrice3;       //申卖价三
    uint64_t askVolume3;    //申卖量三
    double bidPrice3;       //申买价三
    uint64_t bidVolume3;    //申买量三
    double askPrice4;       //申卖价四
    uint64_t askVolume4;    //申卖量四
    double bidPrice4;       //申买价四
    uint64_t bidVolume4;    //申买量四
    double askPrice5;       //申卖价五
    uint64_t askVolume5;    //申卖量五
    double bidPrice5;       //申买价五
    uint64_t bidVolume5;    //申买量五
    // 其余字段
    // .......
    std::string GetCode() const override {
        return instrumentId;
    }

    double GetAskPrice() const override {
        return askPrice1;
    }

    double GetBidPrice() const override {
        return bidPrice1;
    }
};

struct MarketDataB: IMarketData {
    std::string code;       //股票代码
    double askPrice[5];     //申卖价
    uint64_t askVolume[5];  //申卖量
    double bidPrice[5];     //申买价
    uint64_t bidVolume[5];  //申买量
    // 其余字段
    // .......
    std::string GetCode() const override {
        return code;
    }

    double GetAskPrice() const override {
        return askPrice[0];
    }

    double GetBidPrice() const override {
        return bidPrice[0];
    }
};

void printMidPrice(const IMarketData& data) {
    std::cout << "symbol: " << data.GetCode()
        << ", mid price: " << (data.GetAskPrice() + data.GetBidPrice()) / 2
        << std::endl;
}
*/

void onQuoteA(const MarketDataA& quote) {
    printMidPrice(quote);
}

void onQuoteB(const MarketDataB& quote) {
    printMidPrice(quote);
}

int main() {
    MarketDataA a;
    strcpy_s(a.instrumentId, "MarketDataA");
    a.askPrice1 = 20;
    a.bidPrice1 = 10;
    onQuoteA(a);

    MarketDataB b;
    b.code = "MarketDataB";
    b.askPrice[0] = 40;
    b.bidPrice[0] = 30;
    onQuoteB(b);

    return 0;
}

