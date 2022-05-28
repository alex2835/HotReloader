
#include "export.hpp"


DYNALO_EXPORT LibraryMeta DYNALO_CALL GetModuleInfo()
{
   return hr::FunctionRegistry::Get().GetInfo();
}


