
#include "import.hpp"

#include <iostream>

using namespace hr;

void main()
{
   try
   {
      hr::HotReloader hr( "HotReloaderTestLib" );
      auto info = hr.GetFunction<LibraryMeta()>( "GetModuleInfo" );

      for( const auto& [name, args] : info() )
         std::cout << name << "  " << args.ToString() << std::endl;
   }
   catch(const std::exception& e)
   {
      std::cerr << e.what() << '\n';
   }
}