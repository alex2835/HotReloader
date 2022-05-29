#include <import.hpp>

namespace hr
{
   HotReloader::HotReloader( fs::path path )
   {
      // Directory where dll copies are stored
      if (std::filesystem::exists(CACHE_DIR))
         fs::remove_all(CACHE_DIR);
      fs::create_directory(CACHE_DIR);

      fs::path name;
      fs::path library_dir = "./";

      if (path.has_root_path())
      {
         path = path.filename();
         library_dir = path.parent_path();
      }
      
      mLibraryName = path.string();
      TryUpdate();
   }

   bool HotReloader::TryUpdate()
   {
      fs::file_time_type lib_update_time = mLastUpdateTime;
      lib_update_time = fs::last_write_time( GetInputPath() );

      if( mLastUpdateTime != lib_update_time || mLibraryVersions.empty() )
      {
         for( int i = 0; i < 10; i++ )
         {
            try
            {
               std::string input_path = GetInputPath();
               std::string output_path = GetOutputPath();
               fs::copy( input_path, output_path );
               mLibraryVersions.push_back( dynalo::library( output_path ) );
               mLastUpdateTime = lib_update_time;
               mFunctionCache.clear();
               mLibraryMeta = ExtractLibraryMeta();
               return true;
            }
            catch ( const std::exception& /*e*/ )
            {
               if( i > 8 )
                  throw;
            }
            std::this_thread::sleep_for( RELOAD_DELLEY );
         }
      }

      return false;
   }

   std::string HotReloader::GetInputPath()
   {
      return dynalo::to_native_name( mLibraryName );
   }

   std::string HotReloader::GetOutputPath()
   {
      auto version = std::to_string( GetLibraryVersion() );
      auto full_lib_name = mLibraryName + version;
      return dynalo::to_native_name(CACHE_DIR + full_lib_name );
   }

   int HotReloader::GetLibraryVersion()
   {
      return static_cast<int>( mLibraryVersions.size() );
   }

   dynalo::library& HotReloader::GetActiveLibrary()
   {
      return mLibraryVersions.back();
   }

   LibraryMeta HotReloader::ExtractLibraryMeta()
   {
      auto func = GetActiveLibrary().get_function<LibraryMeta()>( "HR_GetModuleInfo" );
      return func();
   }

}