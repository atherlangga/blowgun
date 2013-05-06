#ifndef BLOWGUN_LOG_H
#define BLOWGUN_LOG_H

#include <string>
#include <memory>

namespace blowgun
{

// Forward declaration for `Log`.
enum class LogLevel;

/**
 * Log a message with specified level.
 */
void Log(LogLevel level, std::string message);

// Forward declaration for `SetLogBackend`.
class LogBackend;

/**
 * Set backend and facility for logging subsystem.
 */
void SetLogBackend(std::shared_ptr<LogBackend> backend);

enum class LogLevel
{
	INFO,
	DEBUG,
	WARN,
	ERROR,
	FATAL
};

/**
 * Proxy object for Logging subsystem.
 *
 * Its responsibility is to direct all requests on logging
 * to `LogBackend`.
 */
class LogProxy
{
private:
	void Log(LogLevel level, std::string message);
	void SetLogBackend(std::shared_ptr<LogBackend> backend);
	
	explicit LogProxy();

	static LogProxy* Instance();

private:
	LogProxy(const LogProxy &); // = delete;
	LogProxy & operator=(const LogProxy &); // = delete;

private:
	static std::unique_ptr<LogProxy> instance_;

	std::shared_ptr<LogBackend> backend_;

friend void Log(LogLevel level, std::string message);
friend void SetLogBackend(std::shared_ptr<LogBackend> backend);
};

/**
 * Backend for Logging.
 */
class LogBackend
{
public:
	virtual void LogImpl(LogLevel level, std::string message) = 0;
	virtual ~LogBackend() {}
};

/**
 * Null Object pattern that implements LogBackend.
 */
class LogBackendNull : public LogBackend
{
public:
	void LogImpl(LogLevel /*level*/, std::string /*message*/) { };
};

}

#endif // BLOWGUN_LOG_H
