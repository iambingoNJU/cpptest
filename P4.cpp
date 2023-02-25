
#include <mutex>

struct MarketData {
    int data;
};

struct Order {
    int id;
};

struct OrderRtn {
    int id;
};

class Strategy {
    std::mutex m_Lock;
    bool m_orderInternal = false;

public:
	Strategy(bool internal = false): m_orderInternal(internal) {}

	// 收到行情
	void onMarketData(const MarketData& md)
	{
		std::unique_lock<std::mutex> lock(m_Lock);
		Order order;
		// 获取策略内部数据，并结合行情做一些计算
		// .......
		if (m_orderInternal)
		{
			sendOrderInternal(order);
		}
		else
		{
			sendOrderExternal(order);
		}
	}

	// 回报，反馈订单的状态，如成交或撤单
	void onRtnOrder(const OrderRtn& rtn)
	{
		std::unique_lock<std::mutex> lock(m_Lock);
		// 仓位更新，log打印，策略状态维护等
		// ....
	}

private:
	// 内部报单
	void sendOrderInternal(const Order& order)
	{
		// 报到内部程序
		// ....

		/// 认为订单已成交，请完成回报触发逻辑。

		// translate order to rtnOrder
		OrderRtn rtnOrder;
		rtnOrder.id = order.id;

		// 防止多次lock同一把锁，最简单的改动是用recursive_lock, 但是需要改动lock的位置
		// 因此先unlock，然后在onRtnOrder()里面可以重新上锁，最后再lock
		m_Lock.unlock();
		onRtnOrder(rtnOrder);
		m_Lock.lock();
	}

	// 外部报单
	void sendOrderExternal(const Order& order)
	{
		// 报到外部程序
		// ....
	}
};

int main() {
	Strategy s(true);

	MarketData data;
	s.onMarketData(data);

	return 0;
}
