#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>
#include <unistd.h>

int shared_resource = 0;
atomic_int lck = 0;
#define NUM_ITERS 50000
#define NUM_THREADS 296

void lock();
void unlock();

void lock()
{
	while(atomic_exchange(&lck, 1))
	{
		sleep(0.1);
	}
}

void unlock()
{
   	atomic_store(&lck, 0);
}

void* thread_func(void* arg) {
    int tid = *(int*)arg;
    
    lock();
    
        for(int i = 0; i < NUM_ITERS; i++)    shared_resource++;
    
    unlock();
    
    pthread_exit(NULL);
}

int main() {
    int n = NUM_THREADS;
    
    pthread_t threads[n];
    int tids[n];
    for (int i = 0; i < n; i++) {
        tids[i] = i;
        pthread_create(&threads[i], NULL, thread_func, &tids[i]);
    }
    
    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("shared: %d\n", shared_resource);
    
    return 0;
}
