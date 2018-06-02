#include <Logger.hpp>
#include <iostream>
#include <sstream>
#include <fstream>

Logger::Logger() {

}

Logger::~Logger() {

}

void Logger::log(std::string text, Logger::logLevel level) {
	std::stringstream output;
	// Prepend time and loglevel to the output
	// TODO

	// Append output text and log to both stdout and file
	output << text;
	if (level <= this->level) {
		logToOutput(output.str());
		logToFile(output.str());
	}
}

void Logger::logToOutput(std::string text) {
	std::stringstream output;
	output << text;
	std::cout << output.str() << std::endl;
}

void Logger::logToFile(std::string text) {
	std::ofstream logfile("logfile.txt", std::ofstream::out);
	if(logfile.good()) {
		logfile << text;
		logfile.close();
	} else {
		logToOutput("Failed to open logfile for writing");
	}
}
