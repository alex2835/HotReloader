#pragma once 
#include <vector>
#include <string>
#include <unordered_map>

namespace hr
{

struct FunctionMeta
{
   std::string mReturnType;
   std::vector<std::string> mArgsTypes;

   std::string ToString() const
   {
      int i = 0;
      std::string func_def = mReturnType + " (";
      for( const auto& arg : mArgsTypes )
         func_def += ( i++ ? ", " : "" ) + arg;
      func_def += ")";
      return func_def;
   }
};

}

typedef std::unordered_map<std::string, hr::FunctionMeta> LibraryMeta;
