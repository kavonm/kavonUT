#ifndef MEMORY_POOL_H_
#define MEMORY_POOL_H_

#define MEMORY_ALIGNMENT       8
#define MEMORY_POOL_ALIGNMENT  16
#define MAX_MEMORY_POOL_ALLOC  4095

#define MEMORY_POOL_SIZE       1024 

#define ALIGN_PTR(align, ptr)  ( ((U64)(ptr) + ((U64)(align) - 1)) & ~((U64)(align) - 1) )
#endif /*MEMORY_POOL_H_*/
