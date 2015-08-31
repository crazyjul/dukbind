#pragma once

#include <type_traits>

namespace dukbind
{
    namespace glue
    {
        template<typename _FunctionType_, _FunctionType_ function>
        struct function_glue;

        template<typename _Result_ , _Result_ (*_Function_)()>
        struct function_glue<_Result_ (*)(), _Function_>
        {
            static duk_ret_t function( duk_context * ctx )
            {
                Push( ctx, _Function_(), (_Result_*)0 );
                return 1;
            }
        };

        template<class _Class_, void (_Class_::*_Function_)() >
        struct function_glue<void (_Class_::*)(), _Function_>
        {
            static duk_ret_t function( duk_context * ctx )
            {
                duk_push_this( ctx );
                _Class_ & instance = Get( ctx, -1, (_Class_*)0 );
                duk_pop( ctx );
                (instance.*_Function_)();
                return 0;
            }
        };

        template<typename _Result_, class _Class_, _Result_ (_Class_::*_Function_)() >
        struct function_glue<_Result_ (_Class_::*)(), _Function_>
        {
            static duk_ret_t function( duk_context * ctx )
            {
                duk_push_this( ctx );
                _Class_ & instance = Get( ctx, -1, (_Class_*)0 );
                duk_pop( ctx );
                Push( ctx, (instance.*_Function_)(), (_Result_*)0 );
                return 1;
            }
        };

        template<typename _Result_, class _Class_, _Result_ (_Class_::*_Function_)()const >
        struct function_glue<_Result_ (_Class_::*)() const, _Function_>
        {
            static duk_ret_t function( duk_context * ctx )
            {
                duk_push_this( ctx );
                _Class_ & instance = Get( ctx, -1, (_Class_*)0 );
                duk_pop( ctx );
                Push( ctx, (instance.*_Function_)(), (_Result_*)0 );
                return 1;
            }
        };

        template<void (*_Function_)()>
        struct function_glue<void (*)(), _Function_>
        {
            static duk_ret_t function( duk_context * )
            {
                _Function_();
                return 0;
            }
        };

        template<typename _Result_, typename _Arg0, _Result_ (*_Function_)( _Arg0 ) >
        struct function_glue<_Result_ (*)( _Arg0 ), _Function_>
        {
            static duk_ret_t function( duk_context * ctx )
            {
                Push( ctx,
                    _Function_(
                        Get( ctx, 0, (typename std::remove_reference<_Arg0>::type *)0 )
                        ),
                    (_Result_*)0
                    );
                return 1;
            }
        };

        template<typename _Result_, class _Class_, typename _Arg0, _Result_ (_Class_::*_Function_)( _Arg0 ) >
        struct function_glue<_Result_ (_Class_::*)( _Arg0 ), _Function_>
        {
            static duk_ret_t function( duk_context * ctx )
            {
                duk_push_this( ctx );
                _Class_ & instance = Get( ctx, -1, (_Class_*)0 );
                duk_pop( ctx );
                Push( ctx,
                    (instance.*_Function_)(
                        Get( ctx, 0, (typename std::remove_reference<_Arg0>::type *)0 )
                        ),
                    (_Result_*)0
                    );
                return 1;
            }
        };

        template<class _Class_, typename _Arg0, void (_Class_::*_Function_)( _Arg0 ) >
        struct function_glue<void (_Class_::*)( _Arg0 ), _Function_>
        {
            static duk_ret_t function( duk_context * ctx )
            {
                duk_push_this( ctx );
                _Class_ & instance = Get( ctx, -1, (_Class_*)0 );
                duk_pop( ctx );
                (instance.*_Function_)(
                    Get( ctx, 0, (typename std::remove_reference<_Arg0>::type *)0 )
                    );

                return 0;
            }
        };

        template<typename _Arg0, void (*_Function_)( _Arg0 ) >
        struct function_glue<void (*)( _Arg0 ), _Function_>
        {
            static duk_ret_t function( duk_context * ctx )
            {
                _Function_(
                    Get( ctx, 0, (typename std::remove_reference<_Arg0>::type *)0 )
                    );
                return 0;
            }
        };

        template<typename _Result_, typename _Arg0, typename _Arg1, _Result_ (*_Function_)( _Arg0, _Arg1 ) >
        struct function_glue<_Result_ (*)( _Arg0, _Arg1 ), _Function_>
        {
            static duk_ret_t function( duk_context * ctx )
            {
                Push( ctx,
                    _Function_(
                        Get( ctx, 0, (typename std::remove_reference<_Arg0>::type *)0 ),
                        Get( ctx, 1, (typename std::remove_reference<_Arg1>::type *)0 )
                        ),
                    (_Result_*)0
                    );
                return 1;
            }
        };

