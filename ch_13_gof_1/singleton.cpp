// Одиночка (Singleton)

#include <iostream>
#include <string>



void func()
{
    std::cout << "info" << std::endl;
}








void log_error(const char* msg)
{
    std::cout << "[error]" << msg << std::endl;
}

void log_warn(const char* msg)
{
    std::cout << "[warn]" << msg << std::endl;
}

void log_info(const char* msg)
{
    std::cout << "[info]" << msg << std::endl;
}







enum class Severity
{
    error,
    warn,
    info
};

Severity severity = Severity::warn;

void error(const char* msg)
{
    if (severity >= Severity::error)
        std::cout << "[error]" << msg << std::endl;
}

void warn(const char* msg)
{
    if (severity >= Severity::warn)
        std::cout << "[warn]" << msg << std::endl;
}

void info(const char* msg)
{
    if (severity >= Severity::info)
        std::cout << "[info]" << msg << std::endl;
}

class SimpleLogger
{

	enum class Severity
	{
		error,
		warn,
		info
	};

	Severity severity = Severity::warn;

public:
	void error(const char *msg)
	{
		if (severity >= Severity::error)
			std::cout << "[error]" << msg << std::endl;
	}

	void warn(const char *msg)
	{
		if (severity >= Severity::warn)
			std::cout << "[warn]" << msg << std::endl;
	}

	void info(const char *msg)
	{
		if (severity >= Severity::info)
			std::cout << "[info]" << msg << std::endl;
	}
};

SimpleLogger logger;

class Logger
{
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void set_severity()
    {
        //...
    }
    void set_file(const std::string& filename)
    {
        //...
    }
    
    void info(const std::string &message) {
        std::cerr << "\tinfo: " << message << std::endl;
    }

    void warn(const std::string &message) {
        std::cerr << "warning: " << message << std::endl;
    }
private:
    Logger() = default;

    Logger(const Logger& root) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&& root) = delete;
    Logger& operator=(Logger&&) = delete;    

    ~Logger() = default;
};



int main() {
    // Compile error 
    // error: 'constexpr Logger::Logger()' is private within this context
    // Logger localLogger;

    Logger::getInstance().info("started");
    Logger::getInstance().warn("program is empty");

    Logger& logger = Logger::getInstance();
    logger.info("more logs");
    logger.warn("warning!");

    // Compile error
    // error: 'Logger::~Logger()' is private within this context
    // (&logger)->~Logger();

    return 0;
}