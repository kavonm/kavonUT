/*
 * Version 1
 *
 * Simple synchronous alarm program. This is used as a
 * reference for progressive examples of asynchronous
 * alarm programs.
 */

/* header include files (.h) */
#include <gen.h>
#include "errors.h"
#include "memory_pool.h"
#include "alarm.h"

/* header/extern include files (.x) */
#include <gen.x>
#include "memory_pool.x"
#include "alarm.x"

/* global variables */
MemoryPool *pool;

/* local functions */
PRIVATE void  alarm_insert(Alarm_V5 *alarm);
PRIVATE void *alarm_thread_V3(void *arg);
PRIVATE void *alarm_thread_V4(void *arg);
PRIVATE void *alarm_thread_V5(void *arg);

PUBLIC S16 SyncAlarm()
{
    U32 seconds;
    S8  line[128];
    S8  message[64];

    while (1) 
    {
        DP("Alarm> ");
        if (fgets(line, sizeof(line), stdin) == NULL) 
        {
            exit(0);
        }	

        if (strlen(line) <= 1)
        {
            continue;
        }	

        /*
         * Parse input line into seconds (%d) and a message
         * (%64[^\n]), consisting of up to 64 characters
         * separated from the seconds by whitespace.
         */
        if (sscanf(line, "%d %64[^\n]", &seconds, message) < 2) 
        {
            fprintf(stderr, "Bad command\n");
        } 
        else 
        {
            sleep(seconds);
            DP("(%d) %s\n", seconds, message);
        }
    }

    return ROK;
}

/*
 * Version 2
 *
 * This version uses fork to create a new process to
 * wait for each alarm to expire.
 */

PUBLIC S16 ForkAlarm()
{
    U32 seconds;
    S8  line[128];
    S8  message[64];
    pid_t pid;

    while (1) 
    {
        DP("Alarm> ");
        if (fgets(line, sizeof(line), stdin) == NULL) 
        {
            exit(0);
        }	

        if (strlen(line) <= 1)
        {
            continue;
        }	

        /*
         * Parse input line into seconds (%d) and a message
         * (%64[^\n]), consisting of up to 64 characters
         * separated from the seconds by whitespace.
         */
        if (sscanf(line, "%d %64[^\n]", &seconds, message) < 2) 
        {
            fprintf(stderr, "Bad command\n");
        } 
        else 
        {
            pid = fork();
            if (pid == (pid_t)-1)
            {
                errno_abort ("Fork");
            }

            if (pid == (pid_t)0) 
            {
                /*
                 * If we're in the child, wait and then print a message
                 */
                sleep(seconds);
                DP("(%d) (%s)\n", seconds, message);
                exit(0);
            }
            else 
            {
                /*
                 * In the parent, call waitpid() to collect any children that
                 * have already terminated.
                 */
                DP("forked PID is %d\n", pid);

                do {
                    pid = waitpid((pid_t)-1, NULL, WNOHANG);
                    if (pid == (pid_t)-1)
                    {
                        errno_abort("Wait for child");
                    }
                } while (pid != (pid_t)0);
            }
        }
    }

    return ROK;
}

/*
 * Version 3
 *
 * This version uses pthread_create to create a
 * separate thread to wait for each alarm to expire.
 */

PRIVATE void *alarm_thread_V3(void *arg)
{
    S32   status;
    Alarm_V3 *alarm = (Alarm_V3 *)arg;

    status = pthread_detach(pthread_self());
    if (status != 0)
    {
        err_abort(status, "Detach thread");
    }
	
    sleep(alarm->seconds);
    DP("(%d) %s\n", alarm->seconds, alarm->message);
    Free(pool, (void *)alarm);
    alarm = NULL;

    return NULL;
}

