//test

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdbool.h>


sem_t A;
sem_t B;


void *thread1(void *arg)
{

	// take control or wait on A
	printf("P1: Trying to take A....\n");
	sem_wait(&A);
	printf("P1: Took A - Success!\n");
	sleep(1);	// take a nap

	// try to take control of B
	printf("P1: Trying to take B....\n");
	sem_wait(&B);
	printf("P1: Took B - Success!\n");
	sleep(1);
	sem_post(&B);	// release B
	sem_post(&A); // release A

}

void *thread2(void *arg)
{

	// take control or wait on B
	printf("P2: Trying to take B....\n");
	sem_wait(&B);
	printf("P2: Took B - Success!\n");
	sleep(1);	// take a nap

	// try to take control of A
	printf("P2: Trying to take A....\n");
	sem_wait(&A);
	printf("P2: Took A - Success!\n");
	sleep(1);
	sem_post(&A);	// release A
	sem_post(&B); // release B


}

int main()
{

int r1, r2;

int i[2];

sem_init(&A, 0, 1); 		//init semaphore A as binary
sem_init(&B, 0, 1);			//init semaphore B as binary

pthread_t thread_1;			//declare thread1
pthread_t thread_2;			//declare thread2

i[0] = 0;		// arguments for the threads
i[1] = 1;		// .....

printf("The following program will show what a deadlock does to a system. \n The program will not quit on its own, you'll need to do so manually.\n\n\n");

r1 = pthread_create(&thread_1, NULL, thread1, (void *)&i[0]);
r2 = pthread_create(&thread_2, NULL, thread2, (void *)&i[1]);

pthread_join(thread_1, NULL);
pthread_join(thread_2, NULL);

sem_destroy(&A);
sem_destroy(&B);


return 0;
}
