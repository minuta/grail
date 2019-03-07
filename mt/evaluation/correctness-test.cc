/*
 * test correctness : 
 *      this test spawns 1000 threads, whereby integer variable sharedCounter 
 *      will be increased by the thread id number.
 */

#include <iostream>
#include <pthread.h>
//#include <chrono>
#include <time.h>



const int NUM_OF_THREADS {1000};
const int NUM_OF_LOOPS_IN_THREAD {10};
int sharedCounter = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *threadFunction(void *threadId){
    for (int i=1; i<=NUM_OF_LOOPS_IN_THREAD; i++){
        auto msg = "------> THREAD #" + std::to_string(long(threadId)) + ": loop " + std::to_string(i) + "\n";
        std::cout << msg;

        pthread_mutex_lock(&mutex);
        sharedCounter = sharedCounter + long(threadId);
        pthread_mutex_unlock(&mutex);

     }
    // std::cout << "------> hello from THREAD #" << long(threadId) << std::endl;
     //pthread_mutex_lock(&mutex);
     //sharedCounter = sharedCounter + long(threadId);
     //pthread_mutex_unlock(&mutex);

    pthread_exit(NULL); 
}


int main(int argc, char const *argv[])
{

    std::cout << ">>> starting NS-3 application..." << std::endl;

    pthread_t threads[NUM_OF_THREADS];

    for(size_t i = 1; i < NUM_OF_THREADS+1; i++){
        std::cout << ">>> creating THREAD #" << i << std::endl;
        int threadCreateStatus = pthread_create (&threads[i], NULL, threadFunction, (void *)i);
        
        if (threadCreateStatus) {
            std::cout << ">>> error : failed to create a new THREAD #" << i << std::endl;
        }        
    }

    // do a pthread_join(), otherwise the main thread can terminate before other threads terminate and thus force them to terminate!
    for(size_t i = 1; i <= NUM_OF_THREADS; i++){
        if (pthread_join(threads[i], NULL)==0)
            std::cout << ">>> THREAD #" << i << " joined successfully" << std::endl;
        else
            std::cout << ">>> THREAD #" << i << " couldn't be joined successfully" << std::endl;
    }

    std::cout << "counter : " << sharedCounter << std::endl;
    std::cout << ">>> finishing NS-3 application..." << std::endl;

    pthread_exit(NULL);   // obsolete, due to the return statement in the main()

    return EXIT_SUCCESS;
}
