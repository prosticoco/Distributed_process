#include <stdio.h>
#include <pthread.h>
#include <string.h>



int main(){
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex,NULL);
    pthread_mutex_lock(&mutex);
    int error = pthread_mutex_trylock(&mutex);
    printf("Error num : %s",strerror(error));
    pthread_mutex_unlock(&mutex);
    
}