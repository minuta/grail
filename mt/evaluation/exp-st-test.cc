/*
 * test: 
 *       - testing single-thread mode and perform some math calculations without
 *         I/O operations
 */

#include <iostream>
#include <pthread.h>
#include <time.h>
#include <math.h>

const int NUM_OF_THREADS {10000};
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


void threadFunction(int threadId){
    float res;
    for (int i=1; i<=NUM_OF_LOOPS_IN_THREAD; i++){
        //auto msg = "------> THREAD #" + std::to_string(long(threadId)) + ": hello " + std::to_string(i) + "\n";
        //std::cout << msg << std::endl;
        res = exponential(i, 10);
        //std::cout << exponential(i, 10) << std::endl;

    }
}


int main(int argc, char const *argv[])
{

    //std::cout << ">>> starting NS-3 application..." << std::endl;


    for(size_t i = 1; i < NUM_OF_THREADS+1; i++){
        //std::cout << ">>> creating THREAD #" << i << std::endl;
        threadFunction(i);
    }

    for(size_t i = 1; i <= NUM_OF_THREADS; i++){
            //std::cout << ">>> THREAD #" << i << " joined successfully" << std::endl;
    }

    
    //std::cout << ">>> finishing NS-3 application..." << std::endl;

    return EXIT_SUCCESS;
}
