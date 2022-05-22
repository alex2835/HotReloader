#pragma once

#include <iostream>
#include <string_view>
#include <unordered_map>
#include <typeinfo>

#include <boost/preprocessor/variadic.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/tuple.hpp>

#include "export_function.hpp"


namespace hr
{
using namespace std::literals;

class Registrar
{  
public:
   static Registrar& Get()
   {
      static Registrar registrar;
      return registrar;
   }

   template <typename Ret, typename ...Args>
   void Registrate( std::string_view func_name  )
   {
      std::cout << func_name << std::endl << << std::endl;

      ExportFunction function;
      f.mReturnType = typeid(Ret).name();
      f.mArgsTypes = { typeid(Args).name()... };
      mFunctions[ std::string( func_name ) ] = func;
   }

   std::unordered_map<std::string, ExportFunction> GetInfo();

private:
   Registrar() = default;
   std::unordered_map<std::string, ExportFunction> mFunctions;
};


template< typename Ret, typename ...Args>
class Registarator
{
public:
   Registarator( std::string_view function_name )
   { 
      Registrar::Get().Registrate<Ret, Args...>( function_name );
   }
};

}

#define REGISTRATE_FUNC(name, ret, ...) FOO2(name, ret, __VA_ARGS__, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

#define FOO2(name, ret, types, args) FOO3(name, ret, types, args)
#define FOO3(name, ret, types, args) FOO6(name, ret, types,         \
                              BOOST_PP_SEQ_FOR_EACH_I(FOO4,,args),  \
                              BOOST_PP_SEQ_FOR_EACH_I(FOO5,,args))
#define FOO4(rep, data, index, type) (type BOOST_PP_CAT(arg,index))
#define FOO5(rep, data, index, type) (BOOST_PP_CAT(arg,index))
#define FOO6(name, ret, types, dargs, cargs)                        \
  FOO8(name, ret, types, FOO7(dargs, void), FOO7(cargs, ))
#define FOO7(seq, empty)                                            \
  BOOST_PP_IF(BOOST_PP_SEQ_SIZE(seq),                               \
              BOOST_PP_SEQ_TO_TUPLE(seq), (empty))
#define FOO8(name, ret, types, dargs, cargs)                        \
ret BOOST_PP_CAT( hr_, name ) dargs {                               \
   return name cargs;                                               \
}                                                                   \
Registarator<ret, types> BOOST_PP_CAT( registrator_, name )( BOOST_PP_CAT( #name, sv ) );


