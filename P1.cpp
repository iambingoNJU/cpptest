
#include <string>
#include <cstdint>
#include <iostream>

// static polymorphism
// ���ܸ��ߣ�û���麯����Ķ��⿪��
struct MarketDataA {
    char instrumentId[20];  //��Ʊ����
    double askPrice1;       //������һ
    uint64_t askVolume1;    //������һ
    double bidPrice1;       //�����һ
    uint64_t bidVolume1;    //������һ
    double askPrice2;       //�����۶�
    uint64_t askVolume2;    //��������
    double bidPrice2;       //����۶�
    uint64_t bidVolume2;    //��������
    double askPrice3;       //��������
    uint64_t askVolume3;    //��������
    double bidPrice3;       //�������
    uint64_t bidVolume3;    //��������
    double askPrice4;       //��������
    uint64_t askVolume4;    //��������
    double bidPrice4;       //�������
    uint64_t bidVolume4;    //��������
    double askPrice5;       //��������
    uint64_t askVolume5;    //��������
    double bidPrice5;       //�������
    uint64_t bidVolume5;    //��������
    // �����ֶ�
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
    std::string code;       //��Ʊ����
    double askPrice[5];     //������
    uint64_t askVolume[5];  //������
    double bidPrice[5];     //�����
    uint64_t bidVolume[5];  //������
    // �����ֶ�
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
    char instrumentId[20];  //��Ʊ����
    double askPrice1;       //������һ
    uint64_t askVolume1;    //������һ
    double bidPrice1;       //�����һ
    uint64_t bidVolume1;    //������һ
    double askPrice2;       //�����۶�
    uint64_t askVolume2;    //��������
    double bidPrice2;       //����۶�
    uint64_t bidVolume2;    //��������
    double askPrice3;       //��������
    uint64_t askVolume3;    //��������
    double bidPrice3;       //�������
    uint64_t bidVolume3;    //��������
    double askPrice4;       //��������
    uint64_t askVolume4;    //��������
    double bidPrice4;       //�������
    uint64_t bidVolume4;    //��������
    double askPrice5;       //��������
    uint64_t askVolume5;    //��������
    double bidPrice5;       //�������
    uint64_t bidVolume5;    //��������
    // �����ֶ�
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
    std::string code;       //��Ʊ����
    double askPrice[5];     //������
    uint64_t askVolume[5];  //������
    double bidPrice[5];     //�����
    uint64_t bidVolume[5];  //������
    // �����ֶ�
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

