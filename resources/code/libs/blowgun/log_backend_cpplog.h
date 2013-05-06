#ifndef BLOWGUN_LOG_BACKEND_CPPLOG_H
#define BLOWGUN_LOG_BACKEND_CPPLOG_H

#include "log.h"

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
	void LogImpl(LogLevel level, std::string message);
};

}

#endif // BLOWGUN_LOG_BACKEND_CPPLOG_H