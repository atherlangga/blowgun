#ifndef BLOWGUN_LOG_BACKEND_CPPLOG_H
#define BLOWGUN_LOG_BACKEND_CPPLOG_H

#include "log.h"

#ifdef _WIN32
#define NOGDI
#endif

#include <cpplog/cpplog.hpp>

namespace blowgun
{

/**
 * Logging functionality wrapper for `cpplog`
 * (https://github.com/andrew-d/cpplog)
 */
class LogBackendCpplog : public LogBackend
{
public:
	LogBackendCpplog(std::shared_ptr<cpplog::BaseLogger> logger);
	void LogImpl(LogLevel level, std::string fileName,
		unsigned int lineNumber, std::string message);

private:
	const std::shared_ptr<cpplog::BaseLogger> logger_;
};

}

#endif // BLOWGUN_LOG_BACKEND_CPPLOG_H