PUBLIC S16 ThreadAlarm()
{
    S32   status;
    S8    line[128];
    Alarm_V3  *alarm;
    pthread_t thread;

    while (1) 
    {
        DP("Alarm> ");
        if (fgets(line, sizeof(line), stdin) == NULL) 
        {
            exit(0);
        }	

        if (strlen(line) <= 1)
        {
            continue;
        }	

        alarm = (Alarm_V3 *)Malloc(pool, sizeof(Alarm_V3));
        if (alarm == NULL)
        {
            errno_abort("Allocate alarm");
        }

        /*
         * Parse input line into seconds (%d) and a message
         * (%64[^\n]), consisting of up to 64 characters
         * separated from the seconds by whitespace.
         */
        if (sscanf(line, "%d %64[^\n]", &alarm->seconds, alarm->message) < 2) 
        {
            fprintf(stderr, "Bad command\n");
            Free(pool, (void *)alarm);
            alarm = NULL;
        } 
        else 
        {
            status = pthread_create(&thread, NULL, alarm_thread_V3, alarm);
            if (status != 0)
            {
                err_abort(status, "Create alarm thread");
            }
			/* pthread_detach(thread) */
        }
    }

    return ROK;
}

/*
 * Version 4
 *
 * This is an enhancement to Version 3 program, which
 * created an "alarm thread" for each alarm command. This new
 * version uses a single alarm thread, which reads the next
 * entry in a list. The main thread places new requests onto the
 * list, in order of absolute expiration time. The list is
 * protected by a mutex, and the alarm thread sleeps for at
 * least 1 second, each iteration, to ensure that the main
 * thread can lock the mutex to add new work to the list.
 */

pthread_mutex_t alarm_mutex_V4 = PTHREAD_MUTEX_INITIALIZER;
Alarm_V4 *alarm_list_V4 = NULL;

PRIVATE void *alarm_thread_V4(void *arg)
{
    U32      sleep_time;
    S32      status;
    time_t   now;
    Alarm_V4 *alarm;

    /*
     * Loop forever, processing commands. The alarm thread will
     * be disintegrated when the process exits.
     */
    while (1) 
    {
        status = pthread_mutex_lock(&alarm_mutex_V4);
        if (status != 0)
        {
            err_abort(status, "Lock mutex");
        }

        alarm = alarm_list_V4;

        /*
         * If the alarm list is empty, wait for one second. This
         * allows the main thread to run, and read another
         * command. If the list is not empty, remove the first
         * item. Compute the number of seconds to wait -- if the
         * result is less than 0 (the time has passed), then set
         * the sleep_time to 0.
         */
        if (alarm == NULL)
        {
            sleep_time = 1;
        }
        else
        {
            alarm_list_V4 = alarm->link;
            now = time(NULL);
            if (alarm->time <= now)
            {
                sleep_time = 0;
            }
            else
            {
                sleep_time = alarm->time - now;
            }

            DP("[waiting: %d(%d)\"%s\"]\n",
                (U32)alarm->time, sleep_time, alarm->message);
        }

        /*
         * Unlock the mutex before waiting, so that the main
         * thread can lock it to insert a new alarm request. If
         * the sleep_time is 0, then call sched_yield, giving
         * the main thread a chance to run if it has been
         * readied by user input, without delaying the message
         * if there's no input.
         */
        status = pthread_mutex_unlock (&alarm_mutex_V4);
        if (status != 0)
        {
            err_abort(status, "Unlock mutex");
        }

        if (sleep_time > 0)
        {
            sleep(sleep_time);
        }
        else
        {
            sched_yield();
        }

        /*
         * If a timer expired, print the message and free the
         * structure.
         */
        if (alarm != NULL) 
        {
            DP("(%d) %s\n", alarm->seconds, alarm->message);
            Free(pool, (void *)alarm);
            alarm = NULL;
        }
    }
}

