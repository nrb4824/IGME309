#pragma once
#include <iostream>
#include <cstring>


template <class T>
class Queue
{
private:
	T* queue;
	int size;
	int capacity;

public:
	//default constructor
	Queue()
	{
		queue = new T[1];
		size = 0;
		capacity = 1;
	}


	//copy constructor
	Queue(const Queue& other)
	{
		size = other.size;
		capacity = other.capacity;
		queue = new T[capacity];
		std::copy(other.queue, other.queue + other.size, queue);
	}

	//copy assignment constructor
	Queue& operator=(const Queue& other)
	{
		if (this == &other)			//in case where both are the same Queue
		{
			return *this;
		}
		if (queue != nullptr)		//error checking so no memory leaks
		{
			delete[] queue;
			queue = nullptr;
			size = 1;
		}

		size = other.size;
		capacity = other.capacity;
		queue = new T[capacity];
		std::copy(other.queue, other.queue + other.size, queue);

		return *this;		//returns a Queue

	}

	//Deconstructor
	~Queue()
	{
		delete[] queue;
		queue = nullptr;
	}

	///Push method. Puts temp at the back of the queue
	///If the queue needs more space, creates more space
	void Push(T temp)
	{
		if (size == capacity)
		{
			T* tempQueue = new T[capacity * 2];
			for (int i = 0; i < size; i++)
			{
				tempQueue[i] = queue[i];
			}
			delete[] queue;
			queue = tempQueue;
			tempQueue = nullptr;
			capacity = capacity * 2;

		}

		queue[size] = temp;
		size++;
		Sort();
	}

	void Sort()
	{
		for (int i = 0; i < size - 1; i++)
		{
			for (int j = 0; j < size - i - 1; j++)
			{
				if (queue[j] > queue[j + 1])
				{
					T t = queue[j];
					queue[j] = queue[j + 1];
					queue[j + 1] = t;
				}
			}
		}
	}

	void Shift(int index)
	{
		for (int t = size - 1; t >= index; t--)
		{
			queue[t + 1] = queue[t];
		}
	}
	/// <summary>
	/// Pops the front element of the Queue
	/// </summary>
	/// <returns>the front element of the Queue</returns>
	T Pop()
	{

		T temp = queue[0];
		for (int i = 0; i < size; i++)
		{
			queue[i] = queue[i + 1];
		}
		size--;
		return temp;
	}

	/// <summary>
	/// Prints out the elments of my queue to the console.
	/// </summary>
	void Print()
	{
		for (int i = 0; i < size; i++)
		{
			std::cout << queue[i] << ", ";
		}
		std::cout << std::endl;
	}

	/// <summary>
	/// Finds and returns the size of the queue
	/// </summary>
	/// <returns>the size of the queue</returns>
	int GetSize()
	{
		return size;
	}

	/// <summary>
	/// Determines if the queue is empty or not.
	/// </summary>
	/// <returns>true if there is not data and false if there is data</returns>
	bool isEmpty()
	{
		if (size == 0)
		{
			return true;
		}
		return false;
	}

};