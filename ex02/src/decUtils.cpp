#include "../include/PmergeMe.hpp"

void Pmerge::printDec(int after, std::deque<int> d){
	if (!after)
		std::cout << "Before: ";
	else
		std::cout << "After: ";
	if (d.size() < 15){
		for (std::deque<int>::iterator it = d.begin(); it != d.end(); ++it){
			std::cout << *it << " ";
		}
	}
	else{
		int count = 0;
		for (std::deque<int>::iterator it = d.begin(); it != d.end() && count < 4; ++it){
			std::cout << *it << " ";
			count++;
		}
		std::cout << " [...]";
	}
	std::cout << std::endl;
}

std::deque<int> &Pmerge::getDeq(void){
	return this->_deq;
}

void Pmerge::fordJohnsonDeq(){
	std::deque<int> big;
	std::deque<int> small;
	clock_t dec_start = clock();
	fordJohnsonDeqPairs(this->_deq ,big, small);
	fordJohnsonDeq2(big, small);
	this->_deq = big;
	clock_t dec_end = clock();
	this->_dTime = calc_time(dec_start, dec_end);
}

void Pmerge::fordJohnsonDeqPairs(const std::deque<int> &inputDeque, std::deque<int> &big, std::deque<int> &small){
	std::deque<std::pair<int, int> > _pair;
	for (size_t i = 0; i + 1 < inputDeque.size(); i += 2){
		int a = inputDeque[i];
		int b = inputDeque[i + 1];
		if (a < b)
			_pair.push_back(std::make_pair(a, b));
		else
			_pair.push_back(std::make_pair(b, a));
	}
	for (size_t i = 0; i < _pair.size(); i++){
		small.push_back(_pair[i].first);
		big.push_back(_pair[i].second);
	}
	if (inputDeque.size() % 2 != 0)
		small.push_back(inputDeque.back());
}

void Pmerge::fordJohnsonDeq2(std::deque<int> &big, std::deque<int> &small){
	if (big.size() > 1){
		std::deque<int> newBig;
		std::deque<int> newSmall;
		fordJohnsonDeqPairs(big, newBig, newSmall);
		fordJohnsonDeq2(newBig, newSmall);
		big = newBig;
	}
	orderByJacobstallSeq_deq(big, small);
	while(!small.empty()){
		int value = small.back();
		std::deque<int>::iterator pos = std::lower_bound(big.begin(), big.end(), value);
		big.insert(pos, value);
		small.pop_back();
	}
}

void Pmerge::orderByJacobstallSeq_deq(std::deque<int> &big, std::deque<int> &small){
	std::vector<size_t> _jacobSeq = generateJacobstallSequence(small.size());
	while(!_jacobSeq.empty()){
		size_t index = _jacobSeq.front();
		if (index < small.size()){
			int value = small[index];
			std::deque<int>::iterator pos = std::lower_bound(big.begin(), big.end(), value);
			big.insert(pos, value);
			std::deque<int>::iterator it = std::find(small.begin(), small.end(), value);
			small.erase(it);
		}
		_jacobSeq.erase(_jacobSeq.begin());
	}
}

void Pmerge::printTimeDec(void){
	std::cout << "Time to process a range of " << this->_deq.size();
	std::cout << " elements with std::deque : " << this->_dTime << " us" << std::endl;
}
