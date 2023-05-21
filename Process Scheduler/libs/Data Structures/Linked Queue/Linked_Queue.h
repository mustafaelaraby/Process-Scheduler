#pragma once
#include <iostream>

#include "Node.h"
#include <assert.h>


template<typename T>
class Linked_Queue
{
	Node<T>* _FRONT_;
	Node<T>* _REAR_;
	size_t _SIZE_;

public:
	Linked_Queue();

	bool empty()const;
	size_t size()const;

	T& front();
	T& back();

	void push(const T&);
	void push(T&&);
	void pop();

	void display();
};

template <typename T>
Linked_Queue<T>::Linked_Queue()
{
	_FRONT_ = nullptr;
	_REAR_ = nullptr;
}

template <typename T>
bool Linked_Queue<T>::empty() const
{
	return((_FRONT_ == nullptr && _REAR_ == nullptr));
}

template <typename T>
size_t Linked_Queue<T>::size() const
{
	size_t size = 0;
	Node<T>* temp = _FRONT_;
	while (temp != nullptr)
	{
		++size;
		temp = temp->getNext();
	}
	return size;
}


template <typename T>
T& Linked_Queue<T>::front()
{
	Node<T>* temp = nullptr;
	if (empty())
	{
		assert("empty Linked_Queue!");
	}
	else
	{
		return _FRONT_->getItem();
	}
}

template <typename T>
T& Linked_Queue<T>::back()
{
	if (empty())
	{
		assert("empty Linked_Queue!");
	}
	else
	{
		return _REAR_->getItem();
	}
}

template <typename T>
void Linked_Queue<T>::push(const T& item)
{
	Node<T>* newNode = new Node<T>(item);

	if (empty())
	{
		_FRONT_ = _REAR_ = newNode;
	}
	else
	{
		_REAR_->setNext(newNode);
		_REAR_ = newNode;
	}
}

template <typename T>
void Linked_Queue<T>::push(T&& item)
{
	Node<T>* newNode = new Node<T>();
	newNode->setItem(std::move(item));

	if (empty())
	{
		_FRONT_ = _REAR_ = newNode;
	}
	else
	{
		_REAR_->setNext(newNode);
		_REAR_ = newNode;
	}
}

template <typename T>
void Linked_Queue<T>::pop()
{
	if (empty())
	{
		assert("empty Linked_Queue!");
	}
	else if (_FRONT_ == _REAR_)
	{
		delete _FRONT_;
		_FRONT_ = _REAR_ = nullptr;
	}
	else
	{
		Node<T>* temp = _FRONT_;
		_FRONT_ = _FRONT_->getNext();
		delete temp;
	}
}

template <typename T>
void Linked_Queue<T>::display()
{
	Node<T>* temp = _FRONT_;
	while (temp != nullptr)
	{
		std::cout << temp->getItem() << " ";
		temp = temp->getNext();
	}
	std::cout << std::endl;
}