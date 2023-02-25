
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

	// �յ�����
	void onMarketData(const MarketData& md)
	{
		std::unique_lock<std::mutex> lock(m_Lock);
		Order order;
		// ��ȡ�����ڲ����ݣ������������һЩ����
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

	// �ر�������������״̬����ɽ��򳷵�
	void onRtnOrder(const OrderRtn& rtn)
	{
		std::unique_lock<std::mutex> lock(m_Lock);
		// ��λ���£�log��ӡ������״̬ά����
		// ....
	}

private:
	// �ڲ�����
	void sendOrderInternal(const Order& order)
	{
		// �����ڲ�����
		// ....

		/// ��Ϊ�����ѳɽ�������ɻر������߼���

		// translate order to rtnOrder
		OrderRtn rtnOrder;
		rtnOrder.id = order.id;

		// ��ֹ���lockͬһ��������򵥵ĸĶ�����recursive_lock, ������Ҫ�Ķ�lock��λ��
		// �����unlock��Ȼ����onRtnOrder()����������������������lock
		m_Lock.unlock();
		onRtnOrder(rtnOrder);
		m_Lock.lock();
	}

	// �ⲿ����
	void sendOrderExternal(const Order& order)
	{
		// �����ⲿ����
		// ....
	}
};

int main() {
	Strategy s(true);

	MarketData data;
	s.onMarketData(data);

	return 0;
}
