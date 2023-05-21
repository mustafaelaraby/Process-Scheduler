#pragma once
#include <vector>

template<typename Vector>
class VectorIterator
{
public:
	using ValueType = typename Vector::ValueType;
	using PointerType = ValueType*;
	using ReferenceType = ValueType&;
public:
	VectorIterator()
		:m_ptr(nullptr) {}

	VectorIterator(PointerType ptr)
		:m_ptr(ptr) {}

	VectorIterator& operator++()
	{
		m_ptr++;
		return *this;
	}

	VectorIterator operator++(int)
	{
		VectorIterator iterator = *this;
		++(*this);
		return iterator;
	}

	VectorIterator& operator--()
	{
		m_ptr--;
		return *this;
	}

	VectorIterator operator--(int)
	{
		VectorIterator iterator = *this;
		--(*this);
		return iterator;
	}

	ReferenceType operator[](int index)
	{
		return *(m_ptr + index);
	}

	PointerType operator->()
	{
		return m_ptr;
	}

	ReferenceType operator*()
	{
		return *m_ptr;
	}

	bool operator==(VectorIterator other)
	{
		return m_ptr == other.m_ptr;
	}

	bool operator!=(VectorIterator other)
	{
		return !(m_ptr == other.m_ptr);
	}

private:
	PointerType m_ptr;
};


template<typename T>
class Vector
{
public:
	using ValueType = T;
	using Iterator = VectorIterator<Vector<T>>;
public:
	Vector()
	{
		//Alloc
		ReAlloc(2);
	}

	Vector(const Vector& other) :m_size(other.m_size), m_capacity(other.m_capacity)
	{
		m_data = new T[m_capacity];
		for (size_t i = 0; i < other.m_size; ++i)
			m_data[i] = other.m_data[i];
	}

	Vector(Vector&& other)noexcept :m_data(other.m_data), m_size(other.m_size), m_capacity(other.m_capacity)
	{
		other.m_data = nullptr;
		other.m_size = 0;
		other.m_capacity = 0;
	}

	Vector& operator=(const Vector& other) {
		if (this != &other) {
			delete[] m_data;
			size = other.size;
			m_capacity = other.m_capacity;
			m_data = new T[m_capacity];
			for (size_t i = 0; i < size; ++i)
				m_data[i] = other.m_data[i];
		}
		return *this;
	}

	Vector& operator=(Vector&& other) noexcept {
		if (this != &other) {
			delete[] m_data;
			m_data = other.m_data;
			size = other.size;
			m_capacity = other.m_capacity;
			other.m_data = nullptr;
			other.size = 0;
			other.m_data = 0;
		}
		return *this;
	}



	void push_back(const T& value)
	{
		if (m_size >= m_capacity)
			ReAlloc(m_capacity + m_capacity / 2);

		m_data[m_size++] = value;
	}

	void push_back(T&& value)
	{
		if (m_size >= m_capacity)
		{
			ReAlloc(m_capacity + m_capacity / 2);
		}
		new (&m_data[m_size++]) T(std::move(value));
	}

	T& front()
	{
		return m_data[0];
	}

	T& back()
	{
		return m_data[m_size - 1];
	}


	template<typename ...Args>
	T& emplace_back(Args&&...args)
	{
		if (m_size >= m_capacity)
			ReAlloc(m_capacity + m_capacity / 2);
		new(&m_data[m_size]) T(std::forward<Args>(args)...);
		return m_data[m_size++];
	}

	void pop_back()
	{
		if (m_size > 0)
		{
			m_size--;
			m_data[m_size].~T();
		}
	}

	void pop_front()
	{
		if (m_size > 0)
		{
			m_size--;
			m_data[0].~T();
			for (int i = 0; i < m_size; ++i)
			{
				m_data[i] = std::move(m_data[i + 1]);
				//m_data[i + 1].~T();
			}
		}
	}

	void clear()
	{
		for (size_t i = 0; i < m_size; i++)
			m_data[i].~T();

		m_size = 0;
	}

	Iterator begin()
	{
		return Iterator(m_data);
	}

	Iterator end()
	{
		return Iterator(m_data + m_size);
	}

	Iterator middle()
	{
		return Iterator((m_data + floor(m_size / 2)));
	}

	const T& operator[](size_t index)const
	{
		if (index >= m_size)
		{
			//assert
		}
		return m_data[index];
	}

	T& operator[](size_t index)
	{
		if (index >= m_size)
		{
			//assert
		}
		return m_data[index];
	}

	~Vector()
	{
		clear();
		::operator delete(m_data, sizeof(m_capacity));
	}

	size_t size()const { return m_size; }

	bool empty()const
	{
		return (m_size == 0);
	}

	Iterator binarySearch(const T& value) {
		size_t left = 0;
		size_t right = size() - 1;

		while (left <= right) {
			size_t mid = left + (right - left) / 2;
			if (value == m_data[mid]) {
				return Iterator(&m_data[mid]);
			}
			else if (value < m_data[mid]) {
				right = mid - 1;
			}
			else {
				left = mid + 1;
			}
		}

		return end();
	}
private:
	void ReAlloc(size_t newCapacity)
	{
		T* newBlock = (T*)::operator new(newCapacity * sizeof(T));

		if (newCapacity < m_size)
			m_size = newCapacity;
		for (size_t i = 0; i < m_size; i++)
			new (&newBlock[i]) T(std::move(m_data[i]));


		for (size_t i = 0; i < m_size; i++)
			m_data[i].~T();

		::operator delete(m_data, sizeof(newCapacity));
		m_data = newBlock;
		m_capacity = newCapacity;
	}


private:
	T* m_data = nullptr;
	size_t m_size = 0;
	size_t m_capacity = 0;
};