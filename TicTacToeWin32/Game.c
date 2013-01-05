#include "Game.h"



/* convert int to binary string */
char* itobs (unsigned int n, char* ps)
{
   int i;
   static int size = 8 * sizeof (unsigned int); // size is 32

   for (i = size - 1; i >= 0; i--, n >>= 1)
      ps[i] = (01 & n) + '0';
   ps[size] = '\0';

   return ps;
}
