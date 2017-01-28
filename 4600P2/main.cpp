// Operating Systems 
// Project 2
// Connor Bey, Alexander Hollis, Bob Jack
// 5-5-15

// Compile with: g++  *.cpp process.cpp -std=c++0x
#include "stdafx.h"
#include <iostream>
#include <queue>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <random>
#include <cmath>
#include <chrono>
#include "process.h"

using namespace std;

// MemBlock struct used to designate memory address locations
struct MemBlock
{
	size_t nodeSize;
	bool full;
	int* ptr;
	struct MemBlock *parent;
	struct MemBlock *left;
	struct MemBlock *right;
};

// Memblock new pointer
struct MemBlock *newMemBlockNode(size_t s) 
{
	MemBlock *node = new MemBlock;
	node->nodeSize = s;
	node->full = false;
	node->ptr = NULL;
	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;

	return node;
}

void advanceCycles(deque<Pro*> &, int);
bool hasMEM(Pro*, int);
void printDQ(deque<Pro*>);
void runTrial(queue<Pro*>, deque<Pro*>, deque<Pro*>, int, int, int);
int roundUp(int value);
MemBlock *search(MemBlock*, int*);
int *findFit(MemBlock *&, int);
void printTree(MemBlock *);
void removeNode(MemBlock *);
MemBlock* search(MemBlock *node, int* data);
void my_free(int*);
void clear(MemBlock *node);
queue<Pro*> buildProcesses(int);
int *my_malloc(MemBlock* &, int);
void resetProcs(queue<Pro*>);

// This is used to control number of process created and stored.
const int PRO_COUNT = 50;
const int TEST_SIZE = 10;		// Number of test Iterations
const int MEM_CAP = roundUp(10485760);  // 10 MB Cap
const int MEM_CAP2 = roundUp(10485760/2);  // Divide by 2 for 50% 
const int MEM_CAP3 = roundUp(1048576);   // Divide by 10 for 10%
const int MEM_MAX = roundUp(10485760/16);	  //Upper bounds must fall under cap
const int MEM_MAX2 = roundUp(10485760/32);	  //Upper bounds must fall under cap
const int MEM_MAX3 = roundUp(10485760/64);	  //Upper bounds must fall under cap
const int MEM_MIN = roundUp(64);		
const int CYCLE_MIN = 10000;	// Let these represent cycles in thousands
const int CYCLE_MAX = 200000;  // Let these represent cycles in thousands
static int memTotal = 0; // In bytes
const int cycleTick = 50;	// Number of ticks per cycle advance
bool SYS_ACTIVE = false;		// Flag used if system is active
MemBlock *root;			// Used as the root node of Mem Block

