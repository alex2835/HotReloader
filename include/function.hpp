

namespace hr
{

class HotReloader;

template<typename S>
class Function;

template<typename Ret, typename... Args>
class Function<Ret(Args...)>
{
using FunctionType =  Ret( Args... );

public:
   Ret operator() ( Args&&... args )
   {
      return mFunction( std::forward<Args>( args )... );
   }

   Function( FunctionType* func, int version )
      : mFunction( func ),
        mFunctionVersion( version )
   {}

private:
   FunctionType* mFunction;
   HotReloader* HotReloader;
   int mFunctionVersion;
};


}