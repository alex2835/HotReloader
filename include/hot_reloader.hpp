#pragma once

#include <string>
#include <unordered_map>
#include <type_traits>
#include <filesystem>
#include <thread>
#include <memory>

#include <dynalo/dynalo.hpp>
#include "function.hpp"

namespace hr
{
namespace fs = std::filesystem;
using namespace std::chrono_literals;
typedef void (*FunctionPointer)(void);


const std::string CACHE_DIR = "./HR_CACHE/";
const auto RELOAD_DELLEY = 100ms;

class HotReloader
{
public:

   /*
    *  Name without extension
    *  @throw If dll can't be loaded
    */
   HotReloader(const fs::path &path);

   /*
    *  Name without extension
    *  @throw If dll can't be loaded
    */
   bool TryUpdate();


   template <typename FunctionSignature>
   FunctionSignature* GetFunctionRaw(const std::string &name)
   {
      if (mFunctionCache.count(name))
      {
         return reinterpret_cast<FunctionSignature *>(mFunctionCache[name]);
      }
      else
      {
         auto func = GetActiveLibrary().get_function<FunctionSignature>(name);
         mFunctionCache[name] = reinterpret_cast<FunctionPointer>(func);
         return func;
      }
   }

   /*
    *  After hot reloading function pointers may be invalid
    *  @throw If no function with such signature
    */
   template <typename FunctionSignature>
   Function<FunctionSignature> GetFunction(const std::string &name)
   {
      return Function<FunctionSignature>( GetFunctionRaw<FunctionSignature>(name), GetLibraryVersion() );
   }


private:
   std::string GetOutputPath()
   {
      auto lib_name = mLibraryOrigin.filename().string() + std::to_string(GetLibraryVersion());
      return dynalo::to_native_name(CACHE_DIR + lib_name );
   }

   int GetLibraryVersion()
   {
      return static_cast<int>( mLibraries.size() );
   }

   dynalo::library& GetActiveLibrary()
   {
      return mLibraries.back();
   }

   std::vector<dynalo::library> mLibraries;
   fs::path mLibraryOrigin;
   fs::path mLibraryCopy;
   fs::file_time_type mLastUpdateTime;
   std::unordered_map<std::string, FunctionPointer> mFunctionCache;
};


}