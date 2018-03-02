#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <cstring>
#include <unistd.h>
#include <semaphore.h>			

using namespace std;

char P[9][4];							// Char Array of size 36 to store if the Passenger is carrying luggague
int Row=1, Col=1;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER, mutex2 = PTHREAD_MUTEX_INITIALIZER;

struct thread_data 						// Structure to pass Row and Column to the thread
{
   int  row,col;
   char lugguage;
};

thread_data lug_tag[37];				// Array for the Luggague tag
int tag_number=0;						

void Enqueue(thread_data *my_data)
{
	tag_number++;
	lug_tag[tag_number].col=my_data->col;
	lug_tag[tag_number].row=my_data->row;
	lug_tag[tag_number].lugguage=my_data->lugguage;
}

void Dequeue()
{
	while(tag_number > 0)
	{
		P[lug_tag[tag_number].row-1][lug_tag[tag_number].col-1]=lug_tag[tag_number].lugguage;
		tag_number--;	
	}
}

void AutomatedMachine(thread_data *my_data)
{
   sleep(1);
   my_data->row=Row;
   sleep(1);
   my_data->col=Col++;
   if(Col>4)													// Altering the row and column according to the seats in the bus
	{
        Col=Col%4;
        Row++;
    }
}
void *Passenger(void *threadarg)
{
   sleep(rand()%(2*60+1));                                // Sleeping for 0 to 2 minutes
   struct thread_data *my_data;
   my_data = (struct thread_data *) threadarg;
   pthread_mutex_lock(&mutex1);
	AutomatedMachine(my_data);
	pthread_mutex_unlock(&mutex1);
	sleep(rand()%(60+1));							 // Sleeping for upto 1 minute
	if(rand()%101 <= 20)
		my_data->lugguage='Y';      // Assigning Y if the passenger carries lugguage
  	else
        my_data->lugguage='N';		// Assigning N if the passenger does not carry lugguage
   
    pthread_mutex_lock(&mutex2);
	Enqueue(my_data);   
   pthread_mutex_unlock(&mutex2);
   pthread_exit(NULL);									// return NULL								
}

int main ()
{
	memset(P,'E',sizeof(P));							// Intialising the Char Array as 'E'
	
    int n;
    cin>>n;											   // Number of Passengers as n
    
   pthread_t threads[n];     
	
   void *status;													
   struct thread_data td[n];          
   
   int rc;       
   for(int i=0; i < n; i++ )
   {          

	  rc = pthread_create(&threads[i], NULL,Passenger, (void *)&td[i]);     // Creating Working Threads
      
	  if (rc)
	  {
         cout << "Error:unable to create thread," << rc << endl;
         exit(-1);
      }
   }
   
   for(int i=0; i < n; i++ )
   {
      rc = pthread_join(threads[i], &status);          				
       if (rc)
	   {
         cout << "Error:unable to join thread," << rc << endl;
         exit(-1);
      }
   }
    
	Dequeue();					// Driver reading all the tags 
	  int bag=0;
   	for(int i=0; i<9; i++)                      
   	{
        for(int j=0; j<4; j++)
		{
        	cout<<P[i][j];
        	if(P[i][j]=='Y')
            	bag++;											// Count the number of Bags
        }
        cout<<endl;
	}
	
	cout<<bag<<" passengers carry luggage. "<<endl;
	
	pthread_exit(NULL);
}
