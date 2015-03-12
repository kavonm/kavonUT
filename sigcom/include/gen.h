#ifndef GEN_H_
#define GEN_H_

#include <stdio.h>
#include <stdlib.h>
#if 0
#include <malloc.h>
#endif
#include <string.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <sys/types.h>
#include <wait.h>
#include <pthread.h>

#define ROK            0
#define RFAILED       -1
#define TRUE           1
#define FALSE          0

#define NOTPRSNT       0
#define PRSNT_NODEF    1

#define MF_SIZE_TKNSTR 132

#define DP printf

#define PUBLIC
#define PRIVATE        static
#define EXTERN         extern

#endif /* GEN_H_ */
