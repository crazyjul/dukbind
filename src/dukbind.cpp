#include <duktape.h>
#include "dukbind.h"
#include "dukbindconf.h"

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

    const char * Get( duk_context * ctx, const int index, const char ** )
    {
        dukbind_assert( duk_is_string( ctx, index ), "No conversion is allowed in Get methods" );
        return duk_to_string( ctx, index );
    }

    int Get( duk_context * ctx, const int index, const int * )
    {
        dukbind_assert( duk_is_number( ctx, index ), "No conversion is allowed in Get methods" );

        duk_double_t double_result = duk_to_number( ctx, index );
        int int_result = static_cast<int>( double_result );

        dukbind_assert( double_result == (duk_double_t)int_result, "Number is not a valid integer" );

        return int_result;
    }

    float Get( duk_context * ctx, const int index, const float * )
    {
        dukbind_assert( duk_is_number( ctx, index ), "No conversion is allowed in Get methods" );

        duk_double_t double_result = duk_to_number( ctx, index );
        float float_result = static_cast<float>( double_result );

        #if DUKBIND_NO_FLOAT_CONVERSION
            dukbind_assert( double_result == (duk_double_t)float_result, "Number is not a valid integer" );
        #endif

        return float_result;
    }

    double Get( duk_context * ctx, const int index, const double * )
    {
        dukbind_assert( duk_is_number( ctx, index ), "No conversion is allowed in Get methods" );

        return duk_to_number( ctx, index );
    }
}