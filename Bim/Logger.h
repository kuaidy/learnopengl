#pragma once
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

class Logger {
public:
	Logger() = default;
	void WriteInfo(const std::string& message);
private:
};