int main(int argc, char* argv[])
{
	double results[TEST_SIZE][2];
	deque<Pro*> mySystem;	// System containing processes
	deque<Pro*> waitingQ;	// Waiting Queue for processes
	double total1;			// Value for time total of Malloc Trial
	double total2;			// Value for time total of My_Malloc Trial
	queue<Pro*> initProQ1, initProQ2;

	// -------Comment from below out to just see timed trial data-------------------//

	// Show how the program works 
	cout << "First Test: Normal Constraints\n";
	// Normal Constraints
	initProQ1 = buildProcesses(MEM_MAX2);
	initProQ2 = initProQ1;
	cout << "Malloc\n";
	runTrial(initProQ1, mySystem, waitingQ, MEM_CAP, 0, 1);
	cout << "\n";

	// Reset Cycles
	resetProcs(initProQ2);

	// Allocated first malloc call to reserve memory
	int *myBlock1 = (int*)malloc(MEM_CAP);
	root = newMemBlockNode(MEM_CAP);
	root->ptr = myBlock1;
	cout << "my_malloc\n";
	runTrial(initProQ1, mySystem, waitingQ, MEM_CAP, 1, 1);
	clear(root); root = NULL; free(myBlock1);
	myBlock1 = 0;
	cout << "\n\n" << endl;

	
	cout << "Second Test: 50% Constraints\n";
	// 50% Constraints
	initProQ1 = buildProcesses(MEM_MAX2);
	initProQ2 = initProQ1;
	cout << "Malloc\n";
	runTrial(initProQ1, mySystem, waitingQ, MEM_CAP2, 0, 1);
	cout << "\n";

	// Reset Cycles
	resetProcs(initProQ2);

	myBlock1 = (int*)malloc(MEM_CAP2);
	root = newMemBlockNode(MEM_CAP2);
	root->ptr = myBlock1;
	cout << "my_malloc\n";
	runTrial(initProQ1, mySystem, waitingQ, MEM_CAP2, 1, 1);
	clear(root); root = NULL; free(myBlock1);
	myBlock1 = 0;
	cout << "\n\n" << endl;
	
	cout << "Third Test: 10% Constraints\n";
	// 10% Constraints
	initProQ1 = buildProcesses(MEM_MAX2);
	initProQ2 = initProQ1;
	cout << "Malloc\n";
	runTrial(initProQ1, mySystem, waitingQ, MEM_CAP3, 0, 1);
	cout << "\n";

	// Reset Cycles
	resetProcs(initProQ2);

	myBlock1 = (int*)malloc(MEM_CAP3);
	root = newMemBlockNode(MEM_CAP3);
	root->ptr = myBlock1;
	cout << "my_malloc\n";
	runTrial(initProQ1, mySystem, waitingQ, MEM_CAP3, 1, 1);
	clear(root); root = NULL; free(myBlock1);
	myBlock1 = 0;
	cout << "\n\n" << endl;

	
	//------------End Comment out above for Just time trial data-----------//

	int memStyle;
	// Run three tests with Normal, 50%, and 10% constraits
	for(int j = 0; j < 3; ++j)
	{
		// Specifify constrain modifiers for Normal run
		if(j == 0)
		{
			memStyle = MEM_CAP;
			cout << "Starting Timing Trials with Normal Constraints:\n";
		}
		// Specify constraint modifiers for 50% run
		else if(j == 1)
		{
			memStyle = MEM_CAP2;
			cout << "Starting Timing Trials with 50% Constraints:\n";
		}
		// Specify constraint modifiers for 10% run
		else
		{
			memStyle = MEM_CAP3;
			cout << "Starting Timing Trials with 10% Constraints:\n";
		}
		total1 = 0.0;
		total2 = 0.0;
		// Loop to run trials on Malloc and my_malloc calls
		for(int i = 0; i < TEST_SIZE; ++i)
		{
			// Build process queue to bea used in each distinct trial
		
			initProQ1 = buildProcesses(MEM_MAX);
			initProQ2 = initProQ1;
			cout << "Trial " << i+1 << endl;
			//--------- Malloc Trial--------------//
			auto startTimer = std::chrono::high_resolution_clock::now();
			runTrial(initProQ1, mySystem, waitingQ, memStyle, 0, 0);
			auto endTimer = std::chrono::high_resolution_clock::now();  
			auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(endTimer - startTimer).count();
			results[i][0] = (double)diff;

			// Reset Cycles
			resetProcs(initProQ2);

			//---------my_malloc Trial------------//
			startTimer = std::chrono::high_resolution_clock::now();

			// Allocated first malloc call to reserve memory
			int *myBlock = (int*)malloc(memStyle);
			root = newMemBlockNode(memStyle);
			root->ptr = myBlock;

			runTrial(initProQ2, mySystem, waitingQ, memStyle, 1, 0);
		
			// Clean up
			clear(root); root = NULL; free(myBlock);

			endTimer = std::chrono::high_resolution_clock::now();  
			diff = std::chrono::duration_cast<std::chrono::milliseconds>(endTimer - startTimer).count();
			results[i][1] = (double)diff;
		}
		// Print Results
		cout << "\nResults:\n";
		cout << "Run" << "\tMalloc" << "\tMy_Malloc" << endl;
		for(int i = 0; i < TEST_SIZE; ++i)
		{
			cout << i + 1 << "\t" << results[i][0] << "\t" << results[i][1] << endl;
			total1 += results[i][0];
			total2 += results[i][1];
		}
		cout << "\nAverage for Malloc = " << total1/(double)TEST_SIZE << " milliseconds " << endl;
		cout << "Average for my_malloc = " << total2/(double)TEST_SIZE << " milliseconds " << endl;
		cout << endl << endl;
	}

	return 0;
}


// My Malloc replaces malloc in the trials. It simply just allocates the memory
// to the process. All of the work is done in the function findFit()
int *my_malloc(MemBlock* &root, int size)
{
	int *tempAddress = NULL;
	tempAddress = findFit(root, size);
	return tempAddress;
}


// Frees the pointer address location reserved by the process
void my_free(int* address) 
{
	MemBlock *temp = NULL;
	temp = search(root, address);
	if(temp)
		removeNode(temp);
}


