#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <cstring>
#include <unistd.h>
#include<time.h>
#include <semaphore.h>			
#include<ctime>
using namespace std;

char P[9][4];							// Char Array of size 36 to store if the Passenger is carrying luggague
int Row=1, Col=1, total_P, remain_P, seated=0;		// Global variables to keep track of passengers
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t busfull, startbus;

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
	if(tag_number==36)
		tag_number==0;
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
   
    if(Row == 9 && Col == 5)			// Reset Machine
    {
    	Row=1;
    	Col=1;
	}    
   
   if(Col>4)							// Altering the row and column according to the seats in the bus
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
   pthread_mutex_lock(&mutex);
	AutomatedMachine(my_data);
	pthread_mutex_unlock(&mutex);
	sem_wait(&busfull);
	sleep(rand()%(60+1));							 // Sleeping for upto 1 minute
	if(rand()%101 <= 20)
		my_data->lugguage='Y';      // Assigning Y if the passenger carries lugguage
  	else
        my_data->lugguage='N';		// Assigning N if the passenger does not carry lugguage
   
    pthread_mutex_lock(&mutex);
	Enqueue(my_data);   
   pthread_mutex_unlock(&mutex);
   pthread_mutex_lock(&mutex);
	seated++;
	remain_P--;
   pthread_mutex_unlock(&mutex);
    
    if(seated == 36)				
	{
        pthread_mutex_lock(&mutex);
            seated = 0;
            sem_post(&startbus);
        pthread_mutex_unlock(&mutex);
    }
   pthread_exit(NULL);															
}

void *coach(void *threadarg)
{
	int *ID = (int *)threadarg;
	
	double start_T = 0, leave_T = 0;
	time_t reached, left_T;
	time(&reached);
	
	while(remain_P>0)
	{
		for(int i=0; i<36; i++)
            sem_post(&busfull);
        
        sem_wait(&startbus);
        time(&left_T);

        leave_T =start_T+difftime(left_T,reached);

        cout<<"Coach arrives at time "<<start_T<<"."<<endl<<"Coach departs at time "<<leave_T<<"."<<endl;
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
		memset(P,'E',sizeof(P));
		sleep(60*10);
		time(&reached);
        start_T = leave_T + difftime(reached, left_T);
	}
	 pthread_exit(NULL);
}
int main ()
{
	memset(P,'E',sizeof(P));							// Intialising the Char Array as 'E'
	sem_init(&busfull, 0, 0);
    sem_init(&startbus, 0, 0);
	
    cin>>total_P;											   // Number of Passengers as total_P
	remain_P=total_P;
   pthread_t threads[total_P];     
	
   void *status;													
   struct thread_data td[total_P];          
   
   int driverID = 101, rc;

    pthread_t driver;
    rc = pthread_create(&driver, NULL, coach, (void *)&driverID);

         
   for(int i=0; i < total_P; i++ )
   {          

	  rc = pthread_create(&threads[i], NULL,Passenger, (void *)&td[i]);     // Creating Working Threads
      
	  if (rc)
	  {
         cout << "Error:unable to create thread," << rc << endl;
         exit(-1);
      }
   }
   
   for(int i=0; i < total_P; i++ )
   {
      rc = pthread_join(threads[i], &status);          				
       if (rc)
	   {
         cout << "Error:unable to join thread," << rc << endl;
         exit(-1);
      }
   }
    
	sem_destroy(&busfull);
    sem_destroy(&startbus);
	pthread_exit(NULL);
}