        template<typename _Arg0, typename _Arg1, void (*_Function_)( _Arg0, _Arg1 ) >
        struct function_glue<void (*)( _Arg0, _Arg1 ), _Function_>
        {
            static duk_ret_t function( duk_context * ctx )
            {
                _Function_(
                    Get( ctx, 0, (typename std::remove_reference<_Arg0>::type *)0 ),
                    Get( ctx, 1, (typename std::remove_reference<_Arg1>::type *)0 )
                    );
                return 0;
            }
        };

        template<typename _Result_, typename _Arg0, typename _Arg1, typename _Arg2, _Result_ (*_Function_)( _Arg0, _Arg1, _Arg2 ) >
        struct function_glue<_Result_ (*)( _Arg0, _Arg1, _Arg2 ), _Function_>
        {
            static duk_ret_t function( duk_context * ctx )
            {
                Push( ctx,
                    _Function_(
                        Get( ctx, 0, (typename std::remove_reference<_Arg0>::type *)0 ),
                        Get( ctx, 1, (typename std::remove_reference<_Arg1>::type *)0 ),
                        Get( ctx, 2, (typename std::remove_reference<_Arg2>::type *)0 )
                        ),
                    (_Result_*)0
                    );
                return 1;
            }
        };

        template<typename _Arg0, typename _Arg1, typename _Arg2,void (*_Function_)( _Arg0, _Arg1, _Arg2 ) >
        struct function_glue<void (*)( _Arg0, _Arg1, _Arg2 ),_Function_>
        {
            static duk_ret_t function( duk_context * ctx )
            {
                _Function_(
                    Get( ctx, 0, (typename std::remove_reference<_Arg0>::type *)0 ),
                    Get( ctx, 1, (typename std::remove_reference<_Arg1>::type *)0 ),
                    Get( ctx, 2, (typename std::remove_reference<_Arg2>::type *)0 )
                    );
                return 0;
            }
        };

        template<typename _Class_, typename _Arg0, typename _Arg1, typename _Arg2,void (_Class_::*_Function_)( _Arg0, _Arg1, _Arg2 ) >
        struct function_glue<void (_Class_::*)( _Arg0, _Arg1, _Arg2 ),_Function_>
        {
            static duk_ret_t function( duk_context * ctx )
            {
                duk_push_this( ctx );
                _Class_ & instance = Get( ctx, -1, (_Class_*)0 );
                duk_pop( ctx );

                (instance.*_Function_)(
                    Get( ctx, 0, (typename std::remove_reference<_Arg0>::type *)0 ),
                    Get( ctx, 1, (typename std::remove_reference<_Arg1>::type *)0 ),
                    Get( ctx, 2, (typename std::remove_reference<_Arg2>::type *)0 )
                    );
                return 0;
            }
        };

        template<typename _Result_, typename _Arg0, typename _Arg1, typename _Arg2, typename _Arg3,
            _Result_ (*_Function_)( _Arg0, _Arg1, _Arg2, _Arg3 ) >
        struct function_glue<_Result_ (*)( _Arg0, _Arg1, _Arg2, _Arg3 ), _Function_>
        {
            static duk_ret_t function( duk_context * ctx )
            {
                Push( ctx,
                    _Function_(
                        Get( ctx, 0, (typename std::remove_reference<_Arg0>::type *)0 ),
                        Get( ctx, 1, (typename std::remove_reference<_Arg1>::type *)0 ),
                        Get( ctx, 2, (typename std::remove_reference<_Arg2>::type *)0 ),
                        Get( ctx, 3, (typename std::remove_reference<_Arg3>::type *)0 )
                        ),
                    (_Result_*)0
                    );
                return 1;
            }
        };

        template<typename _Arg0, typename _Arg1, typename _Arg2, typename _Arg3,
            void (*_Function_)( _Arg0, _Arg1, _Arg2, _Arg3 )>
        struct function_glue<void (*)( _Arg0, _Arg1, _Arg2, _Arg3 ), _Function_>
        {
            static duk_ret_t function( duk_context * ctx )
            {
                _Function_(
                    Get( ctx, 0, (typename std::remove_reference<_Arg0>::type *)0 ),
                    Get( ctx, 1, (typename std::remove_reference<_Arg1>::type *)0 ),
                    Get( ctx, 2, (typename std::remove_reference<_Arg2>::type *)0 ),
                    Get( ctx, 3, (typename std::remove_reference<_Arg3>::type *)0 )
                    );
                return 0;
            }
        };

        #define dukbind_GetGlue( _function_ ) \
             &dukbind::glue::function_glue<decltype( &_function_ ), &_function_>::function

    }
}