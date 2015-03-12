/* header include files (.h) */
#include "gen.h"
#include "memory_pool.h"
#include "isup_cust.h"

/* header/extern include files (.x) */
#include "gen.x"
#include "memory_pool.x"
#include "isup_cust.x"

/* global variables */
MemoryPool *pool;

PUBLIC S16 read_ISUP_CUST()
{
    FILE *fhandle = NULL;
    S8 line_buf[256];
    S8 *start_ptr;
    S8 *end_ptr;
	U16 trkgrp;
	U16 i;

    fhandle = fopen("ISUP_CUST", "r");
    if (fhandle == NULL)
    {
        DP("ISUP_CUST file does not exist\n");
        return ROK;
    }

	/* read the file line by line */
	while (fgets(line_buf, 256, fhandle) != NULL)
	{
		i = 0;  
		trkgrp = 0;

        start_ptr = line_buf;
        /* ignore blank space */
        while (*start_ptr == ' ')
        {
            start_ptr++;
        }
        
        /* look for the trkgrp number */ 
        while ((*start_ptr >= '0') && (*start_ptr <= '9') && (i < 4))
        {
			trkgrp = (trkgrp * 10) + (*start_ptr - '0');
			i++;
            start_ptr++;
        }

		if ((*start_ptr == ',') && (trkgrp > 0))
		{
		    start_ptr++;
		}
		else
        {
			continue;
        }

        /* ignore blank space */
        while (*start_ptr == ' ')
        {
            start_ptr++;
        }

		end_ptr = start_ptr;
		/* look for the end position of the key string */
		while ((*end_ptr != ' ') &&
               (*end_ptr != '\n') &&
               (*end_ptr != 0))
		{
			end_ptr++;
		}

		*end_ptr = 0;
		if (strlen(start_ptr) == CLLI_LEN)
		{
			/* action for legal string */
			DP("TRKGRP = %d, CLLI = %s\n", trkgrp, start_ptr);
		}
    }

    return ROK;
}

PUBLIC S16 read_ISUP_CUST_ATCA()
{
    FILE *fhandle = NULL;
    S8 line_buf[256];
    S8 *start_ptr;
    S8 *end_ptr;
	U16 trkgrp;
	U16 i;

    fhandle = fopen("ISUP_CUST", "r");
    if (fhandle == NULL)
    {
        DP("ISUP_CUST file does not exist\n");
        return ROK;
    }

	/* read the file line by line */
	while (fgets(line_buf, 256, fhandle) != NULL)
	{
		i = 0;  
		trkgrp = 0;

        start_ptr = line_buf;
        /* ignore blank space */
        while (*start_ptr == ' ')
        {
            start_ptr++;
        }
        
        /* look for the trkgrp number */ 
        while ((*start_ptr >= '0') && (*start_ptr <= '9') && (i < 4))
        {
			trkgrp = (trkgrp * 10) + (*start_ptr - '0');
			i++;
            start_ptr++;
        }

		if ((*start_ptr == ',') && (trkgrp > 0))
		{
		    /* found trkgrp number */
		    start_ptr++;

            /* ignore blank space */
            while (*start_ptr == ' ')
            {
                start_ptr++;
            }

		    end_ptr = start_ptr;
		    /* look for the end position of the key string */
		    while ((*end_ptr != ' ') &&
                   (*end_ptr != '\n') &&
                   (*end_ptr != 0))
		    {
		    	end_ptr++;
		    }

		    *end_ptr = 0;
            if (strlen(start_ptr) == CLLI_LEN)
            {
            	/* found CLLI string */
            	DP("TRKGRP = %d, CLLI = %s\n", trkgrp, start_ptr);
            }

			/* stop searching */
			continue;
		}

		DP("no match and ready to find other strings\n");
    }

    return ROK;
}
