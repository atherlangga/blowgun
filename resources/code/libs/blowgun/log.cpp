#include "log.h"

#include <cstdlib>

using namespace blowgun;

////////////////////////////////////////////////////////////////////////

void
blowgun::Log(LogLevel level, std::string message)
{
	blowgun::Log(level, "", 0, message);
}

void
blowgun::Log(LogLevel level, std::string fileName, unsigned int lineNumber, std::string message)
{
	LogProxy::Instance()->Log(level, fileName, lineNumber, message);
}

void
blowgun::SetLogBackend(std::shared_ptr<LogBackend> backend)
{
	LogProxy::Instance()->SetLogBackend(backend);
}

////////////////////////////////////////////////////////////////////////

std::unique_ptr<LogProxy> LogProxy::instance_ = nullptr;

LogProxy::LogProxy()
: backend_(std::make_shared<LogBackendNull>())
{
}

LogProxy*
LogProxy::Instance()
{
	///
	// It will be better if this method can be implemented using
	// `std::call_once`, but the combination of Clang 3.1 and
	// GNU libstdc++ 4.7 has bug: Clang cannot compile `chrono`.
	///

	if (instance_ == nullptr)
	{
		instance_.reset(new LogProxy());
	}
	return instance_.get();
}

void
LogProxy::SetLogBackend(std::shared_ptr<LogBackend> backend)
{
	backend_ = backend;
}

void
LogProxy::Log(LogLevel level, std::string fileName, unsigned int lineNumber, std::string message)
{
	backend_->LogImpl(level, fileName, lineNumber, message);
}
