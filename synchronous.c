/* Code by Anaxímeno Brito */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "common.c"

/* SYNCHRONOUS Concurrent Philosophers Version:
 * In the synchronous version, the philosopher who didn't get one of the chopsticks
 * will wait until the chopstick is released to eat and then think.
 */

void* action(void* __phil) {
    phil_t* phil = (phil_t*) __phil;

    do {
        #if SHOW_INTENTION
            printf(" + %s is trying to eat!\n\n", phil->name);
        #endif

        pthread_mutex_lock(phil->left_hashi);
        pthread_mutex_lock(phil->right_hashi);

        eat(rand() % 6 + 3, phil);

        pthread_mutex_unlock(phil->left_hashi);
        pthread_mutex_unlock(phil->right_hashi);

        think(rand() % 7 + 2, phil);
    } while(!halt);

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    run(action, "Synchronous");
    return 0;
}
