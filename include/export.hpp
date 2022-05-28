#pragma once

#define DYNALO_EXPORT_SYMBOLS
#include <dynalo/symbol_helper.hpp>

#include <boost/preprocessor/variadic.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/tuple.hpp>

#include "details/function_registry.hpp"

namespace hr
{

template< typename Ret, typename ...Args>
class Registarator
{
public:
   Registarator( std::string_view function_name )
   { 
      FunctionRegistry::Get().Registrate<Ret, Args...>( function_name );
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
DYNALO_EXPORT ret BOOST_PP_CAT( hr_, name ) DYNALO_CALL dargs {     \
   return name cargs;                                               \
}                                                                   \
hr::Registarator<ret, types> BOOST_PP_CAT( registrator_, name )( std::string_view( #name ) );