// Looks through the tree for a match for a requested memory block size.
// A block will be split if there is room. When a match is found the 
// memory is assigned. This is based on First Fit assigning
int *findFit(MemBlock *&mem, int size)
{
	if(!mem) return 0;

	// Is if full?
	if(mem->full)
		return 0;
	else
	{
		// Does it fit?
		if(mem->nodeSize == size && (mem->left == 0 && mem->right == 0))
		{
			mem->full = true;
			return mem->ptr;
		}
		else 
		{
			int newSize = mem->nodeSize/2;
			if(newSize < size)
				return 0;
			if (mem->left == 0 && mem->right == 0 && newSize >= 64)
			{
				// Split the node
				mem->left = newMemBlockNode(newSize);
				mem->left->parent = mem;
				mem->left->ptr = mem->ptr;

				mem->right = newMemBlockNode(newSize);
				mem->right->parent = mem;
				mem->right->ptr = mem->ptr + newSize/4; // (Compensate for (int*))
			}
			// Recursivly run again on left and right
			int *left = findFit(mem->left, size);

			if(left)
				return left;
			else
				return (findFit(mem->right, size));
		}
	}
}


// Resets the process cycles after each run
void resetProcs(queue<Pro*>inQ)
{
	queue<Pro*> tempQ = inQ;
	// Reset Cycles
	while(!tempQ.empty())
	{
		Pro *temp = tempQ.front();
		tempQ.pop();
		temp->resetCycles();
	}
}


// Returns the MemBlock location of a address values searched for.  Recursively searches 
// through the tree
MemBlock* search(MemBlock *node, int* data)
{
	// Return null if nothing has been found
	if(node == NULL) return NULL;

	// If there is a match and this node is a leaf, return the leaf address
	if(node->ptr == data && node->full)
		return node;
	// If the searching value falls in a node ptr range, then head in that direction
	// The value of 8 is used from the /4 from int* and /2 from node split
	if(data < node->ptr + node->nodeSize/8)
		return search(node->left, data);
	else 
		return search(node->right, data);
}


// Clears the tree completely. From //http://www.bogotobogo.com/cplusplus/binarytree.php
void clear(MemBlock *node)
{
	if(node != NULL)
	{
		clear(node->left);
		clear(node->right);
		delete node;
	}
}


// When a node is chosen to be removed, basically, the full flag is changed to false, 
// Then a coalescing check is run on the parent node and combined if needed
void removeNode(MemBlock * node)
{
	if(node == NULL)
		return;

	// Set the node to empty
	node->full = false;
	
	if(node->parent)
	{
		// Assign variables for Coalescence check
		MemBlock *tempParent = node->parent;
		MemBlock *buddy;
		if(node == tempParent->left)
			buddy = tempParent->right;
		else
			buddy = tempParent->left;

		// Check if Coalescence is needed 
		if(!buddy->full)
		{
			tempParent->left = NULL;
			tempParent->right = NULL;
			delete node;
			delete buddy;
		}
	}
}


// A Tree printing version used for debugging modified from http://www.bogotobogo.com/cplusplus/binarytree.php
void printTreeAll(struct MemBlock *node)
{
	if(node == NULL) return;
	cout << "\nNODE: " << node << "  ----- Size: " << node->nodeSize << endl;
	cout << "Full is " << node->full << endl;
	cout << "Node Address is " << node << endl;
	cout << "PTR is " << node->ptr << endl;
	cout << "Parent: " << node->parent << "  Left: " << node->left << "   Right: " << node->right << endl;
	printTreeAll(node->left);
	printTreeAll(node->right);
}


// A different version of print used for debugging
void printTree(MemBlock *node)
{
	if(node == NULL) return;

	if(node->full || (node->left == 0 && node->right == 0))
	{
		cout << "Node: " << node << " contains " << node->ptr << " Size: " << node->nodeSize;
		if(node->full)
			cout << "--- FILLED";
		cout <<  endl;
	}
	if(node->left)
		printTree(node->left);
	if(node->right)
		printTree(node->right);
}


// Takes a value and rounds up to the next power of 2.
// From http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
int roundUp(int value)
{
	value--;
	value |= value >> 1;
	value |= value >> 2;
	value |= value >> 4;
	value |= value >> 8;
	value |= value >> 16;
	return ++value;
}


