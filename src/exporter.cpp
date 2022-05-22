
#define DYNALO_EXPORT_SYMBOLS
#include <dynalo/symbol_helper.hpp>

#include "exporter.hpp"

DYNALO_EXPORT std::unordered_map<std::string, hr::ExportFunction> DYNALO_CALL GetModuleInfo()
{
   return hr::Registrar::Get().GetInfo();
}

namespace hr
{
   std::unordered_map<std::string, ExportFunction> Registrar::GetInfo()
   {
      return mFunctions;
   }
}