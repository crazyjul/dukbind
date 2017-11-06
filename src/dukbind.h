#pragma once

#ifndef DUKTAPE_H_INCLUDED
    #error Duktape.h should be included before this header
#endif

#include "dukbindconf.h"
#include "dukbind_binding_info.h"
#include "utils/dukbind_rtti.h"
#include "dukbind_assert.h"

#include <type_traits>

namespace dukbind
{
    void Push( duk_context * ctx, const bool value );
    void Push( duk_context * ctx, const char * value );
    void Push( duk_context * ctx, const int value );
    void Push( duk_context * ctx, const float value );
    void Push( duk_context * ctx, const double value );

    bool Get( duk_context * ctx, const int index, const bool * );
    const char * Get( duk_context * ctx, const int index, const char ** );
    int Get( duk_context * ctx, const int index, const int * );
    float Get( duk_context * ctx, const int index, const float * );
    double Get( duk_context * ctx, const int index, const double * );

    // Class binding

    typedef void (* finalizer_t )( void * object );

    void * Push( duk_context * ctx, const size_t class_index, const size_t object_size, finalizer_t finalizer );
    void * Push( duk_context * ctx, const size_t class_index, void * object_pointer, finalizer_t finalizer );
    void * Get( duk_context * ctx, duk_idx_t index, size_t & class_index, finalizer_t & finalizer );

    // Binding by copying

    template<typename _Type_>
    void FinalizeObjectCopy( void * object )
    {
        reinterpret_cast<_Type_*>( object )->~_Type_();
    }

    template< typename _Type_ > struct bind_as_copy_traits;

    #define dukbind_bind_as_copy( _Type_ ) namespace dukbind { template<> struct bind_as_copy_traits<_Type_>{static const bool value = true;}; }

    template< typename _Type_ >
    typename std::enable_if< bind_as_copy_traits<_Type_>::value >::type Push( duk_context * ctx, _Type_ && instance )
    {
        dukbind_assert( rtti::GetTypeIndex<_Type_>() == rtti::GetInstanceIndex( instance ), "Instance should be of the exact same type" );
        void * object_memory = Push( ctx, rtti::GetTypeIndex<_Type_>(), sizeof( _Type_ ), &FinalizeObjectCopy<_Type_> );
        new( object_memory ) _Type_( std::forward<_Type_>( instance ) );
    }

    template< typename _Type_ >
    typename std::enable_if< bind_as_copy_traits<_Type_>::value, _Type_ & >::type Get( duk_context * ctx, duk_idx_t index, const _Type_ * )
    {
        size_t class_identifier;
        finalizer_t finalizer;
        void * object_memory = Get( ctx, index, class_identifier, finalizer );
        dukbind_assert( class_identifier == rtti::GetTypeIndex<_Type_>(), "Parameter is of wrong class" );

        return *reinterpret_cast<_Type_*>( object_memory );
    }

    // Binding by pointer

    template< typename _Type_ > struct bind_as_pointer_traits;

    template< typename _Type_ > struct bind_as_raw_pointer
    {
        static const bool value = true;
        static void OnPush( _Type_ & )
        {

        }

        static bool IsValid( _Type_ & )
        {
            return true;
        }

        static void OnFinalize( _Type_ & )
        {

        }
    };

    #define dukbind_bind_as_raw_pointer( _Type_ ) namespace dukbind { template<> struct bind_as_pointer_traits<_Type_>: bind_as_raw_pointer<_Type_>{}; }

    template<typename _Type_>
    void FinalizeObjectPointer( void * object )
    {
        bind_as_pointer_traits<_Type_>::OnFinalize( *reinterpret_cast<_Type_*>( object ) );
    }

    template< typename _Type_ >
    typename std::enable_if< bind_as_pointer_traits<_Type_>::value >::type Push( duk_context * ctx, _Type_ & type )
    {
        Push( ctx, rtti::GetInstanceIndex( type ), &type, &FinalizeObjectPointer<_Type_> );

        bind_as_pointer_traits<_Type_>::OnPush( type );
    }

    template< typename _Type_ >
    typename std::enable_if< bind_as_pointer_traits<_Type_>::value, _Type_ & >::type Get( duk_context * ctx, duk_idx_t index, const _Type_ * )
    {
        size_t class_identifier;
        finalizer_t finalizer;
        void * object_memory = Get( ctx, index, class_identifier, finalizer );

        // Check for base class dukbind_assert( class_identifier == rtti::GetTypeIndex<_Type_>(), "Parameter is of wrong class" );

        dukbind_assert( bind_as_pointer_traits<_Type_>::IsValid( *reinterpret_cast<_Type_*>( object_memory ) ), "Invalid instance" )
        return *reinterpret_cast<_Type_*>( object_memory );
    }

    // Public API

    // Call setup to attach the binding to the context.
    // A module can be passed, registering all methods into a javascript object of given name
    // If no name is given, the binding will be accessible at the root level.
    // The binding use duktape proxy system to resolve methods and constructor funtion.
    // If the binding is inserted at the global scope, the global table is replaced by the proxy that
    // resolves the binding. It is expected to have a performance impact by adding a level of indirection.
    // Inserting the binding into an object can also be used to have the same javascript interface that embind in emscripten

    void Setup( duk_context * ctx, const BindingInfo & info, const char * module = 0 );

}
