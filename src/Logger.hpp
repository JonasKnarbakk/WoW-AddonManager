#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

class Logger {
	public:
		// Log levels. Everything up to
		// and including the highest log level
		// will be logged.
		enum logLevel{
			SEVERE,
			WARNING,
			INFO,
			FINE,
			FINER,
			FINEST,
		};

		Logger();
		~Logger();

		void log(std::string text, Logger::logLevel level);
	private:
		Logger::logLevel level = Logger::logLevel::INFO;

		void logToOutput(std::string);
		void logToFile(std::string);
};

#endif
