/*
 * test: 
 *       - testing single-thread mode and perform a work amount similar to  perf-test.cc
 *
 */

#include <iostream>
#include <pthread.h>
//#include <chrono>
#include <time.h>



const int NUM_OF_THREADS {5};
const int NUM_OF_LOOPS_IN_THREAD {10};

void threadFunction(int threadId){
    for (int i=1; i<=NUM_OF_LOOPS_IN_THREAD; i++){
        auto msg = "------> THREAD #" + std::to_string(long(threadId)) + ": hello " + std::to_string(i) + "\n";
        std::cout << msg;
    }
    // std::cout << "------> hello from THREAD #" << long(threadId) << std::endl;
    //pthread_exit(NULL); 
}


int main(int argc, char const *argv[])
{

    //clock_t start, end;
    //double cpu_time_used;
     

    //start = clock();

    //std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    std::cout << ">>> starting NS-3 application..." << std::endl;

    //pthread_t threads[NUM_OF_THREADS];

    for(size_t i = 1; i < NUM_OF_THREADS+1; i++){
        std::cout << ">>> creating THREAD #" << i << std::endl;
        threadFunction(i);
        //int threadCreateStatus = pthread_create (&threads[i], NULL, threadFunction, (void *)i);

        
        //if (threadCreateStatus) {
            //std::cout << ">>> error : failed to create a new THREAD #" << i << std::endl;
        //}        
    }

    // do a pthread_join(), otherwise the main thread can terminate before other threads terminate and thus force them to terminate!
    for(size_t i = 1; i <= NUM_OF_THREADS; i++){
        //if (pthread_join(threads[i], NULL)==0)
            std::cout << ">>> THREAD #" << i << " joined successfully" << std::endl;
        //else
            //std::cout << ">>> THREAD #" << i << " couldn't be joined successfully" << std::endl;
    }

    
    std::cout << ">>> finishing NS-3 application..." << std::endl;

    //std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();
    //std::cout << "\n Execution time = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" <<std::endl;
    //std::cout << "Time difference (sec) = " << (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) /1000000.0 <<std::endl;

    //end = clock();
    //cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    //std::cout << "\n Execution time (sec) : " << cpu_time_used;

    //pthread_exit(NULL);   // obsolete, due to the return statement in the main()

    return EXIT_SUCCESS;
}
