// file: process.h
// Operating Systems 
// Project 1
// 3-24-15
// Header file for Process class.

#ifndef PRO_H
#define PRO_H

#include <iostream>
#include <iomanip>

class Pro
{
private:
	long long processID;
	int totalCycles;
	int cycles;
	int memory;
	static long long p_ID_counter;	
	int *memAllocation;
	
public:
	// Constructor #1
	Pro();
	// Constructor #2
	Pro(int c, int m);
	
	long long getID()
	{ return processID; };
	int getTotalCycles()
	{ return totalCycles; };
	int getCycles()
	{ return cycles; };
	int getMemory()
	{ return memory; };
	void printData();
	void subCycles(int value);
	int *getMemAlloc()
	{ return memAllocation; };
	void setMemAlloc(int* ma)
	{ memAllocation = ma; };
	void resetCycles();
};	
#endif