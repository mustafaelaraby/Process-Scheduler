#include "../Vector/Vector.h"

template <typename T>
class Stack {
public:
	// Default constructor
	Stack() = default;

	// Destructor
	~Stack() = default;

	// Push an element onto the stack
	void push(const T& element) {
		data.push_back(element);
	}

	// Remove and return the top element from the stack
	T pop() {
		if (empty())
			throw std::out_of_range("Stack is empty");

		T topElement = data.back();
		data.pop_back();
		return topElement;
	}

	// Return the top element from the stack without removing it
	T& top() {
		if (empty())
			throw std::out_of_range("Stack is empty");

		return data.back();
	}

	// Check if the stack is empty
	bool empty() const {
		return data.empty();
	}

	// Return the number of elements in the stack
	std::size_t size() const {
		return data.size();
	}

private:
	Vector<T> data;
};
