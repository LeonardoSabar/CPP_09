
#include "../include/BitcoinExchange.hpp"

//1 - valores data.csv inseridos, com verificação e remoção de espaços
//2 - Recebe arquivo no input
int main(int argc, char **argv){
	if (argc != 2){
		std::cout << "Error: You must pass the File.";
		return 1;
	} 
	try {
		BitcoinExchange exchange;
		readInputFile(argv[1], exchange);
	} catch (std::runtime_error &e) {
		std::cerr << RED << e.what() << RESET << std::endl;
		return 1;
	}

	return 0;
}
