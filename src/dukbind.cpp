#include <duktape.h>
#include "dukbind.h"

namespace dukbind
{
    void Push( duk_context * ctx, const char * value )
    {
        duk_push_string( ctx, value );
    }

    void Push( duk_context * ctx, const int value )
    {
        dukbind_assert(
            ( (int)(duk_double_t) value ) == value,
            "Value does not fit into a duk_double_t"
            );

        duk_push_number( ctx, (duk_double_t)value );
    }
    void Push( duk_context * ctx, const float value )
    {
        dukbind_assert(
            ( (float)(duk_double_t) value ) == value,
            "Value does not fit into a duk_double_t"
            );

         duk_push_number( ctx, (duk_double_t)value );
    }
    void Push( duk_context * ctx, const double value )
    {
        dukbind_assert(
            ( (double)(duk_double_t) value ) == value,
            "Value does not fit into a duk_double_t"
            );
         duk_push_number( ctx, (duk_double_t)value );
    }
}