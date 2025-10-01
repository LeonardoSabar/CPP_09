#include "../include/PmergeMe.hpp"

void Pmerge::printVec(int after, std::vector<int> v){
	if (after == 0)
		std::cout << "Before: ";
	else
		std::cout << "After: ";
	if (this->_vec.size() < 15){
		for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it)
			std::cout << *it << " ";
	}
	else{
		int count = 0;
		for (std::vector<int>::iterator it = v.begin(); it != v.end() && count < 4; ++it){
			std::cout << *it << " ";
			count++;
		}
		std::cout << " [...]";
	}
	std::cout << std::endl;

}

std::vector<int> &Pmerge::getVec(void){
	return this->_vec;
}

void Pmerge::fordJohnsonVec(){
	std::vector<int> _smallValues;
	std::vector<int> _bigValues;

	clock_t vec_start = clock();
	fordJohnsonVecPairs(this->_vec, _bigValues, _smallValues);
	fordJohnsonStep2(_bigValues, _smallValues);
	this->_vec = _bigValues;
	clock_t vec_end = clock();
	this->_vTime = calc_time(vec_start, vec_end);
}

void Pmerge::fordJohnsonVecPairs(const std::vector<int> &inputVec, std::vector<int> &big, std::vector<int> &small){
	std::vector<std::pair<int, int> > _pairs;

	for (size_t i = 0; i + 1 < inputVec.size(); i += 2){
		int a = inputVec[i];
		int b = inputVec[i + 1];
		if (a < b)
			_pairs.push_back(std::make_pair(a, b));
		else
			_pairs.push_back(std::make_pair(b, a));
	}
	for (size_t i = 0; i < _pairs.size(); i++){
		small.push_back(_pairs[i].first);
		big.push_back(_pairs[i].second);
	}
	if (inputVec.size() % 2 != 0)
		small.push_back(inputVec.back());
}

void Pmerge::fordJohnsonStep2(std::vector<int> &big, std::vector<int> &small){
	if (big.size() > 1){
		std::vector<int> newBig;
		std::vector<int> newSmall;
		fordJohnsonVecPairs(big, newBig, newSmall);
		fordJohnsonStep2(newBig, newSmall);
		big = newBig;
	}
	orderByJacobstallSeq_vec(big, small);
	while(!small.empty()){
		int value = small.back();
		std::vector<int>::iterator pos = std::lower_bound(big.begin(), big.end(), value);
		big.insert(pos, value);
		small.pop_back();
	}
}

void Pmerge::orderByJacobstallSeq_vec(std::vector<int> &big, std::vector<int> &small){
	std::vector<size_t> _jacobSeq = generateJacobstallSequence(small.size());

	while(!_jacobSeq.empty()){
		int value = small[_jacobSeq.front()];
		std::vector<int>::iterator pos = std::lower_bound(big.begin(), big.end(), value);
		big.insert(pos, value);
		std::vector<int>::iterator it = std::find(small.begin(), small.end(), value);
		small.erase(it);
		_jacobSeq.erase(_jacobSeq.begin());
	}
}

void Pmerge::printTimeVec(void){
	std::cout << "Time to process a range of " << this->_vec.size();
	std::cout << " elements with std::vector : " << this->_vTime << " us" << std::endl;
}
