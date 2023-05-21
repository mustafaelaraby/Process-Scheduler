#pragma once
template<typename T>
class node {
public:
	T data;
	node<T>* next;

	node(const T& value) : data(value), next(nullptr) {}
};

template<typename T>
class Queue {
private:
	node<T>* m_front;
	node<T>* m_back;
	size_t m_size;

public:
	Queue() : m_front(nullptr), m_back(nullptr), m_size(0) {}

	Queue(const Queue& other) : m_front(nullptr), m_back(nullptr), m_size(0) {
		node<T>* curr = other.m_front;
		while (curr) {
			push(curr->data);
			curr = curr->next;
		}
	}

	Queue(Queue&& other) noexcept : m_front(other.m_front), m_back(other.m_back), m_size(other.m_size) {
		other.m_front = nullptr;
		other.m_back = nullptr;
		other.m_size = 0;
	}

	~Queue() {
		while (!empty()) {
			pop();
		}
	}

	Queue& operator=(const Queue& other) {
		if (this != &other) {
			Queue temp(other);
			swap(temp);
		}
		return *this;
	}

	Queue& operator=(Queue&& other) noexcept {
		if (this != &other) {
			clear();

			m_front = other.m_front;
			m_back = other.m_back;
			m_size = other.m_size;

			other.m_front = nullptr;
			other.m_back = nullptr;
			other.m_size = 0;
		}
		return *this;
	}

	void swap(Queue& other) noexcept {
		std::swap(m_front, other.m_front);
		std::swap(m_back, other.m_back);
		std::swap(m_size, other.m_size);
	}

	bool empty() const {
		return (m_size == 0);
	}

	size_t size() const {
		return m_size;
	}

	const T& front() const {
		if (empty()) {
			throw std::runtime_error("Queue is empty");
		}
		return m_front->data;
	}

	const T& back() const {
		if (empty()) {
			throw std::runtime_error("Queue is empty");
		}
		return m_back->data;
	}

	void push(const T& value) {
		node<T>* newNode = new node<T>(value);
		if (empty()) {
			m_front = newNode;
			m_back = newNode;
		}
		else {
			m_back->next = newNode;
			m_back = newNode;
		}
		m_size++;
	}

	void push(T&& value) {
		node<T>* newNode = new node<T>(std::move(value));
		if (empty()) {
			m_front = newNode;
			m_back = newNode;
		}
		else {
			m_back->next = newNode;
			m_back = newNode;
		}
		m_size++;
	}

	void pop() {
		if (empty()) {
			throw std::runtime_error("Queue is empty");
		}
		node<T>* temp = m_front;
		m_front = m_front->next;
		delete temp;
		m_size--;
		if (empty()) {
			m_back = nullptr;
		}
	}

	void clear() {
		while (!empty()) {
			pop();
		}
	}
};
