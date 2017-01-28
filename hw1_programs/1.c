#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

main()
{

printf("I am the parent process.\n");



int pid = fork();		//forking

if(pid == 0)
{
	// child process
	
	printf("My PID is: %d\n", getpid() );		//getpid()
	printf("My Parent's PID is: %d\n", getppid() );	// getppid()
	
	printf("I'm now going to call the ls command...\n");
	execl("/bin/ls" , "ls", "-l", (char *)0);	// child calls execl
	printf("I'm now going to exit (child)\n" );
	exit(0);		// child exits

}

if(pid > 0)
{



printf("Waiting for child....\n");

wait(pid);

printf("Child is done, I'm done too!\n");
exit(0);		//parent quits.
}



}
