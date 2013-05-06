#include "log_backend_cpplog.h"
#include <iostream>

using namespace blowgun;
using namespace cpplog;

void LogBackendCpplog::LogImpl(LogLevel/* level*/, std::string message)
{
	static StdErrLogger logger;
	LOG_DEBUG(logger) << message;
}