#include "Logger.h"

void Logger::WriteInfo(const std::string& message) {
	std::time_t t = std::time(nullptr);
	struct tm localTime;
	if (localtime_s(&localTime, &t) == 0) {
		string filename = std::to_string(localTime.tm_year) + '-' + std::to_string(localTime.tm_mon) + '-' + std::to_string(localTime.tm_mday);
		std::ofstream logFile;
		logFile.open(filename, std::ios::app);
		logFile << message << std::endl;
	}
}