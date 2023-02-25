#include <cstdint>
#include <string>
#include <cstring>
#include <set>
#include <vector>
#include <memory>
#include <iostream>

// ����ĳ����Ʊ����ֵ
double SumMarketValue(const std::string& id) {
    (void)id;
    return 100.0 * 1000 * 1000;
}

// ����ĳ����Ʒ����ֵ
double SumProductMarketValue(uint64_t acc) {
    (void)acc;
    return 1.0 * 1000 * 1000 * 1000;
}

struct Order
{
    char instrumentId[20];  //��Ʊ���룬��: 111111.SH, 222222.SH
    uint64_t accountId; //�����˻�����Ӧĳ����Ʒ��
    double price;       //�����۸�
    int32_t qty;        //��������
    float chargeBackRate;   //������
    // �����ֶ�...

    // �����Ƿ��Ǵ�ҵ���Ʊ
    bool isStartupStocks() const {
        static std::set<std::string> startup_stocks = { "StartupA", "StartupB" };
        return startup_stocks.count(instrumentId);
    }

    // �����Ƿ�����Ʒ�ڻ�
    bool isCommodityFutures() const {
        return false;
    }
};

class Strategy {
public:
    virtual ~Strategy() = default;
    virtual bool match(const Order& order) = 0;
    virtual bool enable(const Order& order) = 0;
};

class ProductAStrategy : public Strategy {
public:
    bool match(const Order& order) override {
        return order.instrumentId == "ProductAID";
    }

    bool enable(const Order& order) override {
        if (order.isStartupStocks()) {
            if (order.price * order.qty / SumMarketValue(order.instrumentId) > 0.3) {
                return false;
            }
        } else {
            if (order.price * order.qty / SumProductMarketValue(order.accountId) > 0.05) {
                return false;
            }
        }
        return true;
    }
};

class ProductBStrategy : public Strategy {
public:
    bool match(const Order& order) override {
        return order.instrumentId == "ProductBID";
    }

    bool enable(const Order& order) override {
        if (order.isStartupStocks()) return false;
        if (order.chargeBackRate > 0.5) return false;
        return true;
    }
};

class ProductCStrategy : public Strategy {
public:
    bool match(const Order& order) override {
        return order.instrumentId == "ProductCID";
    }

    bool enable(const Order& order) override {
        if (order.price * order.qty / SumProductMarketValue(order.accountId) > 0.1) {
            return false;
        }
        if (order.isCommodityFutures()) {
            return false;
        }
        return true;
    }
};

// �����µĲ�Ʒֻ��Ҫʵ��һ���µ�Product Strategy

// �����µķ������Ҳ��������һ����ͬmatch���Ե�Strategy
class InstrumentStategy : public Strategy {
    std::string id_;
public:
    InstrumentStategy(std::string id) : id_(std::move(id)) {}

    bool match(const Order& order) override {
        return !strcmp(order.instrumentId, id_.c_str());
    }

    // disable all orders of instrument
    bool enable(const Order& order) override {
        return false;
    }
};


class Risk {
private:
    std::vector<std::unique_ptr<Strategy>> strategies;
public:

    void addStrategy(std::unique_ptr<Strategy> s) {
        strategies.push_back(std::move(s));
    }

    bool check(const Order& order) {
        for (auto& s : strategies) {
            if (s->match(order) && !s->enable(order)) {
                return false;
            }
        }
        return true;
    }
};


int main() {
    Risk risk;
    risk.addStrategy(std::make_unique<ProductAStrategy>());
    risk.addStrategy(std::make_unique<ProductBStrategy>());
    risk.addStrategy(std::make_unique<ProductCStrategy>());
    risk.addStrategy(std::make_unique<InstrumentStategy>("123456.SH"));

    Order order;
    strcpy_s(order.instrumentId, "ProductAID");
    order.accountId = 123;
    order.price = 10;
    order.qty = 100;

    std::cout << risk.check(order) << std::endl;

    Order order2;
    strcpy_s(order2.instrumentId, "123456.SH");

    std::cout << risk.check(order2) << std::endl;

    return 0;
}
