#pragma once

#include "dukbindconf.h"

#include <assert.h>

namespace dukbind
{
    #ifdef DUKBIND_TEST
        struct Exception {};
        #define dukbind_assert( _condition_, _message_ ) { if( !( _condition_ ) ){ throw Exception(); } }
    #elif DUKBIND_DEBUG
        // :TODO: Improve
        #define dukbind_assert( _condition_, _message_ ) { if( !( _condition_ ) ){ assert( false ); } }
    #else
        #define dukbind_assert( _condition_, _message ) { sizeof(_condition_); }
    #endif
}