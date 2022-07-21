/* Code by Anaxímeno Brito */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "common.c"

/* ASYNCHRONOUS Concurrent Philosophers Version:
 * In the asynchronous version, the philosopher who didn't get the two chopsticks
 * will stop eating and go straight to think, instead of waiting until the other chopstick is released to eat
 * and only then thinking as in synchronous.
 */

void* action(void* __phil) {
    phil_t* phil = (phil_t*) __phil;
    long left, right;

    do {
        #if SHOW_INTENTION
            printf(" + %s is trying to eat!\n\n", phil->name);
        #endif

        left = pthread_mutex_trylock(phil->left_hashi);
        right = pthread_mutex_trylock(phil->right_hashi);

        if (!left && !right) {
            eat(rand() % 6 + 3, phil);
        }

        pthread_mutex_unlock(phil->left_hashi);
        pthread_mutex_unlock(phil->right_hashi);

        think(rand() % 7 + 2, phil);
    } while (!halt);

    pthread_exit(NULL);
}


int main(int argc, char* argv[]) {
    run(action, "Asynchronous");
    return 0;
}
