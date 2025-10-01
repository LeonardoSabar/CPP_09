#include "BitcoinExchange.hpp"
#include "Parser.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "Error: could not open file." << std::endl;
		return (1);
	}

	BitcoinExchange exchange;

	try {
		Parser::parseFile("data.csv", exchange);
	} catch (const std::exception &e) {
		std::cerr << "Error: data.csv - " << e.what() << std::endl;
		return (1);
	}

	std::ifstream inputFile(argv[1]);
	if (!inputFile.is_open() || inputFile.fail() || !Parser::validateHeaderInputFile(inputFile)) {
		std::cerr << "Error: could not open file." << std::endl;
		return (1);
	}

	int lineCount = 2;
	std::string line;
	while(std::getline(inputFile, line)) {
		try {
			exchange.makeExchange(line);
		} catch(std::exception& e) {
			std::cerr << "Error line " << lineCount << " - " << line << " => " << e.what() << std::endl;
		}
		lineCount++;
	}

	return (0);
}
