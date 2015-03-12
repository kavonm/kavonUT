#ifndef DIGIT_MAP_H_
#define DIGIT_MAP_H_

#define NUM_DIGITMAP       4
#define MAX_DIGITMAP_LEN   64

#define MATCH              0
#define NO_MATCH           -1
#define NOT_CHECK_INFO     -1

#define IS_NUM(c)                     ( ((c) >= '0') && ((c) <= '9') )

#define IS_LOWER_A2F(c)               ( ((c) >= 'a') && ((c) <= 'f') )
#define IS_UPPER_A2F(c)               ( ((c) >= 'A') && ((c) <= 'F') )
#define IS_A2F(c)                     ( IS_LOWER_A2F(c) || IS_UPPER_A2F(c))

#define IS_LOWER_X(c)                 ( (c) == 'x' )
#define IS_UPPER_X(c)                 ( (c) == 'X' )
#define IS_X(c)                       ( IS_LOWER_X(c) || IS_UPPER_X(c) )
#define IS_NOT_UPPER_X(c)             ( (c) != 'X' )

#define IS_ALPHA(c)                   ( IS_A2F(c) || IS_X(c))
#define TO_UPPER(c)                   c = ( (c)-'a'+'A' )

#define IS_LEFT_BRACKET(c)            ( (c) == '[' )
#define IS_RIGHT_BRACKET(c)           ( (c) == ']' )
#define IS_SHORT_DASH(c)              ( (c) == '-' )

#define IS_VALID_CHAR1(c)             ( IS_NUM(c) || IS_ALPHA(c) || IS_LEFT_BRACKET(c) )
#define IS_VALID_CHAR2(c)             ( IS_NUM(c) || IS_UPPER_A2F(c) )
#define IS_DIGIT_TO_DIGIT(c1, c2)     ( IS_NUM(c1) && IS_NUM(c2) && (c1 <= c2))
#define IS_A2F_TO_A2F(c1, c2)         ( IS_UPPER_A2F(c1) && IS_UPPER_A2F(c2) && (c1 <= c2))

#define DO_EXACT_MATCH(c)     IS_VALID_CHAR2(c)
#define DO_ANY_MATCH(c)       IS_UPPER_X(c)
#define DO_RANGE_MATCH(c)     IS_LEFT_BRACKET(c)
#define IN_RANGE(c,lo,hi)     ( ((c) >= (lo)) && ((c) <= (hi)) )

#define ValidateDigitMap(rawDigitMapStr, len) ValidateDigitMap_real(__FILE__, __LINE__, rawDigitMapStr, len)
#endif /*DIGIT_MAP_H_*/
