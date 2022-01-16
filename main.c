/* Code by Anaxímeno Brito */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define N_DINNING_PHILOSOPHERS 5
#define N_DINNIG_EPOCHS 1


struct _philosopher {
    unsigned short id;
    pthread_mutex_t* left_hashi;
    pthread_mutex_t* right_hashi;
    char* name;
};

typedef struct _philosopher phil_t;


void init_mutexes(pthread_mutex_t mutexes[], int n) {
    for (int i = 0 ; i < n ; ++i) { pthread_mutex_init(&mutexes[i], NULL); }
}


void destroy_mutexes(pthread_mutex_t mutexes[], int n) {
    for (int i = 0 ; i < n ; ++i) { pthread_mutex_destroy(&mutexes[i]); };
}


void init_philosophers(int n, phil_t philosophers[], pthread_mutex_t hashis[]) {
    char* names[] = {"Aristotle", "Plato","Socrates", "Democritus", "Descartes"};
    for (int i = 0 ; i < n ; ++i) {
        (philosophers + i)->id = i;
        (philosophers + i)->name = names[i];
        (philosophers + i)->left_hashi = &hashis[i == 0 ? n-1 : i-1];
        (philosophers + i)->right_hashi = &hashis[i];
    }
}


void eat(int time_eating, phil_t* philosopher) {
    printf(" %s is eating :)\n\n", philosopher->name);
    sleep(time_eating);
    printf(" %s stopped eating.\n\n", philosopher->name);
    return ;
}


void think(int time_thinking, phil_t* phil) {
    printf(" %s is thinking -_- (...)\n\n", phil->name);
    sleep(time_thinking);
}


void* action(void* __phil) {
    phil_t* phil = (phil_t*) __phil;
    printf(" %s is trying to eat!\n\n", phil->name);
    pthread_mutex_lock(phil->left_hashi);
    pthread_mutex_lock(phil->right_hashi);

    eat(rand() % 6 + 3, phil);

    pthread_mutex_unlock(phil->left_hashi);
    pthread_mutex_unlock(phil->right_hashi);

    think(rand() % 7 + 2, phil);

    pthread_exit(NULL);
}



int main() {
    time_t t;
    pthread_mutex_t hashis[N_DINNING_PHILOSOPHERS];
    phil_t philosophers[N_DINNING_PHILOSOPHERS];
    pthread_t threads[N_DINNING_PHILOSOPHERS];

    srand((unsigned) time(&t));
    init_mutexes(hashis, N_DINNING_PHILOSOPHERS);
    init_philosophers(N_DINNING_PHILOSOPHERS, philosophers, hashis);

    long status;
    for (int i = 0 ; i < N_DINNIG_EPOCHS ; ++i) {
        for (int j = 0 ; j < N_DINNING_PHILOSOPHERS ; ++j) {
            status = pthread_create(&threads[j], NULL, action, (void*)(philosophers + j));
            if (status) {
                perror("pthread_create");
                exit(status);
            } else continue;
        }
        for (int k = 0 ; k < N_DINNING_PHILOSOPHERS ; ++k) {
            pthread_join(threads[k], NULL);
        }
        printf("\n -----------End Epoch %d----------\n\n", i + 1);
    }

    destroy_mutexes(hashis, N_DINNING_PHILOSOPHERS);
    return 0;
}
