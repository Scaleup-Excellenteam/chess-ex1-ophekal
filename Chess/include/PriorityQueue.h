#pragma once

#include <list>
#include <iostream>
#include "Exceptions/EmptyQueueException.h"


/**
 * A priority queue implementation that maintains elements in sorted order.
 * Keeps only the top 5 elements to optimize performance.
 *
 * @tparam T The type of elements stored in the queue.
 */

template <typename T>
class PriorityQueue
{
public:
	PriorityQueue() = default;
	void push(const T& move);	// o(n) max complexity
	void poll();				// o(1) complexity
	const std::list<T>& getQueue() const;
	bool isEmpty() const;

private:
	std::list<T> m_queue;

};



//-----------------------------------------------------------------------------
/*
Comparator struct

Comparator struct used to determine insertion order in the priority queue.
Uses the > operator to compare elements, maintaining descending order.

@tparam T The type of elements to compare.
*/ 

//-----------------------------------------------------------------------------

template <typename T>
struct MyComparator {

	/**
	 * Compares two elements to determine their relative priority.
	 *
	 * @param move1 The first element to compare.
	 * @param move2 The second element to compare.
	 * @return True if move1 has higher priority than move2.
	 */
	bool operator()(const T& move1, const T& move2) const {
		return move1 > move2;	
	}
};


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------

/**
* Inserts an element into the queue in sorted order.
*
* @param move The element to insert.
*/
template<typename T>
void PriorityQueue<T>::push(const T& move) {

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
}


/**
 * Removes the highest priority element from the queue.
 *
 * @throws EmptyQueueException If the queue is empty.
 */
template<typename T>
void PriorityQueue<T>::poll() {
	
	if (m_queue.empty()) {
		throw EmptyQueueException();
	}
	m_queue.pop_front();
}


/**
 * Returns a constant reference to the underlying queue.
 *
 * @return A constant reference to the list containing all elements.
 */
template<typename T>
const std::list<T>& PriorityQueue<T>::getQueue() const
{
	return m_queue;
}


/**
 * Checks if the queue is empty.
 *
 * @return True if the queue is empty, false otherwise.
 */
template<typename T>
bool PriorityQueue<T>::isEmpty() const {
	return m_queue.empty();
}


//-----------------------------------------------------------------------------
// Global operators
//-----------------------------------------------------------------------------
/**
 * Stream insertion operator for PriorityQueue.
 * Displays the top 3 recommended moves from the queue.
 *
 * @param os The output stream.
 * @param pq The priority queue to display.
 * @return The output stream.
 */
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
