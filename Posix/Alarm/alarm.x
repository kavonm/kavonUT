#ifndef ALARM_X_
#define ALARM_X_

typedef struct alarm_V3 
{
    S32   seconds;
    S8    message[64];
} Alarm_V3;

/*
 * The "alarm_V4" structure now contains the time_t (time since the
 * Epoch, in seconds) for each alarm, so that they can be
 * sorted. Storing the requested number of seconds would not be
 * enough, since the "alarm thread" cannot tell how long it has
 * been on the list.
 */
typedef struct alarm_V4 Alarm_V4;
struct alarm_V4
{
    Alarm_V4  *link;
    S32       seconds;
    S8        message[64];
    time_t    time;   /* seconds from EPOCH */
};

typedef Alarm_V4 Alarm_V5;

EXTERN S16 SyncAlarm();
EXTERN S16 ForkAlarm();
EXTERN S16 ThreadAlarm();
EXTERN S16 MutexAlarm();
EXTERN S16 CondAlarm();

#endif /*ALARM_X_*/
