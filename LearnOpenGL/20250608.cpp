//rall lock
#include <mutex>
#include <iostream>
#include <functional>
#include <cstring>
#include <future>
#include <queue>
#include <coroutine>
#include <boost/context/continuation.hpp>

using namespace std;
using namespace boost::context;

std::mutex _mutex;

void ThreadFunc() {
	auto func1 = []()
		{
			for (int i = 0; i < 100; i++) {
				std::cout << "one:" << i << " ";
			}
		};

	std::thread t(func1);
	if (t.joinable()) {
		//�̷߳��룬��̨����ִ��
		t.detach();
	}
	auto func2 = []() {
		for (int i = 0; i < 100; i++) {
			std::cout << "two:" << i << " ";
		}
		};
	std::thread t1(func2);
	if (t1.joinable()) {
		t1.join();//�ȴ��߳�ִ�����
	}
}

void MemoryFunc() {
	char src1[] = "hello world";
	char des1[20];
	memcpy(des1, src1, sizeof(src1));
	std::cout << "memcpy option " << des1 << std::endl;

	char src2[] = "overlap example";
	memmove(src2 + 5, src2, 8);
	std::cout << "memmove " << src2 << std::endl;
}

void BindFunc(int a, int b) {
	std::cout << "sum:" << a + b << std::endl;
}

enum Color {
	red,
	green,
	blue
};

enum class ColorClass {
	red,
	green,
	blue
};

bool IsLittleEndian() {
	uint16_t num = 1;
	return *(reinterpret_cast<char*>(&num)) == 1;
}



class MyClass {
public:
	MyClass(const char* inputData) {
		size_t len = std::strlen(inputData) + 1;
		data = new char[len];
		strcpy_s(data, len, inputData);
	};
	//������캯��
	MyClass(const MyClass& ohter) {
		size_t len = std::strlen(ohter.data) + 1;
		data = new char[len];
		strcpy_s(data, len, ohter.data);
	}
	~MyClass() {
		delete[] data;
	}
	void printData() {
		std::cout << data << std::endl;
	}
	void setData(const char* setData) {
		delete[] data;  // �ͷž�����
		size_t len = std::strlen(setData) + 1;
		data = new char[len];
		strcpy_s(data, len, setData);
	}
private:
	char* data;
};


int async_function() {
	return 42;
}

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void fun1() {
	std::unique_lock<std::mutex> lock(mtx);
	cv.wait(lock, [] {return ready; });//�ȴ���������
	std::cout << "fun1����ִ��" << std::endl;
}
void fun2() {
	std::lock_guard<std::mutex> lock(mtx);
	ready = true;
	cv.notify_one();//�������һ���̸߳ɻcv.notify_all();�������еȴ��̸߳ɻ�
}


//�̳߳ص�ʵ��
class ThreadPool {
public:
	ThreadPool(int threadCount) {
		for (int i = 0; i < threadCount; ++i) {
			threads.emplace_back(&ThreadPool::worker, this);
		}
	}
	~ThreadPool() {
		stop.store(true);
		condition.notify_all();
		for (auto& tmpthread : threads) {
			if (tmpthread.joinable()) {
				tmpthread.join();
			}
		}
	}
	/// <summary>
	/// �ύ����
	/// </summary>
	/// <param name="task"></param>
	void enqueue(std::function<void()> task) {
		std::unique_lock<std::mutex> lock(queueMutex);
		tasks.push(std::move(task));
		condition.notify_one();
	}
private:
	//�����̺߳���
	void worker()
	{
		while (true)
		{
			std::function<void()> task;
			{
				std::unique_lock<std::mutex> lock(queueMutex);
				condition.wait(lock, [this]() {return stop.load() || !tasks.empty(); });
				if (stop.load() && tasks.empty()) {
					return;
				}
				task = std::move(tasks.front());
				tasks.pop();
			}
			task();
		}
	}
	/// <summary>
	/// �̹߳���
	/// </summary>
	std::vector<std::thread> threads;
	std::queue<std::function<void()>> tasks;
	/// <summary>
	/// ������л�����
	/// </summary>
	std::mutex queueMutex;
	/// <summary>
	/// ����֪ͨ
	/// </summary>
	std::condition_variable condition;
	/// <summary>
	/// ֹͣ��־
	/// </summary>
	std::atomic<bool> stop;
};

