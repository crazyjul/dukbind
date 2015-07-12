#pragma once

#ifndef DUKTAPE_H_INCLUDED
    #error Duktape.h should be included before this header
#endif

#include "dukbindconf.h"

namespace dukbind
{

    #ifdef DUKBIND_TEST
        struct Exception {};
        #define dukbind_assert( _condition_, _message_ ) { if( !( _condition_ ) ){ throw Exception(); } }
    #elif defined DEBUG
        // :TODO: Improve
        #define dukbind_assert( _condition_, _message_ ) { if( !( _condition_ ) ){ assert( false ); } }
    #else
        #define dukbind_assert( _condition_, _message ) { sizeof(_condition_) }
    #endif

    void Push( duk_context * ctx, const char * value );
    void Push( duk_context * ctx, const int value );
    void Push( duk_context * ctx, const float value );
    void Push( duk_context * ctx, const double value );


    const char * Get( duk_context * ctx, const int index, const char ** );
    int Get( duk_context * ctx, const int index, const int * );
    float Get( duk_context * ctx, const int index, const float * );
    double Get( duk_context * ctx, const int index, const double * );
}