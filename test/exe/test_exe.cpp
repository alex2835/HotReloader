
#include "hot_reloader.hpp"

void main()
{
   hr::HotReloader hr( "test" );

   hr.GetFunction<void()>( "temp" );
}