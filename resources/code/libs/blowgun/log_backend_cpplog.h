#ifndef BLOWGUN_LOG_BACKEND_CPPLOG_H
#define BLOWGUN_LOG_BACKEND_CPPLOG_H

#include "log.h"

#include "platform_id.h"
#include "types.h"

#if (PLATFORM_ID == PLATFORM_WINDOWS)
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
		u32 lineNumber, std::string message);

private:
	const std::shared_ptr<cpplog::BaseLogger> logger_;
};

}

#endif // BLOWGUN_LOG_BACKEND_CPPLOG_H