// Process builder. It will generate porcesses and place in a queue for program use.
queue<Pro*> buildProcesses(int mem_cap)
{
	queue<Pro*> initProQ;
	int memory, cycles;

	// Create the number generator used by c++ 11
	default_random_engine Gen((unsigned)time(NULL));
	normal_distribution<double> cycleDist((CYCLE_MAX+CYCLE_MIN)/2, (CYCLE_MAX-CYCLE_MIN)/4.0);
	normal_distribution<double> memoryDist((mem_cap + MEM_MIN)/2, (mem_cap-MEM_MIN) / 4.0);

	// Loop through the number of processes to be created.
	for (int i = 0; i < PRO_COUNT; ++i)
	{
		bool validCycle = false;
		bool validMemory = false;
		
		// Loop to ensure random values will fall in range desired
		while(!validCycle || ! validMemory)
		{
			// Check for cycle validity
			if(!validCycle)
			{
				// Draw number
				cycles = (int)cycleDist(Gen);

				// If invalid, draw again, else continue
				if(cycles < CYCLE_MIN || cycles > CYCLE_MAX)
					validCycle = false;
				else
					validCycle = true;
			}
			// Check for memory validity
			if(!validMemory)
			{
				// Draw number
				memory = (int)memoryDist(Gen);

				// If invalid, draw again, else continue
				if(memory < MEM_MIN || memory > mem_cap)
					validMemory = false;
				else
					validMemory = true;
			}
		}
		//cout << memory;
		//cout << " Round  " << roundUp(memory) << endl;
		memTotal += memory;
		// Create the process and push to the queue
		Pro *temp = new Pro(cycles, memory);
		initProQ.push(temp);
	}
	//cout << "Total Mem: " << memTotal << endl;
	return initProQ;
}


// Conducts a trial run based on which version of malloc is to be used. This is modified from project 1.
void runTrial(queue<Pro*> initProQ, deque<Pro*> mySystem, deque<Pro*> waitingQ, int mem_cap, int type, int print)
{
	memTotal = 0;

	// Get the first process if there is an Initial Queue, Activate the System
	Pro *currentProcess = 0;
	if(!initProQ.empty())
	{
		currentProcess = initProQ.front();
		initProQ.pop();
		SYS_ACTIVE = true;
	}
	// Start of loop through until all the processes have started
	while(SYS_ACTIVE)
	{
		// If there is not process held and there is more in the Init Q, then get one.
		if(currentProcess == 0 && !initProQ.empty())
		{
			currentProcess = initProQ.front();
			initProQ.pop();
		}
		//if(currentProcess)
			//currentProcess->printData();

		// Process Waiting Queue will take priority over Init Queue
		if(!waitingQ.empty())
		{
			int size = waitingQ.size();
			int index = 0;
			bool foundFit = false;
			bool problem = false;

			// Loop through every process in waiting Queue
			while(!foundFit && index < size)
			{
				// If the index process can fit into the avaliable memory space
				// Then place it into the system, and put the current process at the end of waiting Q
				if(type)
				{
					int memsize = roundUp(waitingQ.at(index)->getMemory());
					int *tempFit;

					tempFit = my_malloc(root, memsize);
				
					if(tempFit) // && hasMEM(waitingQ.at(index)))
					{
						// Update current System memory
						memTotal += roundUp(waitingQ.at(index)->getMemory());

						// Move process into system
						mySystem.push_back(waitingQ.at(index));

						// Update Mem Allocation in Process
						waitingQ.at(index)->setMemAlloc(tempFit);

						if(print)
							cout << "Allocated Mem Address = " << waitingQ.at(index)->getMemAlloc() << endl;
						// Erase process from waiting Q
						waitingQ.erase(waitingQ.begin() + index);

						// Place current process on the waiting Q, if one exists
						if(currentProcess)
						{
							waitingQ.push_back(currentProcess);
							currentProcess = 0;
						}
						// Set to true to break loop
						foundFit = true;
					}
				}
				else
				{
				//	cout << "Memtotal = " << memTotal << endl;
				//	cout << "Mem " << waitingQ.at(index)->getMemory()<< endl;
					//cout << "Has mem " << hasMEM(waitingQ.at(index), mem_cap) << endl;
					if(hasMEM(waitingQ.at(index), mem_cap))
					{
						// Update current System memory
						memTotal += roundUp(waitingQ.at(index)->getMemory());

						// Move process into system
						mySystem.push_back(waitingQ.at(index));

						// Allocate the required memory via malloc
						int* tempAlloc = (int*) malloc(roundUp(waitingQ.at(index)->getMemory()));
						waitingQ.at(index)->setMemAlloc(tempAlloc);

						if(print)
							cout << "Allocated Mem Address = " << waitingQ.at(index)->getMemAlloc() << endl;					
						
						// Erase process from waiting Q
						waitingQ.erase(waitingQ.begin() + index);

						// Place current process on the waiting Q, if one exists
						if(currentProcess)
						{
							waitingQ.push_back(currentProcess);
							currentProcess = 0;
						}
						// Set to true to break loop
						foundFit = true;
					}
				}
				// Increase index
				index++;			
			}
		}
		// Check if there is room for memory in the system for process from Init Q
		if(currentProcess)
		{
			if(type) // && hasMEM(currentProcess))
			{
				int memsize = roundUp(currentProcess->getMemory());
				int *tempFit = NULL;
				tempFit = findFit(root, memsize);
				if(tempFit)
				{
					// Update current System Memory
					memTotal += roundUp(currentProcess->getMemory());
					//currentProcess->printData();
					// Move process into system
					mySystem.push_back(currentProcess);
					currentProcess->setMemAlloc(tempFit);

					if(print)
						cout << "Allocated Mem Address = " << currentProcess->getMemAlloc() << endl;

					currentProcess = 0;
				}
			}
			else
			{
					//cout << "Memtotal = " << memTotal << endl;
					//cout << "Mem " << currentProcess->getMemory()<< endl;
					//cout << "Has mem " << hasMEM(currentProcess, mem_cap) << endl;
				if(hasMEM(currentProcess, mem_cap))
				{
					// Update current System Memory
					memTotal += roundUp(currentProcess->getMemory());
					//currentProcess->printData();

					// Move process into system
					mySystem.push_back(currentProcess);
			
					// Allocate the required memory via malloc
					int* tempAlloc = (int*) malloc(roundUp(currentProcess->getMemory()));
					currentProcess->setMemAlloc(tempAlloc);

					if(print)
						cout << "Allocated Mem Address = " << currentProcess->getMemAlloc() << endl;

					currentProcess = 0;
				}
			}
		}
		// If the process hasnt been used, place on waiting Q, and zero
		if(currentProcess)
		{
			waitingQ.push_back(currentProcess);
			currentProcess = 0;
		}
		// Advance all the processes in the system by designated cycle step
		//if(currentProcess) currentProcess->getID();

		if(!mySystem.empty())
			advanceCycles(mySystem, type);
	
		//printTreeAll(root);
		//printTree(root);

		// Check if all queus are empy, if so, deactivate system
		if(mySystem.empty() && initProQ.empty() && waitingQ.empty())
			SYS_ACTIVE = false;
	}
}


