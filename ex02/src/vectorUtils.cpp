#include "../include/PmergeMe.hpp"

/**
 * @brief Prints the contents of a vector with formatted output.
 *
 * This function prints the elements of the provided vector with a label indicating
 * whether it's the "begin" or "after" state. If the member vector size is less than 15,
 * all elements are printed. Otherwise, only the first 4 elements are shown followed by "[...]"
 * to indicate there are more elements.
 *
 * @param after Integer flag: 0 for "begin:" label, non-zero for "after:" label
 * @param v     Vector of integers to be printed
 *
 * @note Uses the size of this->_vec (not parameter v) to determine print format
 * @note Output format: "begin: 1 2 3 4 [...]" or "after: 1 2 3 4 5"
 */
void Pmerge::printVec(int after, std::vector<int> v){
	if (after == 0)
		std::cout << "begin: ";
	else
		std::cout << "after: ";
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

/**
 * @brief Returns a reference to the member vector.
 *
 * This function provides access to the private member vector `_vec` of the Pmerge class.
 * It returns a reference, allowing both read and write operations on the vector.
 *
 * @return Reference to the member vector `_vec`
 *
 * @note This function allows external modification of the internal vector state
 * @note Typically used for accessing the sorted result after algorithm execution
 */
std::vector<int> &Pmerge::getVec(void){
	return this->_vec;
}

/**
 * @brief Executes the Ford-Johnson sorting algorithm on the member vector.
 *
 * This function performs the Ford-Johnson sort (also known as merge-insertion sort)
 * on the member vector `_vec` of the `Pmerge` class. It divides the vector into pairs,
 * processes them to separate small and big values, and then merges them in a specific order.
 * The function also measures and stores the time taken to perform the sort in `_vTime`.
 *
 * Steps:
 * 1. Splits the input vector into pairs and separates small and big values.
 * 2. Merges the values using the Ford-Johnson algorithm.
 * 3. Updates the member vector with the sorted result.
 * 4. Records the elapsed time for the sorting process.
 */
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

/**
 * @brief Splits the input vector into pairs, sorts each pair, and separates them into 'big' and 'small' vectors.
 *
 * This function processes the input vector by grouping its elements into pairs.
 * For each pair, the smaller element is placed in the 'small' vector and the larger in the 'big' vector.
 * If the input vector has an odd number of elements, the last element is added to the 'small' vector.
 *
 * @param inputVec The input vector of integers to be processed.
 * @param big Reference to a vector where the larger elements of each pair will be stored.
 * @param small Reference to a vector where the smaller elements of each pair (and possibly the last unpaired element) will be stored.
 */
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

/**
 * @brief Performs the second step of the Ford-Johnson merge-insertion sort algorithm on two vectors.
 *
 * This function recursively processes the 'big' and 'small' vectors by pairing and reducing them,
 * then merges the elements from 'small' into 'big' in sorted order using the Jacobsthal sequence.
 * The function modifies the 'big' vector in place to contain the merged and sorted result.
 *
 * @param big   Reference to the vector containing the larger elements (will be modified in place).
 * @param small Reference to the vector containing the smaller elements (will be emptied).
 */
void Pmerge::fordJohnsonStep2(std::vector<int> &big, std::vector<int> &small){
	if (big.size() > 1){
		std::vector<int> newBig;
		std::vector<int> newSmall;
		fordJohnsonVecPairs(big, newBig, newSmall);
		fordJohnsonStep2(newBig, newSmall);
		big = newBig;
	}
	orderByJacobstallSeq_vec(big, small);
	// order following small order back to front, when there is no more sequence to add
	while(!small.empty()){
		int value = small.back();
		std::vector<int>::iterator pos = std::lower_bound(big.begin(), big.end(), value);
		big.insert(pos, value);
		small.pop_back();
	}
}

/**
 * @brief Orders elements from 'small' vector into 'big' vector using the Jacobsthal sequence.
 *
 * This function implements the core insertion step of the Ford-Johnson algorithm. It uses the
 * Jacobsthal sequence to determine the optimal order for inserting elements from the 'small'
 * vector into the already sorted 'big' vector. The Jacobsthal sequence minimizes the number
 * of comparisons needed during the binary search insertion process.
 *
 * Algorithm steps:
 * 1. Generate Jacobsthal sequence based on the size of 'small' vector
 * 2. For each index in the Jacobsthal sequence:
 *    - Get the corresponding element from 'small' vector
 *    - Find the correct insertion position in 'big' using binary search
 *    - Insert the element and remove it from 'small'
 *
 * @param big   Reference to the sorted vector where elements will be inserted
 * @param small Reference to the vector containing elements to be inserted (will be modified)
 *
 * @note The 'big' vector must be sorted before calling this function
 * @note The 'small' vector will be modified (elements removed) during execution
 * @note Uses std::lower_bound for O(log n) insertion position finding
 * @warning Missing bounds checking - should verify index < small.size() before access
 */
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

/**
 * @brief Prints the execution time for the vector-based Ford-Johnson sorting algorithm.
 *
 * This function outputs a formatted message showing the time taken to process
 * a range of elements using std::vector as the container type. The time is
 * measured in microseconds and stored in the private member variable '_vTime'.
 *
 * Output format: "Time to process a range of [size] elements with std::vector : [time] us"
 *
 * @note The time value is calculated and stored during the execution of fordJohnsonVec()
 * @note Time is displayed in microseconds (us)
 * @note The element count corresponds to the final size of the sorted vector
 */
void Pmerge::printTimeVec(void){
	std::cout << "Time to process a range of " << this->_vec.size();
	std::cout << " elements with std::vector : " << this->_vTime << " us" << std::endl;
}
