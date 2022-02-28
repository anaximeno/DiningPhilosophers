#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

#define N_DINNING_PHILOSOPHERS 5
#define SECONDS_TO_WAIT_BEFORE_HALT 30


struct _philosopher {
    char* name;
    unsigned short id;
    pthread_mutex_t* left_hashi;
    pthread_mutex_t* right_hashi;
    unsigned int thought;
    unsigned int ate;
};


typedef struct _philosopher phil_t;


bool halt = false;


void init_mutexes(pthread_mutex_t mutexes[], int n) {
    for (int i = 0 ; i < n ; ++i) { pthread_mutex_init(&mutexes[i], NULL); }
}


void destroy_mutexes(pthread_mutex_t mutexes[], int n) {
    for (int i = 0 ; i < n ; ++i) { pthread_mutex_destroy(&mutexes[i]); };
}


void init_philosophers(int n, phil_t phils[], pthread_mutex_t hashis[]) {
    char* names[] = {
        "Aristotle", "Plato", "Socrates", "Democritus", "Descartes"
    };

    unsigned short i;
    phil_t* phil = NULL;
    for (i = 0, phil = phils ; i < n ; ++i, phil++) {
        phil->id             = i;
        phil->ate            = 0;
        phil->thought        = 0;
        phil->name           = names[i];
        phil->right_hashi    = &hashis[i];
        phil->left_hashi     = &hashis[
            i == 0 ? n - 1 : i - 1
        ];
    }
}


void eat(int time_eating, phil_t* phil) {
    printf(" :) %s is eating.\n\n", phil->name);
    sleep(time_eating);

    // Increase the number of times it ate
    phil->ate ++;

    printf(" - %s stopped eating.\n\n", phil->name);
}


void think(int time_thinking, phil_t* phil) {
    printf(" -_- %s is thinking...\n\n", phil->name);
    sleep(time_thinking);

    // Increase the number of time ti thought
    phil->thought ++;

    printf(" - %s stopped thinking.\n\n", phil->name);
}


void* timer(void* _) {
    sleep(SECONDS_TO_WAIT_BEFORE_HALT);

    // Stops the concurrent threads
    halt = true;

    pthread_exit(NULL);
}


void show_action_status(phil_t* philosophers, int n) {
    int i, sum_of_actions = 0;
    phil_t* phil = NULL;
    for (i = 0, phil = philosophers ; i < n ; ++i, phil++) {
        sum_of_actions += phil->ate + phil->thought;
        printf(" %s ate %d times and thought %d times.\n\n",
            phil->name, phil->ate, phil->thought);
    }
    printf(" -> The total of taken actions are %d <- \n\n", sum_of_actions);
}