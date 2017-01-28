#include <cstdlib>
#include <iostream>
#include <random>		// C++11

/***********************

Title: Homework 2, Problem 3
Class: CSCE 4600
Author: Alex Hollis

Special Instructions: You must compile using C++11 >> "g++ -std=c++0x number3.cpp"


/************************/


using namespace std;

class Process
{
public:
	int id;
	int cycles;
	int memory;
	

};


main()
{

	/*SET NUMBER OF PROCESSES HERE*/
	int k = 10;
	/* Processes */
	
	
	default_random_engine generator;
	//using normal distribution to spread out values for cycles and memory//
	normal_distribution<double> distribution_cycles(6000, 2500);
	normal_distribution<double> distribution_mem(20.0, 24.75);
	
	Process arr[k];		// array of processes
	
	int cycles, memory;
	int id = 1;
	
	for(int i=0; i< k; i++)
	{
		arr[i].id = id;
		id++;
		while(1) {
			cycles = (int)distribution_cycles(generator);
			memory = (int)distribution_mem(generator);
			
			if(cycles >= 1000 && cycles <= 11000)
			{
				arr[i].cycles = cycles;
			}
			if(memory >= 1 && memory <= 100)
			{
				arr[i].memory = memory;
			}
			
			if(cycles >= 1000 && cycles <= 11000 && memory >= 1 && memory <= 100)
			{
				break;	// move on to next process
			}
			
			
			
		}

}

		cout << "Results: <id, cycles, memory_use> \n\n" << endl;
		for(int j=0; j < k; j++)
		{
		cout << "< " << arr[j].id <<", " << arr[j].cycles << ", " << arr[j].memory << " >" << endl;
		}




	}