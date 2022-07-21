#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>

#define SECONDS_TO_WAIT_BEFORE_HALT 30


struct _philosopher {
    const char* name;
    unsigned short id;
    pthread_mutex_t* left_hashi;
    pthread_mutex_t* right_hashi;
    unsigned int thought;
    unsigned int ate;
};

struct name_set {
    const char* const* names;
    const unsigned quantity;
};


typedef struct _philosopher phil_t;


static bool halt = false;


static void init_mutexes(pthread_mutex_t mutexes[], int n)
/* This function initializes a list of mutexes of size `n` */
{
    for (int i = 0 ; i < n ; ++i) { pthread_mutex_init(&mutexes[i], NULL); }
}


static void destroy_mutexes(pthread_mutex_t mutexes[], int n)
/* This function should be used to destroy a list of mutexes of size `n` */
{
    for (int i = 0 ; i < n ; ++i) { pthread_mutex_destroy(&mutexes[i]); };
}


static void init_philosophers(phil_t phils[], struct name_set phil_names,
                                pthread_mutex_t hashis[])
/* Initializes the philosophers. */
{
    unsigned short i;
    phil_t* phil = NULL;
    for (i = 0, phil = phils ; i < phil_names.quantity ; ++i, phil++) {
        int j = i == 0 ? phil_names.quantity - 1 : i - 1;

        phil->id             = i;
        phil->ate            = 0;
        phil->thought        = 0;
        phil->name           = phil_names.names[i];
        phil->right_hashi    = &hashis[i];
        phil->left_hashi     = &hashis[j];
    }
}


static void eat(int time_eating, phil_t* phil)
/* Simulates the philosopher eatting. What it really does is
 * displaying some messages while sleeping for the given time.
 * */
{
    printf(" :) %s is eating.\n\n", phil->name);
    sleep(time_eating);

    phil->ate ++;

    printf(" - %s stopped eating.\n\n", phil->name);
}


static void think(int time_thinking, phil_t* phil)
/* Simulates the philosopher thinking. What it really does is
 * displaying some messages while sleeping for the given time.
 * */
{
    printf(" -_- %s is thinking...\n\n", phil->name);
    sleep(time_thinking);

    phil->thought ++;

    printf(" - %s stopped thinking.\n\n", phil->name);
}


static void* timer(void* time)
/* Waits for a certain amount of time before updating the status
 * of the vairable that controls the execution of the action cycle of the
 * philosophers to false, allowing the threads to stop the execution.
 * */
{
    sleep((long) time);

    // Stops the concurrent threads
    halt = true;

    pthread_exit(NULL);
}


static void show_action_status(phil_t* philosophers, int n)
/* Resumes information on all actions performed by the philosophers. */
{
    int i, sum_of_actions = 0;
    phil_t* phil = NULL;
    for (i = 0, phil = philosophers ; i < n ; ++i, phil++) {
        sum_of_actions += phil->ate + phil->thought;
        printf(" %s ate %d times and thought %d times.\n\n",
            phil->name, phil->ate, phil->thought);
    }
    printf(" -> The total of taken actions are %d <- \n\n", sum_of_actions);
}


static void execute_threads(pthread_t* threads, phil_t* philosophers,
                        unsigned n_philosophers, void* (*action) (void*))
/* Creates and runs the threads. */
{
    long status;
    unsigned short j;
    for (j = 0 ; j < n_philosophers ; ++j) {
        status = pthread_create(&threads[j], NULL, action, (void*)(philosophers + j));
        if (status) {
            perror("pthread_create");
            exit(status);
        } else continue;
    }

    if ((status = pthread_create(&threads[j], NULL, timer, (void*) SECONDS_TO_WAIT_BEFORE_HALT))) {
        perror("pthread_create");
        exit(status);
    }

    for (int k = 0 ; k < n_philosophers + 1 ; ++k) {
        pthread_join(threads[k], NULL);
    }
}

extern void run(void* (*action) (void*), const char* type)
/* Runs the code depending on the action. */
{
    static const char* names[] = {
        "Aristotle", "Plato", "Socrates", "Democritus", "Descartes",
    };

    struct name_set phil_names = {
        .names=names,
        .quantity=5
    };


    pthread_mutex_t hashis[phil_names.quantity];
    phil_t philosophers[phil_names.quantity];
    pthread_t threads[phil_names.quantity + 1];

    time_t t;

    srand((unsigned) time(&t));
    init_mutexes(hashis, phil_names.quantity);
    init_philosophers(philosophers, phil_names, hashis);

    printf("\n Type: %s\n", type);
    printf(" Execution Time: %d seconds\n", SECONDS_TO_WAIT_BEFORE_HALT);
    printf("\n ---------------------Init---------------------\n\n");

    execute_threads(threads, philosophers, phil_names.quantity, action);

    printf("\n ---------------------End---------------------\n\n");

    destroy_mutexes(hashis, phil_names.quantity);

    show_action_status(philosophers, phil_names.quantity);
}
