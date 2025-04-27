#pragma once

#include <list>
#include <ostream>

template <typename T>
class PriorityQueue
{
public:
	PriorityQueue<T>();
	void push(const T& move);	// o(n) max complexity
	const T pull();				// o(1) complexity
	std::ostream& operator<<(std::ostream& os);


private:
	std::list<T> m_queue;

};


// This struct will be used when entering moves into our priority queue- in order to find the location of the move.
// This means that there'll be a loop that'll call this function with each member in the queue (or up to where already smaller)
// and will find the right place to insert it! meaning the push func will call it!
template <typename T>
struct MyComparator {

	T operator()(const T& move1, const T& move2) const {

		// here i wull do the comparing of the two T objects
		
	}
};



//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------


template<typename T>
PriorityQueue<T>::PriorityQueue()
{
}

template<typename T>
void PriorityQueue<T>::push(const T& move)
{
}

template<typename T>
const T PriorityQueue<T>::pull()
{
	return T();
}

template <typename T>
std::ostream&::PriorityQueue<T> operator<<(std::ostream& os)
{
	for (auto i = 0; i < 3 ; i++) {
		os << i + 1 << ") " << endl;
	}
	
	return os;
}