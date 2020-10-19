#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

//struct to keep track of the data for each thread
struct HotPotato
{
	short hot;
	short gold;
	int threadID;
	int numThreads;
};

//global variable
struct HotPotato gameInfo[8];

//the list of instructions that the treads follow
void *threadJob(void *t)
{
	//casting and initializing variables
	int index = *((int *)t);
	int i;
	int randNum = 0;
	int stolenGold = 0;
	//infinite loop to sleep then check if there is a hot potato
	while(1)
	{
		sleep(2);
		//check for hot potato
		if (gameInfo[index].hot > 0)
		{
			//performs the process for hot potatos for however many hot potatos exist
			for (i = 0; i<gameInfo[index].hot;i++)
			{
				printf("%d: Ouch! I have the hot potato!\n",gameInfo[index].threadID);
				//subtracts one from the total potatos, creates a random number, and increments the random thread's
				//hot potato count
				gameInfo[index].hot = gameInfo[index].hot -1;
				randNum = rand() % gameInfo[index].numThreads;
				gameInfo[randNum].hot = gameInfo[randNum].hot +1;
			}//end of for
			//performs the process of gold stealing
			for (i =0; i<gameInfo[index].numThreads; i++)
			{
				//tests to see if the other threads have any gold and steals it
				if ( gameInfo[i].gold > 0 && i != index )
				{
					stolenGold=gameInfo[i].gold;
					gameInfo[i].gold = 0;
					gameInfo[index].gold = gameInfo[index].gold + stolenGold;
					stolenGold = 0;
				}//end of if
			}//end of for
		}//end of if
	}//end of while
}//end of threadJob

int main()
{
	
	//variable definitions	
	int i;
	char choice='n';
	int threadID = 0;
	int randomN = 0;
	int hold = 0;
	//max amount of threads 8
	pthread_t threads[8];
	
	//initializing the array of structs
	for(i=0; i<8; i++)
	{
		gameInfo[i].hot = 0;
		gameInfo[i].gold = 0;
		gameInfo[i].threadID = i;
		gameInfo[i].numThreads = 0;
	}//end of for
	
	//while loop that will continue looping until an 'x' is entered
	while(choice != 'x')
	{
		sleep(1);
		printf("T -thread; H -Hot Potato; G -Gold; x –exit\n");
		scanf(" %c",&choice);
		//series of if statements that perform actions based off of what the user inputs
		if (choice == 'T')
		{
			hold = threadID;
			//creates a thread and sends it the thread id and the instruction list
			pthread_create(&threads[threadID], NULL, threadJob,(void *) &hold);
			printf("Creating Thread #%d\n", threadID);
			threadID = threadID+1;
			//updates the number of threads that have been spawned for all the threads
			for(i =0; i<8; i++)
			{
				gameInfo[i].numThreads = threadID;
			}//end of for
		}//end of if
		else if(choice == 'H')
		{
			//creates a random number that complies with how many threads have been created 
			//and increments the hot potato variable of the randomly chosen struct
			randomN = rand() % threadID;
			gameInfo[randomN].hot = gameInfo[randomN].hot + 1;
			printf("Hot potato given to thread #%d\n",randomN);
			
		}//end of elseif
		else if(choice == 'G')
		{
			//creates a random number that complies with how many threads have been created
			//and increments the gold variable of the randomly chosen struct
			randomN = rand() % threadID;
			gameInfo[randomN].gold = gameInfo[randomN].gold + 1;
                        printf("Gold given to thread #%d\n",randomN);
		}	
	}//end of while loop
	//prints out the final game data after the while loop terminates
	for(i=0;i<threadID;i++)
	{
		printf("%d: %d/%d ", i, gameInfo[i].hot, gameInfo[i].gold);
	}//end of for
	printf("\n");
	return 0;
}//end of main
