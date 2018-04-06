#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include <queue>
#include <vector>

using namespace std;

queue<int> leftToMake, leftToPack;

struct thread_data {
   int makingId, packingId;
};

int hotdogs;
vector<thread_data> data;

int M = 30, P = 25;

void *make(void *t) {
   if(leftToMake.empty()){
      pthread_exit(NULL);
   }

   long machineId = (long)t;
   int hotDogId = leftToMake.front();
   leftToMake.pop();

   sleep(rand()%3+4);
   sleep(rand()%2+1);
   data[hotDogId-1].makingId = machineId;
   leftToPack.push(hotDogId);
}

void *pack(void *t) {

   if(leftToPack.empty()){
      pthread_exit(NULL);
   }

   long machineId = (long)t;
   int hotDogId = leftToPack.front();
   leftToPack.pop();

   sleep(rand()%2+1);
   sleep(rand()%2+1);

   // Write Log to file

   sleep(rand()%2+1);
   data[hotDogId-1].packingId = machineId;
 
}

int main () {
     hotdogs=5;
     for(int i=1; i<=hotdogs; i++){
      leftToMake.push(i);
      data.push_back(thread_data());
     }

   int rc;
   int i;
   pthread_t makingThreads[M], packingThreads[P];
   pthread_attr_t attr;
   void *status;

   // Initialize and set thread joinable
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

   for( i = 0; i < M; i++ ) {
      rc = pthread_create(&makingThreads[i], NULL, make, (void *)i+1 );
      
      if (rc) {
         cout << "Error:unable to create thread," << rc << endl;
         exit(-1);
      }
   }

   // free attribute and wait for the other threads
   pthread_attr_destroy(&attr);
   for( i = 0; i < M; i++ ) {
      rc = pthread_join(makingThreads[i], &status);
      if (rc) {
         cout << "Error:unable to join," << rc << endl;
         exit(-1);
      }
   }

   // Initialize and set thread joinable
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

   for( i = 0; i < P; i++ ) {
      rc = pthread_create(&packingThreads[i], NULL, pack, (void *)i+1 );
      
      if (rc) {
         cout << "Error:unable to create thread," << rc << endl;
         exit(-1);
      }
   }

   // free attribute and wait for the other threads
   pthread_attr_destroy(&attr);
   for( i = 0; i < P; i++ ) {
      rc = pthread_join(packingThreads[i], &status);
      if (rc) {
         cout << "Error:unable to join," << rc << endl;
         exit(-1);
      }
   }

   // Write Manager Logs using the data structure
   pthread_exit(NULL);
}