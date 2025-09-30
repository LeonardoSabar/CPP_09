#ifndef PMERGEME_HPP
# define PMERGEME_HPP

# include <iostream>
# include <vector>
# include <deque>
# include <string>
# include <cstdlib>
# include <cctype>
# include <algorithm>
# include <climits>

class Pmerge{
	private:
		std::vector<int> _vec;
		std::deque<int> _deq;
		double _vTime;
		double _dTime;
	public:
		Pmerge();
		~Pmerge();
		Pmerge(const Pmerge &other);
		Pmerge &operator=(const Pmerge &other);

		void	run(int argc, char **argv);
		bool	checkArgs(int argc, char **argv);
		std::vector<size_t>	generateJacobstallSequence(size_t len);
		double	calc_time(clock_t start, clock_t end);


		// methods for vector
		void	printVec(int after, std::vector<int> v);
		void	fordJohnsonVec(void);
		void	fordJohnsonVecPairs(const std::vector<int> &inputVec, std::vector<int> &big, std::vector<int> &small);
		void	fordJohnsonStep2(std::vector<int> &big, std::vector<int> &small);
		void	orderByJacobstallSeq_vec(std::vector<int> &big, std::vector<int> &small);
		void	printTimeVec(void);
		std::vector<int> &getVec(void);

		// methods for deque
		void	printDec(int after, std::deque<int> d);
		std::deque<int> &getDeq(void);
		void	fordJohnsonDeq(void);
		void	fordJohnsonDeqPairs(const std::deque<int> &inputDeq, std::deque<int> &big, std::deque<int> &small);
		void	fordJohnsonDeq2(std::deque<int> &big, std::deque<int> &small);
		void	orderByJacobstallSeq_deq(std::deque<int> &big, std::deque<int> &small);
		void	printTimeDec(void);
};


#endif
