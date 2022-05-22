#pragma once 
#include <vector>
#include <string>

namespace hr
{

struct ExportFunction
{
   std::string mReturnType;
   std::vector<std::string> mArgsTypes;
};

}