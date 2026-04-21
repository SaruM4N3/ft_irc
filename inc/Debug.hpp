/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Debug.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsonie <zsonie@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 00:40:24 by zsonie            #+#    #+#             */
/*   Updated: 2026/04/21 18:37:08 by zsonie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <ctime>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>

#ifndef DEBUG_MODE
#define DEBUG_MODE 0
#endif

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"

enum LogLevel { LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR };

class Logger {
   public:
	static Logger& getInstance() {
		static Logger instance;
		return instance;
	}

	void log(LogLevel level, const std::string& msg) {
		if (level == LOG_DEBUG && !DEBUG_MODE) return;

		std::string ts = timestamp();
		std::string entry = "[" + ts + "] " + prefix(level) + msg;

		std::cout << color(level) << entry << RESET << std::endl;
		if (_file.is_open()) _file << entry << std::endl;
	}

   private:
	std::ofstream _file;

	Logger() { _file.open("debug.log", std::ios::out | std::ios::trunc); }
	~Logger() {
		if (_file.is_open()) _file.close();
	}
	Logger(const Logger&);
	Logger& operator=(const Logger&);

	std::string timestamp() const {
		time_t now = time(NULL);
		char buf[20];
		strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
		return std::string(buf);
	}

	std::string prefix(LogLevel level) const {
		switch (level) {
			case LOG_DEBUG:
				return "[DEBUG]   :";
			case LOG_INFO:
				return "[INFO]    :";
			case LOG_WARNING:
				return "[WARNING] :";
			case LOG_ERROR:
				return "[ERROR]   :";
			default:
				return "[UNKNOWN] :";
		}
	}

	const char* color(LogLevel level) const {
		switch (level) {
			case LOG_DEBUG:
				return CYAN;
			case LOG_INFO:
				return GREEN;
			case LOG_WARNING:
				return YELLOW;
			case LOG_ERROR:
				return RED;
			default:
				return RESET;
		}
	}
};

template <typename T>
inline std::string toString(T val) {
	std::ostringstream oss;
	oss << val;
	return oss.str();
}

#define LOG_D(msg) Logger::getInstance().log(LOG_DEBUG, msg)
#define LOG_I(msg) Logger::getInstance().log(LOG_INFO, msg)
#define LOG_W(msg) Logger::getInstance().log(LOG_WARNING, msg)
#define LOG_E(msg) Logger::getInstance().log(LOG_ERROR, msg)

#endif
