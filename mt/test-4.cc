/*
 * test: 
 *       - testing multiple threads : main thread and three additional threads
 *       - testing execution of multiple statements in the thread-function
 *       - add sleep in thread-function
 * 
 *
 * this program creates some threads and prints thread IDs to STDOUT
 * in particular it triggers the system call clone
 */

#include <iostream>
#include <pthread.h>
//#include <unistd.h>
#include <chrono>
#include <thread>

const int NUM_OF_THREADS {3};
const int NUM_OF_LOOPS_IN_THREAD {100};

void *threadFunction(void *threadId){
    for (int i=1; i<=NUM_OF_LOOPS_IN_THREAD; i++){
        auto msg = "------> THREAD #" + std::to_string(long(threadId)) + ": hello " + std::to_string(i) + "\n";
        std::cout << msg;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        //sleep (1);
    }
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

    
    std::cout << ">>> finishing NS-3 application..." << std::endl;
    pthread_exit(NULL);   // obsolete, due to the return statement in the main()

    return EXIT_SUCCESS;
}
