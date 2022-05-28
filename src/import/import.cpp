#include <import.hpp>

namespace hr
{
   HotReloader::HotReloader( const fs::path &path )
   {
      // Directory where dll copies are stored
      if (std::filesystem::exists(CACHE_DIR))
         fs::remove_all(CACHE_DIR);
      fs::create_directory(CACHE_DIR);

      fs::path name;
      fs::path library_dir = "./";

      if (path.has_root_path())
      {
         name = path.filename();
         library_dir = path.parent_path();
      }
      else
         name = path;
      
      mLibraryName = name.string();

      name = dynalo::to_native_name(name.string());
      mLibraryOrigin = library_dir / name;
      mLibraryCopy = CACHE_DIR / name;
      TryUpdate();
   }

   bool HotReloader::TryUpdate()
   {
      fs::file_time_type lib_update_time = mLastUpdateTime;
      try
      {
         lib_update_time = fs::last_write_time( mLibraryOrigin );
      }
      catch( const std::exception& /*e*/ )
      {}

      if( mLastUpdateTime != lib_update_time || mLibraries.empty() )
      {
         for( int i = 0; i < 10; i++ )
         {
            std::this_thread::sleep_for( RELOAD_DELLEY );

            try
            {
               std::string output_path = GetOutputPath();
               fs::copy( mLibraryOrigin, output_path );
               mLibraries.push_back( dynalo::library( output_path ) );
               mLastUpdateTime = lib_update_time;
               mFunctionCache.clear();
               return true;
            }
            catch ( const std::exception& /*e*/ )
            {
               if( i > 9 )
                  throw;
            }
         }
      }
      return false;
   }

   std::string HotReloader::GetOutputPath()
   {
      auto version = std::to_string( GetLibraryVersion() );
      auto full_lib_name = mLibraryName + version;
      return dynalo::to_native_name(CACHE_DIR + full_lib_name );
   }

   int HotReloader::GetLibraryVersion()
   {
      return static_cast<int>( mLibraries.size() );
   }

   dynalo::library& HotReloader::GetActiveLibrary()
   {
      return mLibraries.back();
   }

}