#pragma once

#include <list>
#include <iostream>

template <typename T>
class PriorityQueue
{
public:
	PriorityQueue() = default;
	void push(const T& move);	// o(n) max complexity
	const T poll();				// o(1) complexity
	const std::list<T>& getQueue() const;

private:
	std::list<T> m_queue;

};

// This struct will be used when entering moves into our priority queue- in order to find the location of the move.
// This means that there'll be a loop that'll call this function with each member in the queue (or up to where already smaller)
// and will find the right place to insert it! meaning the push func will call it!

//-----------------------------------------------------------------------------
// Comparator struct
//-----------------------------------------------------------------------------
template <typename T>
struct MyComparator {

	bool operator()(const T& move1, const T& move2) const {
		return move1 > move2;	
	}
};


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------

template<typename T>
void PriorityQueue<T>::push(const T& move) {

	std::cout << "Pushing move: " << move << " with score: " << move.getScore() << std::endl;

	MyComparator<T> cmp;
	auto it = m_queue.begin();

	while (it != m_queue.end() && !cmp(move, *it)) {
		++it;
	}

	m_queue.insert(it, move);

	// Keep only 5 best moves
	if (m_queue.size() > 5) {
		m_queue.pop_back();
	}

	std::cout << "__________________________________________________\n"<< std::endl;

}


template<typename T>
const T PriorityQueue<T>::poll() {
	T bestMove = m_queue.front();
	m_queue.pop_front();
	return bestMove;
}

template<typename T>
const std::list<T>& PriorityQueue<T>::getQueue() const
{
	return m_queue;
}


//-----------------------------------------------------------------------------
// Global operators (declarations)
//-----------------------------------------------------------------------------
template <typename T>
std::ostream& operator<<(std::ostream& os, const PriorityQueue<T>& pq) {

	os << "recommended moves:\n";

	int i = 1;

	for (const auto& move : pq.getQueue()) {

		if (i > 3) break;

		os << i << ") " << move << std::endl;
		++i;
	}
	
	return os;
}
