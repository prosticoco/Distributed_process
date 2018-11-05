#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#define S 1000000

/*
    Findings:
    * calling pthread_cancel() on a sleeping thread wakes it up and cancels it.
    * calling pthread_cancel() on a thread blocked on a mutex, DOES NOT wake it up, UNLESS it is set CANCEL ASYNCHRONOUS
    * calling pthread_kill() on a thread blocked on a mutex, KILLS THE WHOLE PROGRAM OH NO
    * same if the thread is just sleeping :(
*/
pthread_t thread;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *f(void* args) {
    printf("Thread started, try to acquire lock\n");
    int dump;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &dump);
    pthread_mutex_lock(&mutex);
    //usleep(20*S);
    printf("Bye from INSIDE thread\n");
}

int main(void) {
    // Seize the lock now so the thread will wait on it.
    printf("main() seizes lock\n");
    pthread_mutex_lock(&mutex);
    pthread_create(&thread, NULL, f, NULL);
    usleep(2*S);
    printf("main() attempts to kill and join thread waiting on lock\n");
    pthread_cancel(thread);
    pthread_join(thread, NULL);
    //pthread_kill(thread, SIGKILL);
    printf("Cancel succeeded\n");
    pthread_mutex_unlock(&mutex);
    pthread_mutex_destroy(&mutex);
    printf("BYE from OUTSIDE thread\n");
}