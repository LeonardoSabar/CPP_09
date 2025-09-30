#include "../include/PmergeMe.hpp"

/**
 * @brief Prints the contents of a deque with formatted output.
 *
 * This function prints the elements of the provided deque with a label indicating
 * whether it's the "begin" or "after" state. If the deque size is less than 15,
 * all elements are printed. Otherwise, only the first 4 elements are shown followed by "[...]"
 * to indicate there are more elements.
 *
 * @param after Integer flag: 0 for "begin:" label, non-zero for "after:" label
 * @param d     Deque of integers to be printed
 *
 * @note Uses the size of parameter d (not this->_deq) to determine print format
 * @note Output format: "begin: 1 2 3 4 [...]" or "after: 1 2 3 4 5"
 */
void Pmerge::printDec(int after, std::deque<int> d){
	if (!after)
		std::cout << "begin: ";
	else
		std::cout << "after: ";
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

/**
 * @brief Returns a reference to the member deque.
 *
 * This function provides access to the private member deque `_deq` of the Pmerge class.
 * It returns a reference, allowing both read and write operations on the deque.
 *
 * @return Reference to the member deque `_deq`
 *
 * @note This function allows external modification of the internal deque state
 * @note Typically used for accessing the sorted result after algorithm execution
 */
std::deque<int> &Pmerge::getDeq(void){
	return this->_deq;
}

/**
 * @brief Executes the Ford-Johnson sorting algorithm on the member deque.
 *
 * This function performs the Ford-Johnson sort (also known as merge-insertion sort)
 * on the member deque `_deq` of the `Pmerge` class. It divides the deque into pairs,
 * processes them to separate small and big values, and then merges them in a specific order.
 * The function also measures and stores the time taken to perform the sort in `_dTime`.
 *
 * Steps:
 * 1. Splits the input deque into pairs and separates small and big values.
 * 2. Merges the values using the Ford-Johnson algorithm.
 * 3. Updates the member deque with the sorted result.
 * 4. Records the elapsed time for the sorting process.
 */
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

/**
 * @brief Splits the input deque into pairs, sorts each pair, and separates them into 'big' and 'small' deques.
 *
 * This function processes the input deque by grouping its elements into pairs.
 * For each pair, the smaller element is placed in the 'small' deque and the larger in the 'big' deque.
 * If the input deque has an odd number of elements, the last element is added to the 'small' deque.
 *
 * @param inputDeque The input deque of integers to be processed.
 * @param big        Reference to a deque where the larger elements of each pair will be stored.
 * @param small      Reference to a deque where the smaller elements of each pair (and possibly the last unpaired element) will be stored.
 *
 * @note The function preserves the relative order of pairs while ensuring each pair is sorted
 * @note Handles odd-sized inputs by placing the last element in the 'small' deque
 */
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

/**
 * @brief Performs the second step of the Ford-Johnson merge-insertion sort algorithm on two deques.
 *
 * This function recursively processes the 'big' and 'small' deques by pairing and reducing them,
 * then merges the elements from 'small' into 'big' in sorted order using the Jacobsthal sequence.
 * The function modifies the 'big' deque in place to contain the merged and sorted result.
 *
 * Algorithm steps:
 * 1. If big has more than 1 element, recursively sort it using pair reduction
 * 2. Insert elements from small into big using Jacobsthal sequence order
 * 3. Insert any remaining elements from small in back to front sequential order
 *
 * @param big   Reference to the deque containing the larger elements (will be modified in place).
 * @param small Reference to the deque containing the smaller elements (will be emptied).
 *
 * @note The recursion ensures that 'big' is sorted before the insertion phase
 * @note Uses both Jacobsthal sequence and sequential insertion for optimal performance
 */
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

/**
 * @brief Orders elements from 'small' deque into 'big' deque using the Jacobsthal sequence.
 *
 * This function implements the core insertion step of the Ford-Johnson algorithm for deques. It uses the
 * Jacobsthal sequence to determine the optimal order for inserting elements from the 'small'
 * deque into the already sorted 'big' deque. The Jacobsthal sequence minimizes the number
 * of comparisons needed during the binary search insertion process.
 *
 * Algorithm steps:
 * 1. Generate Jacobsthal sequence based on the size of 'small' deque
 * 2. For each valid index in the Jacobsthal sequence:
 *    - Get the corresponding element from 'small' deque
 *    - Find the correct insertion position in 'big' using binary search
 *    - Insert the element and remove it from 'small'
 *
 * @param big   Reference to the sorted deque where elements will be inserted
 * @param small Reference to the deque containing elements to be inserted (will be modified)
 *
 * @note The 'big' deque must be sorted before calling this function
 * @note The 'small' deque will be modified (elements removed) during execution
 * @note Uses std::lower_bound for O(log n) insertion position finding
 * @note Includes bounds checking to prevent invalid array access
 */
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

/**
 * @brief Prints the execution time for the deque-based Ford-Johnson sorting algorithm.
 *
 * This function outputs a formatted message showing the time taken to process
 * a range of elements using std::deque as the container type. The time is
 * measured in microseconds and stored in the private member variable '_dTime'.
 *
 * Output format: "Time to process a range of [size] elements with std::deque : [time] us"
 *
 * @note The time value is calculated and stored during the execution of fordJohnsonDeq()
 * @note Time is displayed in microseconds (us)
 * @note The element count corresponds to the final size of the sorted deque
 */
void Pmerge::printTimeDec(void){
	std::cout << "Time to process a range of " << this->_deq.size();
	std::cout << " elements with std::deque : " << this->_dTime << " us" << std::endl;
}
