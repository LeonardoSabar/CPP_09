#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) : rates(other.rates) {}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other) {
	if (this != &other) {
		rates = other.rates;
	}
	return (*this);
}

BitcoinExchange::~BitcoinExchange() {}

std::map<std::string, float>& BitcoinExchange::getRates() {
	return (rates);
}

void BitcoinExchange::makeExchange(const std::string &date) const {
	size_t	pipe_pos = date.find('|');

	std::string	dateStr	= Parser::validateDate(date.substr(0, pipe_pos));
	std::string	rateStr	= date.substr(pipe_pos + 1);
	float		rate	= Parser::atof(rateStr);

	if (rate > 1000.0f)
		throw std::invalid_argument("too large a number " + rateStr);

	if (rates.empty())
		throw std::runtime_error("No rates available for exchange.");

	std::map<std::string, float>::const_iterator it;

	it = rates.find(dateStr);
	if (it != rates.end()) {
		float result = rate * it->second;
		std::cout << dateStr << " => " << std::fixed << std::setprecision(2) << rate << " = " << result << std::endl;
		return;
	}

	it = rates.upper_bound(dateStr);

	if (it == rates.begin())
		throw std::runtime_error("No available rate for date: " + dateStr);

	it--;
	float result = rate * it->second;
	std::cout << dateStr << " => " << rate << " = " << std::fixed << std::setprecision(2) << result << std::endl;
}
