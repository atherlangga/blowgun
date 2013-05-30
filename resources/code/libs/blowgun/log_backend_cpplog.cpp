#include "log_backend_cpplog.h"
#include <iostream>

using namespace blowgun;
using namespace cpplog;

static int TranslateLogLevel(LogLevel level);
static std::vector<std::string> SplitIntoLines(std::string message);

LogBackendCpplog::LogBackendCpplog(
	std::shared_ptr<cpplog::BaseLogger> logger)
: logger_(logger)
{
}

void
LogBackendCpplog::LogImpl(LogLevel level, std::string fileName,
	unsigned int lineNumber, std::string message)
{
	int cpplogLevel = TranslateLogLevel(level);

	for (auto m : SplitIntoLines(message))
	{
		cpplog::LogMessage(fileName.c_str(), lineNumber,
			cpplogLevel, logger_.get()).getStream() << message << std::endl;
	}
}

static int TranslateLogLevel(LogLevel level)
{
	int result = LL_INFO;
	switch (level)
	{
		case LogLevel::INFO:
			result = LL_INFO;
			break;
		case LogLevel::DEBUG:
			result = LL_DEBUG;
			break;
		case LogLevel::WARN:
			result = LL_WARN;
			break;
		case LogLevel::ERROR:
			result = LL_ERROR;
			break;
		case LogLevel::FATAL:
			result = LL_FATAL;
			break;
		default:
			break;
	}
	return result;
}

static std::vector<std::string> SplitIntoLines(std::string message)
{
	std::stringstream stream(message);
	std::vector<std::string> result;

	while(stream.good())
	{
		std::string line;
		std::getline(stream, line);
		result.push_back(line);
	}

	return result;
}
