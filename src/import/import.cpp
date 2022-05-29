#include <import.hpp>

namespace hr
{
   HotReloader::HotReloader( fs::path path )
   {
      if (path.has_root_path())
      {
         mLibraryName = path.filename().string();
         mLibraryPath = path.parent_path().string();
      }
      else
      {
         mLibraryName = path.string();
         mLibraryPath = "./";
      }

      if( !std::filesystem::exists( CACHE_DIR ) )
         fs::create_directory( CACHE_DIR );

      auto cache_dir = GetCacheDir();
      if( std::filesystem::exists( cache_dir ) )
         fs::remove_all( cache_dir );
      fs::create_directory( cache_dir );
      
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
               mLastUpdateTime = lib_update_time;
               std::string input_path = GetInputPath();
               std::string output_path = GetOutputPath();
               fs::copy( input_path, output_path, std::filesystem::copy_options::overwrite_existing );
               dynalo::library lib ( output_path );
               UpdateMeta( std::move( lib ) );
               mLibraryVersions.push_back( std::move( lib ) );
               mFunctionCache.clear();
               return true;
            }
            catch( const std::exception& /*e*/ )
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
      return dynalo::to_native_name( mLibraryPath + mLibraryName );
   }

   std::string HotReloader::GetOutputPath()
   {
      auto version = std::to_string( GetLibraryVersion() );
      auto full_lib_name = mLibraryName + version;
      return dynalo::to_native_name( GetCacheDir() + full_lib_name );
   }

   std::string HotReloader::GetCacheDir()
   {
      return CACHE_DIR + mLibraryName + "/";
   }

   int HotReloader::GetLibraryVersion()
   {
      return static_cast<int>( mLibraryVersions.size() );
   }

   dynalo::library& HotReloader::GetActiveLibrary()
   {
      return mLibraryVersions.back();
   }

   LibraryMeta HotReloader::ExtractLibraryMeta( dynalo::library& lib )
   {
      auto func = lib.get_function<LibraryMeta()>( "HR_GetModuleInfo" );
      return func();
   }

   void HotReloader::UpdateMeta( dynalo::library& lib )
   {
      auto new_meta = ExtractLibraryMeta( lib );
      for( const auto& [name, func] : new_meta )
      {
         if( mLibraryMeta.count( name ) )
         {
            auto old_sig = mLibraryMeta[name].ToString();
            auto new_sig = func.ToString();
            if( old_sig != new_sig )
               throw std::runtime_error( "Loading failed: New signature is deferent "
                                         "to previous for function: " + name +
                                         "\nprevious: " + old_sig +
                                         "\nnew: " + new_sig );
         }
      }
      mLibraryMeta = std::move( new_meta );
   }

}