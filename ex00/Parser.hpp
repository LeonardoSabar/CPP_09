#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>

#include "BitcoinExchange.hpp"

class BitcoinExchange;

class Parser {
	public:
		static void			parseFile(const std::string &filename, BitcoinExchange &exchange);
		static std::string	validateDate(const std::string &dateStr);
		static float		atof(const std::string &rateStr);
		static int			atoi(const std::string &str);

		static bool			validateHeaderInputFile(std::ifstream &file);

	private:
		Parser();
		Parser(const Parser &other);
		Parser &operator=(const Parser &other);
		~Parser();
};

#endif
