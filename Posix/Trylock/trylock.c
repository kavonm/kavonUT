/*
 * trylock.c
 *
 * Demonstrate a simple use of pthread_mutex_trylock. The
 * counter_thread updates a shared counter at intervals, and a
 * monitor_thread occasionally reports the current value of the
 * counter -- but only if the mutex is not already locked by
 * counter_thread.
 *
 * Special notes: On a Solaris system, call thr_setconcurrency()
 * to allow interleaved thread execution, since threads are not
 * timesliced.
 */

/* header include files (.h) */
#include <gen.h>
#include "errors.h"
#include "memory_pool.h"
#include "trylock.h"

/* header/extern include files (.x) */
#include <gen.x>
#include "memory_pool.x"
#include "trylock.x"

/* global variables */
MemoryPool *pool;

/* local variables */
U32 counter;
time_t end_time;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/* local functions */
PRIVATE void *counter_thread();
PRIVATE void *monitor_thread();

/*
 * Thread start routine that repeatedly locks a mutex and
 * increments a counter.
 */
PRIVATE void *counter_thread()
{
    S32 status;
    U32 spin;

    /*
     * Until end_time, increment the counter each
     * second. Instead of just incrementing the counter, it
     * sleeps for another second with the mutex locked, to give
     * monitor_thread a reasonable chance of running.
     */
    while (time(NULL) < end_time)
    {
        status = pthread_mutex_lock (&mutex);
        if (status != 0)
        {
            err_abort (status, "Lock mutex");
        }

        for (spin = 0; spin < SPIN; spin++)
        {
            counter++;
        }

        status = pthread_mutex_unlock (&mutex);
        if (status != 0)
        {
            err_abort (status, "Unlock mutex");
        }

        sleep (1);
    }

    DP("Counter is %d\n", counter);

    return NULL;
}

/*
 * Thread start routine to "monitor" the counter. Every 3
 * seconds, try to lock the mutex and read the counter. If the
 * trylock fails, skip this cycle.
 */
PRIVATE void *monitor_thread()
{
    S32 status;
    U32 misses = 0;

    /*
     * Loop until end_time, checking the counter every 3
     * seconds.
     */
    while(time(NULL) < end_time)
    {
        sleep(3);
        status = pthread_mutex_trylock(&mutex);
        if(status != EBUSY)
        {
            if (status != 0)
            {
                err_abort (status, "Trylock mutex");
            }

            DP("Counter is %d\n", counter/SPIN);

            status = pthread_mutex_unlock(&mutex);
            if (status != 0)
            {
                err_abort (status, "Unlock mutex");
            }
        }
        else
        {
            misses++;           /* Count "misses" on the lock */
            DP("misses++ = %d\n", misses);
        }
    }

    DP("Monitor thread missed update %d times.\n", misses);

    return NULL;
}
                                                              
PUBLIC S16 Trylock()
{
    S32 status;
    pthread_t counter_thread_id;
    pthread_t monitor_thread_id;

#ifdef sun
    /*
     * On Solaris 2.5, threads are not timesliced. To ensure
     * that our threads can run concurrently, we need to
     * increase the concurrency level to 2.
     */
    DPRINTF (("Setting concurrency level to 2\n"));
    thr_setconcurrency (2); 
#endif  
    
    end_time = time(NULL) + 60;        /* Run for 1 minute */

    status = pthread_create(&counter_thread_id, NULL, counter_thread, NULL);
    if (status != 0)
    {
        err_abort(status, "Create counter thread");
    }

    status = pthread_create(&monitor_thread_id, NULL, monitor_thread, NULL);
    if (status != 0)
    {
        err_abort (status, "Create monitor thread");
    }

    status = pthread_join(counter_thread_id, NULL);
    if (status != 0)
    {
        err_abort (status, "Join counter thread");
    }

    status = pthread_join (monitor_thread_id, NULL);
    if (status != 0)
    {
        err_abort (status, "Join monitor thread");
    }

    return ROK;
}
