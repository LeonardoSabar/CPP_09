#include "../include/Parser.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

Parser::Parser() {}

Parser::Parser(const Parser &other) {
	(void)other;
}

Parser &Parser::operator=(const Parser &other) {
	(void)other;
	return *this;
}

Parser::~Parser() {}

void Parser::parseFile(const std::string &filename, BitcoinExchange &exchange) {
	std::ifstream file(filename.c_str());
	if (!file.is_open() || file.fail())
		throw std::runtime_error("Could not open file");

	std::string line;
	std::getline(file, line);
	if (line != "date,exchange_rate")
		throw std::runtime_error("Invalid file format: expected header 'date,exchange_rate'");

	std::map<std::string, float> &rates = exchange.getRates();

	while (std::getline(file, line)) {
		size_t commaPos = line.find(',');
		if (commaPos != std::string::npos) {
			std::string	date = Parser::validateDate(line.substr(0, commaPos));
			float		rate = Parser::atof(line.substr(commaPos + 1));
			rates[date] = rate;
		}
	}
}

std::string Parser::validateDate(const std::string &dateStr) {
	size_t first = dateStr.find('-');
	size_t second = dateStr.find('-', first + 1);

	std::string yearStr = dateStr.substr(0, first);
	std::string monthStr = dateStr.substr(first + 1, second - first - 1);
	std::string dayStr = dateStr.substr(second + 1);

	int year = Parser::atoi(yearStr);
	int month = Parser::atoi(monthStr);
	int day = Parser::atoi(dayStr);

	if (year < 0 || month < 1 || month > 12 || day < 1 || day > 31) {
		throw std::invalid_argument("Invalid date format: " + dateStr);
	}

	if (month == 2) {
		if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
			if (day > 29)
				throw std::invalid_argument("invalid date format: " + dateStr);
		} else {
			if (day > 28)
				throw std::invalid_argument("invalid date format: " + dateStr);
		}
	} else if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
		throw std::invalid_argument("Invalid date format: " + dateStr);
	}

	return (dateStr);
}

int Parser::atoi(const std::string &str) {
	int value;
	std::istringstream iss(str);

	iss >> value;

	if (iss.fail())
		throw std::invalid_argument("bad input - " + str);
	return (value);
}

float Parser::atof(const std::string &rateStr) {
	float rate;
	std::istringstream iss(rateStr);

	iss >> rate;

	if (iss.fail())
		throw std::invalid_argument("bad input " + rateStr);

	if (rate < 0)
		throw std::invalid_argument("not a positive number " + rateStr);

	return (rate);
}

bool Parser::validateHeaderInputFile(std::ifstream &file) {
	std::string header;
	std::getline(file, header);
	return (header == "date | value");
}