// A process enters the system every 50 cycles, so advance cycle by increments of 50
void advanceCycles(deque<Pro*> &sys, int type)
{
	int size = sys.size();
	vector<int> markedForDelete;

	// Loop through subtracting from each each cycle
	for(int i = 0; i < size; ++i)
	{
		sys.at(i)->subCycles(cycleTick);

		// If the process is complete, mark it for deletion
		if(sys.at(i)->getCycles() == 0)
		{	
			markedForDelete.push_back(i);
		}
	}
	//printDQ(sys);

	// Establish deletion vector size
	int sizeV = markedForDelete.size();

	// If there marked queue exists, go through and eliminate each process
	if(!markedForDelete.empty())
	{
		// To safely remove from deque, start from end and go to front
		for(int j = sizeV-1; j >= 0; --j)
		{
			int d = markedForDelete[j];
			// Extract a temp
			Pro *temp = sys.at(d);
			//cout << "Deleting : " << temp << endl;
			// Update the System memory with remove process
			memTotal -= roundUp(temp->getMemory());
			// Remove from System Q
			sys.erase(sys.begin() + d);
	
			// Free the Memory
			if(!type)
			{
				free(temp->getMemAlloc());
				temp->setMemAlloc(0);	
			}
			else   // Free with My Malloc
			{
				my_free(temp->getMemAlloc());
				temp->setMemAlloc(0);			
			
				// Permenantly remove process from heap
				delete temp;
			}
		}
	}
	// Clear vector to be safe
	markedForDelete.clear(); 
}


// Will check the system mem total to see if the current process will fit
// Returns true if yes, false if no.
bool hasMEM(Pro* pro, int mem_cap)
{
	if(mem_cap - memTotal - roundUp(pro->getMemory()) >= 0 )
		return true;
	else
		return false;
}


// Prints the data for each process in a given queue
void printDQ(deque<Pro*> pro)
{
	int index = 0;
	int size = pro.size();
	for(; index < size; ++index)
	{
		pro.at(index)->printData();
	}
}