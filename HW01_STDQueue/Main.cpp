#include "Main.h"
#include "Queue.h"
template <typename T> 
using namespace std;
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

int main(void)
{
//	std::deque<int> myQueue;
//	//Push elements
//	myQueue.push_back(0);
//	std::cout << "	just pushed +++++" << std::endl;
//	myQueue.push_back(1);
//	std::cout << "	just pushed +++++" << std::endl;
//	myQueue.push_back(2);
//	std::cout << "	just pushed +++++" << std::endl;
//	//Print queue
//	Print<int>(myQueue);
//	//pop an element
//	myQueue.pop_front();
//	std::cout << "		just popped -----" << std::endl;
//	//add elements
//	myQueue.push_back(3);
//	std::cout << "	just pushed +++++" << std::endl;
//	myQueue.push_back(4);
//	std::cout << "	just pushed +++++" << std::endl;
//	myQueue.push_back(5);
//	std::cout << "	just pushed +++++" << std::endl;
//	//Print queue
//	Print<int>(myQueue);
//	myQueue.pop_front();
//	std::cout << "		just popped -----" << std::endl;
//	myQueue.pop_front();
//	std::cout << "		just popped -----" << std::endl;
//	//Print queue
//	Print<int>(myQueue);
//	//clear queue
//	uint count = myQueue.size();
//	for (uint i = 0; i < count; ++i)
//	{
//		myQueue.pop_front();
//		std::cout << "		just popped -----" << std::endl;
//	}
//	//Print queue
//	Print<int>(myQueue);
//
//	//Complex Data usage
//	std::deque<Foo> myFooQueue;
//	for (uint i = 0; i < 10; ++i)
//	{
//		Foo foo(i);
//		myFooQueue.push_back(foo);
//		std::cout << "	just pushed +++++" << std::endl;
//		if (i % 2 == 0)
//		{
//			myFooQueue.pop_front();
//			std::cout << "		just popped -----" << std::endl;
//		}
//	}
//
//	std::deque<AlbertoClass> myAlbertoQueue;
//	for (uint i = 30; i < 40; ++i)
//	{
//		AlbertoClass alberto(i);
//		myAlbertoQueue.push_back(alberto);
//		std::cout << "	just pushed +++++" << std::endl;
//	}
//	std::cout << "	Content" << std::endl;
//	for (uint i = 0; i < 10; ++i)
//	{
//		std::cout << myAlbertoQueue[i] << std::endl;
//	}

	cout << "int Queue" << endl;
		Queue<int> intQueue;
	
		intQueue.Push(1);
		intQueue.Push(7);
		intQueue.Push(3);
		intQueue.Push(5);
		intQueue.Push(4);
		intQueue.Push(6);
		intQueue.Push(2);
		intQueue.Print();
	
		cout << endl << "Pop two values from the int Queue" << endl;
	
		intQueue.Pop();
		intQueue.Pop();
		intQueue.Print();
	
		cout << endl << "Char Queue" << endl;
	
		Queue<char> charQueue;
		charQueue.Push('v');
		charQueue.Push('b');
		charQueue.Push('l');
		charQueue.Push('d');
		charQueue.Print();
	
		cout << endl << "Pop two values from the char Queue" << endl;
	
		charQueue.Pop();
		charQueue.Pop();
		charQueue.Print();
	
		cout << endl << "Double Queue" << endl;
	
		Queue<double> dubQueue;
		dubQueue.Push(1.1);
		dubQueue.Push(5.5);
		dubQueue.Push(3.3);
		dubQueue.Push(4.4);
		dubQueue.Print();
	
		cout << endl << "Pop two values from the double Queue" << endl;
	
		dubQueue.Pop();
		dubQueue.Pop();
		dubQueue.Print();
	
		cout << endl << "Alberto Queue" << endl;
	
		Queue<AlbertoClass> AlbertoQueue;
		AlbertoQueue.Push(Alberto(5));
		AlbertoQueue.Push(Alberto(15));
		AlbertoQueue.Push(Alberto(10));
		AlbertoQueue.Push(Alberto(50));
		AlbertoQueue.Print();
	
		cout << endl << "Pop two values from the Alberto Queue" << endl;
	
		AlbertoQueue.Pop();
		AlbertoQueue.Pop();
		AlbertoQueue.Print();
	
		cout << endl << "Copy the int Queue" << endl;
	
		Queue<int> int2Queue = intQueue;
		int2Queue.Print();
	
		cout << endl << "Pop two values from the copied int Queue" << endl;
	
		int2Queue.Pop();
		int2Queue.Pop();
		int2Queue.Print();
	
		cout << endl << "Set copied int Queue equal to int Queue" << endl;
	
		int2Queue = intQueue;
		int2Queue.Print();

	std::cout << "Press Enter to finish." << std::endl;


	
	getchar();
}



///// <summary>
///// Tests for custom Queue.
///// Creates a int, char, and double Queue
///// Pushes data and pops data
///// Copies the int constructor
///// </summary>
///// <returns></returns>
//int main()
//{
//	cout << "int Queue" << endl;
//	Queue<int> intQueue;
//
//	intQueue.Push(1);
//	intQueue.Push(7);
//	intQueue.Push(3);
//	intQueue.Push(5);
//	intQueue.Push(4);
//	intQueue.Push(6);
//	intQueue.Push(2);
//	intQueue.Print();
//
//	cout << endl << "Pop two values from the int Queue" << endl;
//
//	intQueue.Pop();
//	intQueue.Pop();
//	intQueue.Print();
//
//	cout << endl << "Char Queue" << endl;
//
//	Queue<char> charQueue;
//	charQueue.Push('v');
//	charQueue.Push('b');
//	charQueue.Push('l');
//	charQueue.Push('d');
//	charQueue.Print();
//
//	cout << endl << "Pop two values from the char Queue" << endl;
//
//	charQueue.Pop();
//	charQueue.Pop();
//	charQueue.Print();
//
//	cout << endl << "Double Queue" << endl;
//
//	Queue<double> dubQueue;
//	dubQueue.Push(1.1);
//	dubQueue.Push(5.5);
//	dubQueue.Push(3.3);
//	dubQueue.Push(4.4);
//	dubQueue.Print();
//
//	cout << endl << "Pop two values from the double Queue" << endl;
//
//	dubQueue.Pop();
//	dubQueue.Pop();
//	dubQueue.Print();
//
//	cout << endl << "Alberto Queue" << endl;
//
//	Queue<AlbertoClass> AlbertoQueue;
//	AlbertoQueue.Push(Alberto(5));
//	AlbertoQueue.Push(Alberto(15));
//	AlbertoQueue.Push(Alberto(10));
//	AlbertoQueue.Push(Alberto(50));
//	AlbertoQueue.Print();
//
//	cout << endl << "Pop two values from the Alberto Queue" << endl;
//
//	AlbertoQueue.Pop();
//	AlbertoQueue.Pop();
//	AlbertoQueue.Print();
//
//	cout << endl << "Copy the int Queue" << endl;
//
//	Queue<int> int2Queue = intQueue;
//	int2Queue.Print();
//
//	cout << endl << "Pop two values from the copied int Queue" << endl;
//
//	int2Queue.Pop();
//	int2Queue.Pop();
//	int2Queue.Print();
//
//	cout << endl << "Set copied int Queue equal to int Queue" << endl;
//
//	int2Queue = intQueue;
//	int2Queue.Print();
//}