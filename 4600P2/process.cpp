// file: process.cpp
// Operating Systems 
// Project 1 - Question 3
// 23-24-15
// Implementation for Process class.
#include "stdafx.h"
#include "process.h"


// Value initialization for class ID counter. Starts at 10k
long long Pro::p_ID_counter = 10000LL;

// Default Constructor. Takes no arguments. It will assign and 
// increment the program ID counter, but initialize other values to 0.
Pro::Pro()
{
	processID = p_ID_counter++;
	totalCycles = 0;
	cycles = 0;
	memory = 0;
	memAllocation = 0;
}

// Standard Constructor. Accepts the values for cycles and memory
// and assigns them to their variables. The program ID is also 
// assigned and incremented.
Pro::Pro(int c, int m)
{
	processID = p_ID_counter++;
	totalCycles = c;
	cycles = c;
	memory = m;
	memAllocation = 0;
}

// This function outputs the process data.
void Pro::printData()
{
	std::cout << "ID: " <<  std::setw(6) << processID << ", "
	<< std::setw(5) << cycles << " cycles, " << 
	std::setw(3) << memory << " bytes of memory" << std::endl;
}

// Subtracts the cycle value from the cycle count
void Pro::subCycles(int value)
{
	if(cycles > 0)
	{
		cycles -= value;
		if(cycles < 0)
		{
			cycles = 0;
		}
	}
}

// Resets the cycle counts to original
void Pro::resetCycles()
{
	cycles = totalCycles;
}