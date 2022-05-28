
#include "export.hpp"


int mul( int a, int b )
{
   return a * b;
}
REGISTRATE_FUNC( mul, int, int, int )