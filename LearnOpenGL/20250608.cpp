//rall lock
#include <mutex>
#include <iostream>
#include <functional>
#include <cstring>
#include <future>

using namespace std;
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
		//线程分离，后台进行执行
		t.detach();
	}
	auto func2 = []() {
		for (int i = 0; i < 100; i++) {
			std::cout << "two:" << i << " ";
		}
		};
	std::thread t1(func2);
	if (t1.joinable()) {
		t1.join();//等待线程执行完成
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
	MyClass(const char * inputData) {
		size_t len = std::strlen(inputData) + 1;
		data = new char[len];
		strcpy_s(data, len, inputData);
	};
	//深拷贝构造函数
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
	void setData(const char *setData) {
		delete[] data;  // 释放旧数据
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

int main() {
	std::future<int> res = std::async(std::launch::async, async_function);
	std::cout << res.get() << std::endl;

	//string 的 sso
	//std::string str1 = "hello world";
	//std::string str2 = "hello world hello world hello world hello world hello world hello world hello world";

	//std::cout << "address of short str:" << static_cast<const void*>(str1.data()) << std::endl;
	//std::cout << "address of long str:" << static_cast<const void*>(str2.data()) << std::endl;
	//拷贝构造函数
	//MyClass obj1("Hello");
	//MyClass obj2 = obj1;
	//obj2.printData();
	//obj1.printData();

	//大端，小端转换
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