PUBLIC S16 MutexAlarm()
{
    S8  line[128];
    S32 status;
    Alarm_V4 *alarm, **last, *next;
    pthread_t thread;

    status = pthread_create(&thread, NULL, alarm_thread_V4, NULL);
    if (status != 0)
    {
        err_abort(status, "Create alarm thread");
    }

    while (1)
    {
        DP("Alarm> ");
        if (fgets(line, sizeof(line), stdin) == NULL) 
        {
            exit(0);
        }	

        if (strlen(line) <= 1)
        {
            continue;
        }	

        alarm = (Alarm_V4 *)Malloc(pool, sizeof(Alarm_V4));
        if (alarm == NULL)
        {
            errno_abort("Allocate alarm");
        }

        /*
         * Parse input line into seconds (%d) and a message
         * (%64[^\n]), consisting of up to 64 characters
         * separated from the seconds by whitespace.
         */
        if (sscanf(line, "%d %64[^\n]", &alarm->seconds, alarm->message) < 2) 
        {
            fprintf(stderr, "Bad command\n");
            Free(pool, (void *)alarm);
            alarm = NULL;
        } 
        else 
        {
            status = pthread_mutex_lock(&alarm_mutex_V4);
            if (status != 0)
            {
                err_abort(status, "Lock mutex");
            }
            alarm->time = time(NULL) + alarm->seconds;

            /*
             * Insert the new alarm into the list of alarms,
             * sorted by expiration time.
             */
            last = &alarm_list_V4;
            next = *last;
            while (next != NULL) 
            {
                if (next->time >= alarm->time)
                {
                    alarm->link = next;
                    *last = alarm;
                    break;
                }
                last = &next->link;
                next = next->link;
            }

            /*
             * If we reached the end of the list, insert the new
             * alarm there. ("next" is NULL, and "last" points
             * to the link field of the last item, or to the
             * list header).
             */
            if (next == NULL)
            {
                *last = alarm;
                alarm->link = NULL;
            }

            DP("[list: ");
            for (next = alarm_list_V4; next != NULL; next = next->link)
            {
                DP("%d(%d)[\"%s\"] ", (U32)next->time, (U32)(next->time - time(NULL)), next->message);
            }
            DP("]\n");

            status = pthread_mutex_unlock(&alarm_mutex_V4);
            if (status != 0)
            {
                err_abort(status, "Unlock mutex");
            }
        }
    }

    return ROK;
}

/*
 * Version 5
 *
 * This is an enhancement to Version 4 program, which
 * used only a mutex to synchronize access to the shared alarm
 * list. This version adds a condition variable. The alarm
 * thread waits on this condition variable, with a timeout that
 * corresponds to the earliest timer request. If the main thread
 * enters an earlier timeout, it signals the condition variable
 * so that the alarm thread will wake up and process the earlier
 * timeout first, requeueing the later request.
 */

pthread_mutex_t alarm_mutex_V5 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t alarm_cond = PTHREAD_COND_INITIALIZER;
Alarm_V5 *alarm_list_V5 = NULL;
time_t current_alarm = 0;

/*
 * Insert alarm entry on list, in order.
 */
PRIVATE void alarm_insert(Alarm_V5 *alarm)
{
    S32 status;
    Alarm_V5 **last, *next;

    /*
     * LOCKING PROTOCOL:
     * 
     * This routine requires that the caller have locked the
     * alarm_mutex_V5!
     */
    last = &alarm_list_V5;
    next = *last;
    while (next != NULL) 
    {
        if (next->time >= alarm->time)
        {
            alarm->link = next;
            *last = alarm;
            break;
        }
        last = &next->link;
        next = next->link;
    }

    /*
     * If we reached the end of the list, insert the new alarm
     * there.  ("next" is NULL, and "last" points to the link
     * field of the last item, or to the list header.)
     */
    if (next == NULL) {
        *last = alarm;
        alarm->link = NULL;
    }

    DP("[list: ");
    for (next = alarm_list_V5; next != NULL; next = next->link)
    {
        DP("%d(%d)[\"%s\"] ", (U32)next->time, (U32)(next->time - time(NULL)), next->message);
    }
    DP("]\n");

    /*
     * Wake the alarm thread if it is not busy (that is, if
     * current_alarm is 0, signifying that it's waiting for
     * work), or if the new alarm comes before the one on
     * which the alarm thread is waiting.
     */
    if (current_alarm == 0 || alarm->time < current_alarm)
    {
        current_alarm = alarm->time;

        status = pthread_cond_signal(&alarm_cond);
        if (status != 0)
        {
            err_abort(status, "Signal cond");
        }
    }
}

