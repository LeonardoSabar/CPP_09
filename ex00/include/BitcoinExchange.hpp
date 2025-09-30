#ifndef BitcoinExchange_HPP
#define BitcoinExchange_HPP


#include <iostream>
#include <fstream>
#include <map>
#include <cstdlib>
#include <algorithm>

# define GREEN	"\033[92m"
# define RED	"\033[91m"
# define YELLOW	"\033[93m"
# define BLUE	"\033[94m"
# define PURPLE	"\033[95m"
# define RESET	"\033[0m"

class BitcoinExchange{
	private:
		std::map<std::string, float> _data;
		bool loadData(const std::string &data);
	public:
		BitcoinExchange();
		~BitcoinExchange();
		BitcoinExchange(const BitcoinExchange &other);
		BitcoinExchange &operator=(const BitcoinExchange &other);

		float getRate(const std::string date);
};
void	readInputFile(const char *inputFile, BitcoinExchange exchange);
bool	checkInputFile(std::string inputLine, BitcoinExchange exchange);
bool	dataFormat(std::string &date);
bool	check_date(int year, int month, int day);
bool	check_value(std::string value);
bool	checkIsOnlySpace(std::string line);
bool	leapYear(int year);
int		getCurrentYear(void);
std::string trim(const std::string &str);
void	printFormat(std::string date, float value, BitcoinExchange exchange);


#endif
