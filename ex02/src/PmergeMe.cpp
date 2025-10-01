#include "../include/PmergeMe.hpp"

Pmerge::Pmerge() : _vTime(0.0), _dTime(0.0){}

Pmerge::~Pmerge(){}

Pmerge::Pmerge(const Pmerge &other) : _vec(other._vec), _deq(other._deq), _vTime(other._vTime), _dTime(other._dTime) {
	this->_vec = other._vec;
	this->_deq =  other._deq;
}

Pmerge &Pmerge::operator=(const Pmerge &other){
	if (this != &other){
		this->_vec = other._vec;
		this->_deq = other._deq;
	}
	return *this;
}

void Pmerge::run(int argc, char **argv){
	if (!checkArgs(argc, argv)){
		std::cerr << "Error" << std::endl;
		return ;
	}
	for (int i = 1; i < argc; i++){
		long value = std::atol(argv[i]);
		if (value > INT_MAX){
			std::cerr << "Error" << std::endl;
			return ;
		}
		_vec.push_back(value);
		_deq.push_back(value);
	}
	printVec(0, this->_vec);
	fordJohnsonVec();
	printVec(1, this->_vec);
	printTimeVec();
	printDec(0, this->_deq);
	fordJohnsonDeq();
	printDec(1, this->_deq);
	printTimeDec();
}

bool Pmerge::checkArgs(int argc, char **argv){
	for (int i = 1; i < argc; i++){
		if (argv[i][0] == '\0')
			return false;
		std::string str(argv[i]);
		for (size_t j = 0; j < str.size(); j++){
			if (!std::isdigit(str[j]))
				return false;
		}
	}
	return true;
}

std::vector<size_t> Pmerge::generateJacobstallSequence(size_t len){
	std::vector<size_t> _order;
	size_t j0 = 1;
	size_t j1 = 1;

	while(j1 < len){
		_order.push_back(j1);
		size_t next = j1 + 2 * j0;
		j0 = j1;
		j1 = next;
	}
	return _order;
}

double Pmerge::calc_time(clock_t start, clock_t end){
	double time = static_cast<double>(end - start) / CLOCKS_PER_SEC * 100000;
	return time;
}
