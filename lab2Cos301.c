#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

//declare semaphores as global values
sem_t numberEmptyB;
sem_t numberFullB;
sem_t mutex;
//the buffer sets the max number of producers to 8
int buffer[8];


//function that the consumers execute
void *jobCons(void *t)
{
	int index = *((int *)t);
	printf("Hi, I am consumer %d\n", index);
	//get the timespec struct set up for nanosleep
	struct timespec sleepTime, thisOneDoesntMatter;
	sleepTime.tv_sec = 0;
	int i;
	int indexFull;
	//infinite loop
	while(1)
	{	
		//set the sleep time to a random number from
		//100000 to 100 nanoseconds
		sleepTime.tv_nsec = rand()%100000 +100;
		//execute the sleep
		nanosleep(&sleepTime, &thisOneDoesntMatter);
		//decrement the number of full buffers
		sem_wait(&numberFullB);
		//grab the lock
		sem_wait(&mutex);
		//printf("lock grabed by consum %d\n", index);
		//sleep(2);
		//loop through the buffer to look for the index
		//where there is a product/number>0
		for(i=0;i<8;i++)
		{
			if(buffer[i]>0)
			{
				//once that index is found, store
				//the data and stop looping
				indexFull=i;
				break;
			}//end of if
		}//end of for
		//display who is consuming what
		printf("\t\t\t%c: B[%d]=%d\n", (char)(65+index), indexFull, buffer[indexFull]);
		//clearn the data in the buffer
		buffer[indexFull]=0;
		//let go of the lock
		sem_post(&mutex);
		//printf("lock releaced by consum %d\n", index);
		//increment the semaphore that keeps track of
		//how many empty buffer slots there are 
		sem_post(&numberEmptyB);	
	}//end of while
}//end of jobCons

//the function that the procucer threads execute
void *jobProd(void *t)
{
	int index = *((int *)t);
	int i;
	printf("Hi, I am producer %d\n", index);

	//the multiple of 1000 that identifies the producer's products
	int prodNum = (index + 1)*1000;

	//infastructure for nanosleep
	struct timespec sleepTime, thisOneDoesntMatter;
	sleepTime.tv_sec = 0;
	//infinite loop
	while(1)
	{
		//the random amount of nanoseconds the producer will sleep
		sleepTime.tv_nsec = rand()%100000 + 100;
		//execute the sleep
		nanosleep(&sleepTime, &thisOneDoesntMatter);
		//decrement the semaphore that keeps track of how many empty buffers there are
		sem_wait(&numberEmptyB);
		//grab the lock
		sem_wait(&mutex);
		//printf("lock grabed by prod %d\n",index);
		//sleep(2);
		//print out production statement
		printf("Producer %d: %d\n",index,prodNum);
		//search for an empty buffer and fill it, then break the loop
		for(i=0;i<8;i++)
		{
			if(buffer[i]==0)
			{
				buffer[i] = prodNum;
				break;
			}//end of if

		}//end of for
		//increment the production number or product identifier
		prodNum++;
		//release the lock
		sem_post(&mutex);
		//printf("lock releaced by prod %d\n",index);
		//increment the semaphore that keeps track of the number of full buffers
		sem_post(&numberFullB);
	}//end of while
}//end of jobProd


int main(int argc, char **argv)
{
	//change the chars in the char array to ints and throw them into an array to keep
	//track of how many producers and consumers to be made
	int numThreads[] = {atoi(argv[1]),atoi(argv[2])};
//	printf("%d", numThreads[0]);
//	printf("%d", numThreads[1]);

	
	//declare the pthreads for consumers and producers
	pthread_t consumer[numThreads[0]];
	pthread_t producer[numThreads[1]];

	//declare variables that will be used later
	int i;
	int j;
	int holdCons[numThreads[0]];
	int holdProd[numThreads[1]];
	int hold2;
	int hold3;
	int numberOfBuffers = 4;

	//initialize the buffer
	for(i=0;i<8;i++)
	{
		buffer[i]=0;
	}//end of for	

	//initialize the semaphores
	sem_init(&numberEmptyB, 0, numberOfBuffers);
	sem_init(&numberFullB, 0, 0);
	sem_init(&mutex, 0, 1);

	//initialize arrays of thread ids for consumers
	for(i=0;i<numThreads[0];i++)
	{
		holdCons[i] = i;
	}//end of for

	//initialize the array of thread ids for producers
	for(i=0;i<numThreads[1];i++)
	{
		holdProd[i] = i;
	}//end of for

	//nested for loop that creates the consumer and producer threads
	//first loop(of the outer for loop) makes the consumers
	//second loop(of the outer for loop) makes the producers
	for(i=0;i<2; i++)
	{
		for(j=0;j<numThreads[i];j++)
		{
			if(i==0)
			{
	
				printf("Creating consumer %d\n", hold2);
				pthread_create(&consumer[j], NULL, jobCons,(void *) &holdCons[j]);
	
			}//end of if
			else
			{
			
				printf("Creating producer %d\n", hold3);
				pthread_create(&producer[j], NULL, jobProd, (void *) &holdProd[j]);
				
			}//end of else

		}//end of for
	}//end of for

	//inifinite loop to gaurentee that the pthreads don't get killed too early
	while(1)
	{
		printf("");
	}//end of while
	return 0;
}//end of main