struct MyTask {
	struct promise_type {
		std::coroutine_handle<promise_type> handle;
		MyTask get_return_object() {
			handle = std::coroutine_handle<promise_type>::from_promise(*this);
			return MyTask{ handle };
		}
		//����������ִ��
		std::suspend_never initial_suspend() {
			return {};
		}
		/// <summary>
		/// ����������ִ�У������쳣
		/// </summary>
		/// <returns></returns>
		std::suspend_always final_suspend() noexcept{
			return {};
		}
		void return_void(){}
		void unhandled_exception(){}
	};
	std::coroutine_handle<promise_type> handle;
	MyTask(std::coroutine_handle<promise_type> h) :handle(h) {

	}
	~MyTask() {
		if (handle)
			handle.destroy();
	}
	void resume() {
		if (!handle.done()) {
			handle.resume();
		}
	}
};
MyTask myCoroutine() {
	std::cout << "hello" << std::endl;
	co_await std::suspend_always{};
	std::cout << "world" << std::endl;
}

//��ջЭ��
continuation coro_func(continuation &&c) {
	std::cout << "start" << std::endl;
	//����ȴ��ָ�
	c = c.resume();
	std::cout << "coroutine resumed" << std::endl;
	//�ٴι���
	c = c.resume();
	std::cout << "coroutine finished" << std::endl;
	//���ؿ���Ȩ
	return std::move(c);
}


int main() {
	//ʹ��boostЭ��
	continuation coro = callcc(coro_func);
	std::cout << "back in main" << std::endl;
	coro = coro.resume();
	std::cout << "back in main" << std::endl;
	coro = coro.resume();
	std::cout << "back in main" << std::endl;
	////Э��
	//auto t = myCoroutine();
	//std::cout << "Э����ͣ��" << std::endl;
	//t.resume();
	////�̳߳�
	//ThreadPool pool(5);
	//for (int i = 0; i < 10; ++i) {
	//	pool.enqueue([i]() {
	//		std::cout << "task " << i << " is running in thread " << std::this_thread::get_id() << std::endl;
	//		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	//		});
	//}


	/*fun2();
	fun1();*/

	//������������


	//std::future<int> res = std::async(std::launch::async, async_function);
	//std::cout << res.get() << std::endl;

	//string �� sso
	//std::string str1 = "hello world";
	//std::string str2 = "hello world hello world hello world hello world hello world hello world hello world";

	//std::cout << "address of short str:" << static_cast<const void*>(str1.data()) << std::endl;
	//std::cout << "address of long str:" << static_cast<const void*>(str2.data()) << std::endl;
	//�������캯��
	//MyClass obj1("Hello");
	//MyClass obj2 = obj1;
	//obj2.printData();
	//obj1.printData();

	//��ˣ�С��ת��
	//uint32_t num = 0x12345678;
	//uint32_t n_num = htonl(num);
	//uint32_t h_num = ntohl(n_num);

	//std::cout << "Original: " << std::hex << num << std::endl;
	//std::cout << "Network Byte Order: " << std::hex << n_num << std::endl;
	//std::cout << "Converted Back: " << std::hex << h_num << std::endl;

	//if (IsLittleEndian()) {
	//	std::cout << "system is little endian" << std::endl;
	//}
	//else {
	//	std::cout << "system is big endian" << std::endl;
	//}
	//auto bindFunc = std::bind(BindFunc,std::placeholders::_1,std::placeholders::_2);
	//bindFunc(1,2);
	/*MemoryFunc();*/
	/*int c = Color::red;
	ColorClass cc = ColorClass::red;
	ThreadFunc();*/
	//auto fun1 = [](int k) {
	//	std::unique_lock<std::mutex> lock(_mutex);
	//	for (int i = 0; i < k; ++i) {
	//		cout << i <<" ";
	//	}
	//	cout << endl;
	//};
	//std::thread threads[5];
	//for (int i = 0; i < 5;++i) {
	//	threads[i] = thread(fun1,200);
	//}
	//for (auto &th:threads) {
	//	th.join();
	//}

	return 0;
}