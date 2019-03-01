/*
 * test: 
 *       - testing multiple-thread mode and perform some math calculations 
 *         without any I/O operations
 */

#include <iostream>
#include <pthread.h>
#include <time.h>
#include <math.h>

const int NUM_OF_THREADS {1000};
const int NUM_OF_LOOPS_IN_THREAD {20};

unsigned long int factorial(int n)
{
  return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

// Returns approximate value of e^x  
// using sum of first n terms of Taylor Series 
float exponential(int n, float x){
    float res = 1;
    for (int i=1; i<=n; i++){
        res = res + pow(x, i)/factorial(i);
    }
    return res;
}

void *threadFunction(void *threadId){
    float res;
    for (int i=1; i<=NUM_OF_LOOPS_IN_THREAD; i++){
        res = exponential(i, 10);
        //std::cout << i << ")" << res << std::endl;
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
