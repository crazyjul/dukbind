#pragma once

#ifndef DUKTAPE_H_INCLUDED
    #error Duktape.h should be included before this header
#endif

#include "dukbindconf.h"
#include "dukbind_binding_info.h"

namespace dukbind
{

    #ifdef DUKBIND_TEST
        struct Exception {};
        #define dukbind_assert( _condition_, _message_ ) { if( !( _condition_ ) ){ throw Exception(); } }
    #elif defined DEBUG
        // :TODO: Improve
        #define dukbind_assert( _condition_, _message_ ) { if( !( _condition_ ) ){ assert( false ); } }
    #else
        #define dukbind_assert( _condition_, _message ) { sizeof(_condition_); }
    #endif

    void Push( duk_context * ctx, const char * value );
    void Push( duk_context * ctx, const int value );
    void Push( duk_context * ctx, const float value );
    void Push( duk_context * ctx, const double value );


    const char * Get( duk_context * ctx, const int index, const char ** );
    int Get( duk_context * ctx, const int index, const int * );
    float Get( duk_context * ctx, const int index, const float * );
    double Get( duk_context * ctx, const int index, const double * );


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
