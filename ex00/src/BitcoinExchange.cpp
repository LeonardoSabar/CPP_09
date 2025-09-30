#include "../include/BitcoinExchange.hpp"
#include <ctime>

BitcoinExchange::BitcoinExchange(){
	loadData("data.csv");
}

BitcoinExchange::~BitcoinExchange(){}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other){
	this->_data = other._data;
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other){
	if (this != &other)
		this->_data = other._data;
	return *this;
}

bool BitcoinExchange::loadData(const std::string &filename){
	std::ifstream file(filename.c_str());
	if (!file.is_open()){
		std::cerr << "Error: opening the file" << std::endl;
		return false;
	}
	std::string line;
	std::getline(file, line);
	while (std::getline(file, line))
	{
		line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
		size_t pos = line.find(',');
		if (pos == std::string::npos)
			throw std::runtime_error("Invalid data format in file");
		if (line.substr(0, pos).empty() || line.substr(pos + 1).empty())
			throw std::runtime_error("Empty date or rate in file");
		std::string date = line.substr(0, pos);
		std::string rateStr = line.substr(pos + 1);
		float rate = std::atof(rateStr.c_str());
		this->_data[date] = rate;
	}
	return true;
}

float BitcoinExchange::getRate(const std::string date){
	std::map<std::string, float>::iterator it = this->_data.lower_bound(date);
	if (it == this->_data.begin())
		throw std::runtime_error("Error: Date is too early.");
	--it;
	return it->second;
}

void readInputFile(const char *file, BitcoinExchange exchange){
	std::ifstream inputFile(file);
	if (!inputFile.is_open())
		throw std::runtime_error("Error: File is not opening.");
	if (inputFile.peek() == EOF)
		throw std::runtime_error("Error: Empty file.");
	std::string inputLine;
	std::getline(inputFile, inputLine);
	if (inputLine != "date | value")
		throw std::runtime_error("Error: File in wrong format.");
	while (std::getline(inputFile, inputLine))
		checkInputFile(inputLine, exchange);
}

bool checkInputFile(std::string inputLine, BitcoinExchange exchange){
	if (inputLine.empty() || checkIsOnlySpace(inputLine)){
		std::cerr << "Error: bad input => empty line" << inputLine << std::endl;
		return false;
	}
	size_t pipe = inputLine.find('|');
	if (pipe == inputLine.npos){
		std::cerr << "Error: bad input => " << inputLine << std::endl;
		return false;
	}
	std::string date = trim(inputLine.substr(0, pipe));
	std::string value = trim(inputLine.substr(pipe + 1));
	if (date.empty() || value.empty()){
		std::cerr << "Error: bad input => " << inputLine << std::endl;
		return false;
	}
	if (dataFormat(date) == false){
		std::cerr << "Error: bad input => " << inputLine << std::endl;
		return false;
	}
	if (!check_value(value))
		return false;
	printFormat(date, std::atof(value.c_str()), exchange);
	return true;
}

bool dataFormat(std::string &date){
	if (date.size() < 10)
		return false;
	for (size_t i = 0; i < date.size(); i++){
		if (!std::isdigit(date[i]) && date[i] != '-')
			return false;
	}
	if (date.size() != 10 || date[4] != '-' || date[7] != '-')
		return false;
	int year = std::atoi(date.substr(0,4).c_str());
	int month = std::atoi(date.substr(5,2).c_str());
	int day = std::atoi(date.substr(8,2).c_str());
	if (!check_date(year, month, day))
		return false;
	return true;
}

bool check_date(int year, int month, int day){
	int currentYear = getCurrentYear();
	int daysInMonth[] = {31, (leapYear(year) ? 29 : 28), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	if (month < 1 || month > 12)
		return false;
	if (day < 1 || day > daysInMonth[month - 1])
		return false;
	if (year > currentYear)
		return false;
	return true;
}

bool check_value(std::string value){
	if (value.empty()){
		std::cerr << "Error: not a positive number." << std::endl;
		return false;
	}
	for (size_t i = 0; i < value.size(); i++){
		if (!std::isdigit(value[i]) && value[i] != '.'){
			std::cerr << "Error: not a positive number." << std::endl;
			return false;
		}
	}
	float floatValue = std::atof(value.c_str());
	if (floatValue < 0){
		std::cerr << "Error: not a positive number." << std::endl;
		return false;
	}
	if (floatValue > 1000){
		std::cerr << "Error: too large a number." << std::endl;
		return false;
	}
	if (value == "."){
		std::cerr << "Error: not a positive number." << std::endl;
		return false;
	}
	if (std::count(value.begin(), value.end(), '.') > 1){
		std::cerr << "Error: too many decimal points." << std::endl;
		return false;
	}
	return true;
}

bool checkIsOnlySpace(std::string line){
	for (size_t i = 0; i < line.size(); i++){
		if (!std::isspace(line[i]))
			return false;
	}
	return true;
}

bool leapYear(int year){
	if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
		return true;
	return false;
}

int getCurrentYear(){
	time_t now = time(0);
	int currentYear = 1900 + localtime(&now)->tm_year;
	return currentYear;
}

void printFormat(std::string date, float value, BitcoinExchange exchange){
	std::cout << date << " => " << value << " = " << exchange.getRate(date) * value << std::endl;
}

std::string trim(const std::string &str){
	if (str.empty())
		return str;
	size_t start = str.find_first_not_of(" \t");
	if (start == std::string::npos)
		return "";
	size_t end = str.find_last_not_of(" \t");
	return (str.substr(start, end - start + 1));
}