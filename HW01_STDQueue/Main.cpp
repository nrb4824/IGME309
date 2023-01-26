#include "Main.h"
#include "Queue.h"
template <typename T> 
void Print(std::deque<T> a_queue)
{
	std::cout << "Begining of queue......." << std::endl;
	uint count = a_queue.size();
	for (uint i = 0; i < count; ++i)
	{
		std::cout << a_queue[i];
		std::cout << std::endl;
	}
	std::cout << "End of queue............" << std::endl;
}


/// <summary>
/// Tests for Queue.h. Creates queues of different types including the AlbertoClass
/// Pushes and Pops elements and prints out the queues.
/// </summary>
/// <param name=""></param>
/// <returns></returns>
int main(void)
{
	std::cout << "int Queue" << std::endl;
	Queue<int> intQueue;

	intQueue.Push(1);
	intQueue.Push(7);
	intQueue.Push(3);
	intQueue.Push(5);
	intQueue.Push(4);
	intQueue.Push(6);
	intQueue.Push(2);
	intQueue.Print();

	std::cout << std::endl << "Pop two values from the int Queue" << std::endl;

	intQueue.Pop();
	intQueue.Pop();
	intQueue.Print();

	std::cout << std::endl << "Char Queue" << std::endl;

	Queue<char> charQueue;
	charQueue.Push('v');
	charQueue.Push('b');
	charQueue.Push('l');
	charQueue.Push('d');
	charQueue.Print();

	std::cout << std::endl << "Pop two values from the char Queue" << std::endl;

	charQueue.Pop();
	charQueue.Pop();
	charQueue.Print();

	std::cout << std::endl << "Double Queue" << std::endl;

	Queue<double> dubQueue;
	dubQueue.Push(1.1);
	dubQueue.Push(5.5);
	dubQueue.Push(3.3);
	dubQueue.Push(4.4);
	dubQueue.Print();

	std::cout << std::endl << "Pop two values from the double Queue" << std::endl;

	dubQueue.Pop();
	dubQueue.Pop();
	dubQueue.Print();

	std::cout << std::endl << "Alberto Queue" << std::endl;

	Queue<AlbertoClass> AlbertoQueue;
	AlbertoQueue.Push(AlbertoClass(5));
	AlbertoQueue.Push(AlbertoClass(15));
	AlbertoQueue.Push(AlbertoClass(10));
	AlbertoQueue.Push(AlbertoClass(50));
	AlbertoQueue.Print();

	std::cout << std::endl << "Pop two values from the Alberto Queue" << std::endl;

	AlbertoQueue.Pop();
	AlbertoQueue.Pop();
	AlbertoQueue.Print();

	std::cout << std::endl << "Copy the int Queue" << std::endl;

	Queue<int> int2Queue = intQueue;
	int2Queue.Print();

	std::cout << std::endl << "Pop two values from the copied int Queue" << std::endl;

	int2Queue.Pop();
	int2Queue.Pop();
	int2Queue.Print();

	std::cout << std::endl << "Set copied int Queue equal to int Queue" << std::endl;

	int2Queue = intQueue;
	int2Queue.Print();

	std::cout << std::endl << "Press Enter to finish." << std::endl;
	
	getchar();
}
