#pragma once
#include <functional>
#include "../Vector/Vector.h"

template<typename T, typename Compare = std::less<T>>
class Priority_Queue {
private:
	Vector<T> heap;
	Compare comp;

	void heapifyUp(int);
	void heapifyDown(int);
public:
	Priority_Queue() = default;
	explicit Priority_Queue(const Compare&);

	// Copy Constructor
	Priority_Queue(const Priority_Queue& other);

	// Move Constructor
	Priority_Queue(Priority_Queue&& other) noexcept;

	// Copy Assignment Operator
	Priority_Queue& operator=(const Priority_Queue& other);

	// Move Assignment Operator
	Priority_Queue& operator=(Priority_Queue&& other) noexcept;

	bool empty()const;
	size_t size()const;
	const T& at(size_t);
	void show();
	T& top();
	void pop();
	void push(const T&);
	void push(T&&);
};

template <typename T, typename Compare>
Priority_Queue<T, Compare>::Priority_Queue(const Compare& compare) :comp(compare)
{}


template <typename T, typename Compare>
Priority_Queue<T, Compare>::Priority_Queue(const Priority_Queue& other)
	: heap(other.heap), comp(other.comp)
{}

template <typename T, typename Compare>
Priority_Queue<T, Compare>::Priority_Queue(Priority_Queue&& other) noexcept
	: heap(std::move(other.heap)), comp(std::move(other.comp))
{}

template <typename T, typename Compare>
Priority_Queue<T, Compare>& Priority_Queue<T, Compare>::operator=(const Priority_Queue& other)
{
	if (this != &other) {
		heap = other.heap;
		comp = other.comp;
	}
	return *this;
}

template <typename T, typename Compare>
Priority_Queue<T, Compare>& Priority_Queue<T, Compare>::operator=(Priority_Queue&& other) noexcept
{
	if (this != &other) {
		heap = std::move(other.heap);
		comp = std::move(other.comp);
	}
	return *this;
}



template <typename T, typename Compare>
void Priority_Queue<T, Compare>::heapifyUp(int index)
{
	if (index == 0)
		return;

	int parent = (index - 1) / 2;
	if (comp(heap[index], heap[parent])) {
		std::swap(heap[parent], heap[index]);
		heapifyUp(parent);
	}
}

template <typename T, typename Compare>
void Priority_Queue<T, Compare>::heapifyDown(int index)
{
	int leftChild = 2 * index + 1;
	int rightChild = 2 * index + 2;
	int smallest = index;

	if (leftChild < heap.size() && comp(heap[leftChild], heap[smallest]))
		smallest = leftChild;
	if (rightChild < heap.size() && comp(heap[rightChild], heap[smallest]))
		smallest = rightChild;

	if (smallest != index) {
		std::swap(heap[index], heap[smallest]);
		heapifyDown(smallest);
	}
}

template <typename T, typename Compare>
bool Priority_Queue<T, Compare>::empty() const
{
	return heap.empty();
}

template <typename T, typename Compare>
size_t Priority_Queue<T, Compare>::size() const
{
	return heap.size();
}

template <typename T, typename Compare>
const T& Priority_Queue<T, Compare>::at(size_t i)
{
	if (heap.empty())
		throw std::out_of_range("indexing empty Priority Queue.");
	return heap[i];
}

template <typename T, typename Compare>
T& Priority_Queue<T, Compare>::top()
{
	if (heap.empty())
		throw std::out_of_range("indexing empty Priority Queue.");
	return heap[0];
}

template <typename T, typename Compare>
void Priority_Queue<T, Compare>::pop()
{
	if (heap.empty())
		throw std::out_of_range("pop(): Priority queue is empty.");

	std::swap(heap[0], heap[heap.size() - 1]);
	heap.pop_back();
	heapifyDown(0);
}

template <typename T, typename Compare>
void Priority_Queue<T, Compare>::push(const T& key)
{
	heap.push_back(key);
	heapifyUp(heap.size() - 1);
}

template <typename T, typename Compare>
void Priority_Queue<T, Compare>::push(T&& key)
{
	heap.push_back(std::move(key));
	heapifyUp(heap.size() - 1);
}

template <typename T, typename Compare>
void Priority_Queue<T, Compare>::show()
{
	size_t size = heap.size();
	size_t i;
	std::cout << "<";
	if (size > 0) {
		for (i = 0; i < size - 1; ++i) {
			std::cout << heap[i] << ", ";
		}
		std::cout << heap[i];
	}
	std::cout << ">" << std::endl;
}
