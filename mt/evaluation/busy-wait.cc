#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
 
volatile int i = 0; /* i is global, so it is visible to all functions.
                       It's also marked volatile, because it
                       may change in a way which is not predictable by the compiler,
                       here from a different thread. Thus preventing the compiler from
                       caching its value.*/
 
/* f1 uses a spinlock to wait for i to change from 0. */
static void *f1(void *p)
{
    printf("Thread 1 : has started...\n");
    while (i==0) {
        /* do nothing - just keep checking over and over */
    } 
    printf("Thread 1 : Busy-waiting loop is over, because condition is now true...\n");
    return NULL;
}

static void *f2(void *p)
{
    printf("Thread 2 : has started...\n");
    sleep(10);   /* sleep for 10 seconds */
    printf("Thread 2 : has changed condition for the Thread 1 to true\n");
    i = 1;
    return NULL;
}

int main()
{
    int rc;
    pthread_t t1, t2;

    rc = pthread_create(&t1, NULL, f1, NULL);
    if (rc != 0) {
        fprintf(stderr,"pthread f1 failed\n");
        return EXIT_FAILURE;
    }
 
    rc = pthread_create(&t2, NULL, f2, NULL);
    if (rc != 0) {
        fprintf(stderr,"pthread f2 failed\n");
        return EXIT_FAILURE;
    }
 
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    puts("All pthreads have finished.\n");
    return 0;
}
