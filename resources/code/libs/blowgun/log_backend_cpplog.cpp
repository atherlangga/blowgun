#include "log_backend_cpplog.h"
#include <iostream>

using namespace blowgun;
using namespace cpplog;

static int TranslateLogLevel(LogLevel level);

LogBackendCpplog::LogBackendCpplog(std::shared_ptr<cpplog::BaseLogger> logger)
: logger_(logger)
{
}

void LogBackendCpplog::LogImpl(LogLevel level, std::string message)
{
	int cpplogLevel = TranslateLogLevel(level);
	LOG_LEVEL(cpplogLevel, logger_.get()) << message;
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
