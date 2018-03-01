#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <cstring>
#include <unistd.h>

using namespace std;

char P[9][4];							// Char Array of size 36 to store if the Passenger is carrying luggague


struct thread_data 						// Structure to pass Row and Column to the thread
{
   int  row,col;
};

void *Passenger(void *threadarg)
{
   sleep(rand()%(10*60+1));                                // Sleeping for 0 to 10 minutes
   
   struct thread_data *my_data;

   my_data = (struct thread_data *) threadarg;
	
	if(rand()%101 <= 20)
		P[my_data->row - 1][my_data->col - 1]='Y';      // Assigning Y if the passenger carries lugguage
  	else
        P[my_data->row - 1][my_data->col - 1]='N';		// Assigning N if the passenger does not carry lugguage
   
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
   int i,row=1,col=1;
       
   for( i=0; i < n; i++ )
   {          
      if(col>4)													// Altering the row and column according to the seats in the bus
      {
         col=col%4;
         row++;
      }
      
      td[i].row =row;      											// Pass row and column 
      td[i].col=col++;  
      
	  rc = pthread_create(&threads[i], NULL,Passenger, (void *)&td[i]);     // Creating Working Threads
      
	  if (rc)
	  {
         cout << "Error:unable to create thread," << rc << endl;
         exit(-1);
      }
   }
   
   for( i=0; i < n; i++ )
   {
      rc = pthread_join(threads[i], &status);          				
       if (rc)
	   {
         cout << "Error:unable to join thread," << rc << endl;
         exit(-1);
      }
   }
      
	  int bag=0;
   	for(i=0; i<9; i++)                      
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
