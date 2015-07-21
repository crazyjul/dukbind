#pragma once

#include <type_traits>

namespace dukbind
{
    namespace glue
    {
        template<typename _FunctionType_>
        struct function_glue;

        template<typename _Result_ >
        struct function_glue<_Result_ (*)()>
        {
            template<_Result_ (*_Function_)()>
            static duk_ret_t function( duk_context * ctx )
            {
                Push( ctx, _Function_() );
                return 1;
            }
        };

        template<typename _Result_, typename _Arg0 >
        struct function_glue<_Result_ (*)( _Arg0 )>
        {
            template<_Result_ (*_Function_)( _Arg0 )>
            static duk_ret_t function( duk_context * ctx )
            {
                Push( ctx,
                    _Function_(
                        Get( ctx, 0, (typename std::remove_reference<_Arg0>::type *)0 )
                        )
                    );
                return 1;
            }
        };

        template<typename _Result_, typename _Arg0, typename _Arg1 >
        struct function_glue<_Result_ (*)( _Arg0, _Arg1 )>
        {
            template<_Result_ (*_Function_)( _Arg0, _Arg1 )>
            static duk_ret_t function( duk_context * ctx )
            {
                Push( ctx,
                    _Function_(
                        Get( ctx, 0, (typename std::remove_reference<_Arg0>::type *)0 ),
                        Get( ctx, 1, (typename std::remove_reference<_Arg1>::type *)0 )
                        )
                    );
                return 1;
            }
        };

        template<typename _Result_, typename _Arg0, typename _Arg1, typename _Arg2 >
        struct function_glue<_Result_ (*)( _Arg0, _Arg1, _Arg2 )>
        {
            template<_Result_ (*_Function_)( _Arg0, _Arg1, _Arg2 )>
            static duk_ret_t function( duk_context * ctx )
            {
                Push( ctx,
                    _Function_(
                        Get( ctx, 0, (typename std::remove_reference<_Arg0>::type *)0 ),
                        Get( ctx, 1, (typename std::remove_reference<_Arg1>::type *)0 ),
                        Get( ctx, 2, (typename std::remove_reference<_Arg2>::type *)0 )
                        )
                    );
                return 1;
            }
        };

        template<typename _Result_, typename _Arg0, typename _Arg1, typename _Arg2, typename _Arg3 >
        struct function_glue<_Result_ (*)( _Arg0, _Arg1, _Arg2, _Arg3 )>
        {
            template<_Result_ (*_Function_)( _Arg0, _Arg1, _Arg2, _Arg3 )>
            static duk_ret_t function( duk_context * ctx )
            {
                Push( ctx,
                    _Function_(
                        Get( ctx, 0, (typename std::remove_reference<_Arg0>::type *)0 ),
                        Get( ctx, 1, (typename std::remove_reference<_Arg1>::type *)0 ),
                        Get( ctx, 2, (typename std::remove_reference<_Arg2>::type *)0 ),
                        Get( ctx, 3, (typename std::remove_reference<_Arg3>::type *)0 )
                        )
                    );
                return 1;
            }
        };
    }
}