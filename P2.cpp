#include <cstdint>
#include <string>
#include <cstring>
#include <set>
#include <vector>
#include <memory>
#include <iostream>

// 返回某个股票总市值
double SumMarketValue(const std::string& id) {
    (void)id;
    return 100.0 * 1000 * 1000;
}

// 返回某个产品总市值
double SumProductMarketValue(uint64_t acc) {
    (void)acc;
    return 1.0 * 1000 * 1000 * 1000;
}

struct Order
{
    char instrumentId[20];  //股票代码，如: 111111.SH, 222222.SH
    uint64_t accountId; //报单账户（对应某个产品）
    double price;       //报单价格
    int32_t qty;        //报单数量
    float chargeBackRate;   //撤单率
    // 其他字段...

    // 返回是否是创业板股票
    bool isStartupStocks() const {
        static std::set<std::string> startup_stocks = { "StartupA", "StartupB" };
        return startup_stocks.count(instrumentId);
    }

    // 返回是否是商品期货
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

// 增加新的产品只需要实现一个新的Product Strategy

// 增加新的风控需求也可以新增一个不同match策略的Strategy
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
