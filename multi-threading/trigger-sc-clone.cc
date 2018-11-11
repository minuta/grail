/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

/*
 * this program creates some threads and prints thread IDs to STDOUT
 * in particular it triggers the system call clone
 */

#include <iostream>
#include <pthread.h>

const int NUM_OF_THREADS {5};

void *sayHi(void *threadId){
    std::cout << ">>> hello from THREAD #" << long(threadId) << std::endl;
    pthread_exit(NULL); 
}


int main(int argc, char const *argv[])
{
    // std::cout << ">>> hello from the multi-threaded app" << std::endl;
    // std::cout << ">>> planning to create " << NUM_OF_THREADS << " threads:" << std::endl;

    pthread_t threads[NUM_OF_THREADS];

    for(size_t i = 1; i < NUM_OF_THREADS+1; i++){
        std::cout << ">>> creating THREAD #" << i << std::endl;
        int threadCreateStatus = pthread_create (&threads[i], NULL, sayHi, (void *)i);

        
        if (threadCreateStatus) {
            std::cout << ">>> error : failed to create a new THREAD #" << i << std::endl;
        }        
    }
    
    pthread_exit(NULL);

    return EXIT_SUCCESS;
}
