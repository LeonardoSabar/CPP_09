#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>
#include <stdexcept>
#include <iostream>
#include <iomanip>

#include "Parser.hpp"

class BitcoinExchange {
public:
	BitcoinExchange();
	BitcoinExchange(const BitcoinExchange &other);
	BitcoinExchange &operator=(const BitcoinExchange &other);
	~BitcoinExchange();

	std::map<std::string, float>& getRates();

	void	makeExchange(const std::string &date) const;

	private:
	std::map<std::string, float> rates;
};

#endif
