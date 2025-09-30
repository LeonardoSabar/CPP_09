#include "../include/PmergeMe.hpp"

/**
 * @brief Default constructor for the Pmerge class.
 *
 * Initializes a new Pmerge object with empty containers and zero timing values.
 * The member vectors and deques are default-constructed (empty), and timing
 * variables are explicitly set to 0.0.
 *
 * @note Member containers (_vec, _deq) are automatically initialized as empty
 * @note Timing variables (_vTime, _dTime) are explicitly initialized to prevent undefined behavior
 */
Pmerge::Pmerge() : _vTime(0.0), _dTime(0.0){}

/**
 * @brief Destructor for the Pmerge class.
 *
 * Cleans up any resources used by the Pmerge object. Since this class only uses
 * standard library containers that handle their own memory management, no explicit
 * cleanup is required.
 *
 * @note STL containers automatically handle their own memory deallocation
 */
Pmerge::~Pmerge(){}

/**
 * @brief Copy constructor for the Pmerge class.
 *
 * Creates a new Pmerge object as a copy of another Pmerge object. All member
 * variables including containers and timing data are copied from the source object.
 *
 * @param other The Pmerge object to copy from
 *
 * @note Performs deep copy of all member containers and variables
 * @note The assignment in the constructor body is redundant with the initializer list
 */
Pmerge::Pmerge(const Pmerge &other) : _vec(other._vec), _deq(other._deq), _vTime(other._vTime), _dTime(other._dTime) {
	this->_vec = other._vec;
	this->_deq =  other._deq;
}

/**
 * @brief Assignment operator for the Pmerge class.
 *
 * Assigns the contents of another Pmerge object to this object. Performs
 * self-assignment check to prevent issues when an object is assigned to itself.
 *
 * @param other The Pmerge object to assign from
 * @return Reference to this object after assignment
 *
 * @note Includes self-assignment protection
 * @note Only copies container data, timing variables are not copied in current implementation
 * @warning Timing variables (_vTime, _dTime) are not copied, which may be intentional
 */
Pmerge &Pmerge::operator=(const Pmerge &other){
	if (this != &other){
		this->_vec = other._vec;
		this->_deq = other._deq;
	}
	return *this;
}

/**
 * @brief Main execution function that orchestrates the Ford-Johnson sorting demonstration.
 *
 * This function performs the complete Ford-Johnson sorting demonstration by:
 * 1. Validating command line arguments
 * 2. Populating both vector and deque containers with the input data
 * 3. Running and timing the Ford-Johnson algorithm on both containers
 * 4. Displaying results and performance metrics
 *
 * The function demonstrates the performance comparison between std::vector and
 * std::deque implementations of the same Ford-Johnson algorithm.
 *
 * @param argc Number of command line arguments (including program name)
 * @param argv Array of command line argument strings
 *
 * @note Exits early with error message if argument validation fails
 * @note Both containers are populated with identical data for fair comparison
 * @note Displays before/after states and timing information for both containers
 */
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

/**
 * @brief Validates command line arguments to ensure they are all positive integers.
 *
 * This function checks each command line argument (excluding the program name)
 * to verify that it contains only digit characters. This validation ensures
 * that all inputs can be safely converted to positive integers for sorting.
 *
 * @param argc Number of command line arguments (including program name)
 * @param argv Array of command line argument strings
 * @return true if all arguments are valid positive integers, false otherwise
 *
 * @note Skips argv[0] (program name) and starts validation from argv[1]
 * @note Only accepts strings containing digit characters (0-9)
 * @note Does not accept negative numbers, floating point numbers, or non-numeric input
 * @note Empty strings or arguments starting with non-digits will fail validation
 */
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

/**
 * @brief Generates a Jacobsthal sequence up to a specified length.
 *
 * The Jacobsthal sequence is defined by the recurrence relation:
 * J(0) = 1, J(1) = 1, J(n) = J(n-1) + 2*J(n-2) for n > 1
 * This produces the sequence: 1, 1, 3, 5, 11, 21, 43, 85, 171, ...
 *
 * This sequence is used in the Ford-Johnson algorithm to determine the optimal
 * order for inserting elements, which minimizes the number of comparisons needed.
 *
 * @param len The maximum value up to which to generate the Jacobsthal sequence
 * @return Vector containing Jacobsthal numbers less than 'len'
 *
 * @note The function stops generating when the next Jacobsthal number would exceed 'len'
 * @note Used internally by the Ford-Johnson algorithm for optimal insertion ordering
 * @note The sequence starts with J(1) = 1, effectively skipping J(0)
 */
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

/**
 * @brief Calculates the elapsed time between two clock measurements.
 *
 * This function computes the time difference between a start and end clock measurement
 * and converts it to microseconds. The calculation uses CLOCKS_PER_SEC to normalize
 * the clock ticks to seconds, then multiplies by 100,000 to convert to microseconds.
 *
 * @param start Starting clock measurement (from clock())
 * @param end   Ending clock measurement (from clock())
 * @return Elapsed time in microseconds as a double
 *
 * @note Uses static_cast to ensure floating-point division
 * @note Result is in microseconds (multiplied by 100,000)
 * @note Assumes CLOCKS_PER_SEC gives clock ticks per second
 */
double Pmerge::calc_time(clock_t start, clock_t end){
	double time = static_cast<double>(end - start) / CLOCKS_PER_SEC * 100000;
	return time;
}
