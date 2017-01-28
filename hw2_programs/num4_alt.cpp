#include <cstdlib>
#include <iostream>
#include <fstream>
#include <unistd.h>


/***********************

Title: Homework 2, Problem 4
Class: CSCE 4600
Author: Alex Hollis

Special Instructions: N/A


/************************/

using namespace std;


main()
{


	fstream fs_p1, fs_p2;
	
	int pid;
	pid_t p1, p2;
	int n;
	int running = 1;
	int p1_count=0;
	int p2_count =0;
	
	
	while(running)
	{
		
		
		
		
		
		
		
		
		
			p1 = fork();
			
			if(p1 == 0)
			{
			//Process 1//
			//odd ints//

			fs_p1.open("F.txt", fstream::in);					// open F
			fs_p1 >> n;											// read seq #
			fs_p1.close();										// close f
			if(n % 2 > 0) {
				cout << "N= " << n << " PID: " << p1 << endl;		// output N & pid
				n++;												// increment n
				fs_p1.open("F.txt", fstream::out | fstream::trunc);  // open f
				fs_p1 << n;											// write n to f, truncates previous values, thereby flushing f
				fs_p1.close();
				p1_count++;
			}
			
			if(p1_count == 50) exit(0);
		
			}
			
			
			
			
			else	// process 2
			{
				p2 = getpid();
				//Process 2//
				//even ints//
				if(p2_count < 50)
				{
				fs_p2.open("F.txt", fstream::in);					// open F
				fs_p2 >> n;											// read seq #
				fs_p2.close();										// close f
				if(n % 2 == 0) {
					cout << "N= " << n << " PID: " << p2 << endl;		// output N & pid
					n++;												// increment n
					fs_p2.open("F.txt", fstream::out | fstream::trunc);  // open f
					fs_p2 << n;											// write n to f, truncates previous values, thereby flushing f
					fs_p2.close();
					p2_count++;
				}
				}
				if(p1_count == 50 && p2_count == 50) running = 0;
			}
			
			
		

		

		
		






	}






}