PRIVATE void *alarm_thread_V5(void *arg)
{
    Alarm_V5 *alarm;
    struct timespec cond_time;
    time_t now;
    S32 status, expired;

    /*
     * Loop forever, processing commands. The alarm thread will
     * be disintegrated when the process exits. Lock the mutex
     * at the start -- it will be unlocked during condition
     * waits, so the main thread can insert alarms.
     */
    status = pthread_mutex_lock(&alarm_mutex_V5);
    if (status != 0)
    {
        err_abort(status, "Lock mutex");
    }

    while (1) 
    {
        /*
         * If the alarm list is empty, wait until an alarm is
         * added. Setting current_alarm to 0 informs the insert
         * routine that the thread is not busy.
         */
        current_alarm = 0;

        while (alarm_list_V5 == NULL)
        {
            status = pthread_cond_wait(&alarm_cond, &alarm_mutex_V5);
            if (status != 0)
            {
                err_abort(status, "Wait on cond");
            }
        }

        alarm = alarm_list_V5;
        alarm_list_V5 = alarm->link;
        now = time(NULL);
        expired = 0;
        if (alarm->time > now)
        {
            DP("[waiting: %d(%d)\"%s\"]\n", (U32)alarm->time, (U32)(alarm->time - time(NULL)), alarm->message);

            cond_time.tv_sec = alarm->time;
            cond_time.tv_nsec = 0;

            current_alarm = alarm->time;
            while (current_alarm == alarm->time)
            {
                status = pthread_cond_timedwait(&alarm_cond, &alarm_mutex_V5, &cond_time);
                if (status == ETIMEDOUT)
                {
                    expired = 1;
                    break;
                }

                if (status != 0)
                {
                    err_abort(status, "Cond timedwait");
                }
            }

            if (!expired)
            {
                alarm_insert(alarm);
            }
        } 
        else
        {
            expired = 1;
        }

        if (expired)
        {
            DP("(%d) %s\n", alarm->seconds, alarm->message);
            Free(pool, (void *)alarm);
            alarm = NULL;
        }
    }
}

PUBLIC S16 CondAlarm()
{
    S32 status;
    S8  line[128];
    Alarm_V5 *alarm;
    pthread_t thread;

    status = pthread_create(&thread, NULL, alarm_thread_V5, NULL);
    if (status != 0)
    {
        err_abort (status, "Create alarm thread");
    }

    while (1)
    {
        DP("Alarm> ");
        if (fgets(line, sizeof(line), stdin) == NULL) 
        {
            exit(0);
        }	

        if (strlen(line) <= 1)
        {
            continue;
        }	

        alarm = (Alarm_V5 *)Malloc(pool, sizeof(Alarm_V5));
        if (alarm == NULL)
        {
            errno_abort ("Allocate alarm");
        }

        /*
         * Parse input line into seconds (%d) and a message
         * (%64[^\n]), consisting of up to 64 characters
         * separated from the seconds by whitespace.
         */
        if (sscanf(line, "%d %64[^\n]", &alarm->seconds, alarm->message) < 2) 
        {
            fprintf(stderr, "Bad command\n");
            Free(pool, (void *)alarm);
            alarm = NULL;
        } 
        else 
        {
            status = pthread_mutex_lock(&alarm_mutex_V5);
            if (status != 0)
            {
                err_abort(status, "Lock mutex");
            }

            alarm->time = time(NULL) + alarm->seconds;

            /*
             * Insert the new alarm into the list of alarms,
             * sorted by expiration time.
             */
            alarm_insert(alarm);
            status = pthread_mutex_unlock(&alarm_mutex_V5);
            if (status != 0)
            {
                err_abort(status, "Unlock mutex");
            }
        }
    }

    return ROK;
}
