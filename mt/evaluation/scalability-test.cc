/*
 * test: 
 *       - testing multiple-thread mode and perform a nanosleep() per loop-iteration
 */

#include <iostream>
#include <pthread.h>
#include <time.h>
#include <math.h>

const int NUM_OF_THREADS {1000};
const int NUM_OF_LOOPS_IN_THREAD {100};

//#define NANO_SECOND_MULTIPLIER  1000000  // 1 millisecond = 1,000,000 Nanoseconds
#define MICRO_SECOND  1000  // 1 microsecond = 1,000 Nanoseconds
const long INTERVAL_MS = 1 * MICRO_SECOND;



void *threadFunction(void *threadId){

    timespec sleepValue = {0};
    sleepValue.tv_nsec = INTERVAL_MS;

    for (int i=1; i<=NUM_OF_LOOPS_IN_THREAD; i++){

        nanosleep(&sleepValue, NULL); // sleep for 1 milisecond

    }
    pthread_exit(NULL); 
}


int main() {

  
    pthread_t threads[NUM_OF_THREADS];

    for(size_t i = 1; i < NUM_OF_THREADS+1; i++){
        //std::cout << ">>> creating THREAD #" << i << std::endl;
        int threadCreateStatus = pthread_create (&threads[i], NULL, threadFunction, (void *)i);

        if (threadCreateStatus) {
            std::cout << ">>> error : failed to create a new THREAD #" << i << std::endl;
        }        
    }

    // do a pthread_join(), otherwise the main thread can terminate before other 
    // threads terminate and thus force them to terminate!
    for(size_t i = 1; i <= NUM_OF_THREADS; i++){
        if (pthread_join(threads[i], NULL)!=0)
            //std::cout << ">>> THREAD #" << i << " joined successfully" << std::endl;
            std::cout << ">>> THREAD #" << i << " couldn't be joined successfully" << std::endl;
    }

    //std::cout << ">>> finishing NS-3 application..." << std::endl;
    pthread_exit(NULL);   // obsolete, due to the return statement in the main()

    return EXIT_SUCCESS;
}
