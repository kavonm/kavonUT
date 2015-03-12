#ifndef GEN_X_
#define GEN_X_

typedef char S8;           
typedef unsigned char U8;
typedef short S16;
typedef unsigned short U16;
typedef int S32;
typedef unsigned int U32;
typedef long S64;
typedef unsigned long U64;
typedef float FLOAT;
typedef double DOUBLE;

typedef struct tknU8                     
{
    U8  pres;
    U8  val;
} TknU8;

typedef struct tknStr                     
{
    U8  pres;
    U8  len;
    U16 spare;
    S8  val[(MF_SIZE_TKNSTR + 3) & 0xffc];
} TknStr;

typedef struct tknUStr                     
{
    U8  pres;
    U8  len;
    U16 spare;
    U8  val[(MF_SIZE_TKNSTR + 3) & 0xffc];
} TknUStr;

typedef struct elmtHdr
{
    U8 pres;
} ElmtHdr;
#endif /* GEN_X_ */
