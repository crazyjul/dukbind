#include "catch.hpp"
#include "duktape.h"
#include "dukbind.h"
#include <limits>

TEST_CASE( "Primitives can be pushed into a context", "[primitives][push]" )
{
    duk_context * ctx = duk_create_heap_default();

    SECTION( "boolean are pushed" )
    {
        dukbind::Push( ctx, true, (bool*)0 );

        REQUIRE( duk_is_boolean( ctx, -1 ) );
        REQUIRE( duk_to_boolean( ctx, -1 ) );
    }

    SECTION( "Char strings are pushed" )
    {
        const char * value = "Hello world";
        dukbind::Push( ctx, value,( const char ** )0 );

        REQUIRE( duk_is_string( ctx, -1 ) );
        REQUIRE( strcmp( value, duk_to_string( ctx, -1 ) ) == 0 );
    }

    SECTION( "ints are pushed" )
    {
        const int value = 1234567;
        dukbind::Push( ctx, value, (int*)0 );

        REQUIRE( duk_is_number( ctx, -1 ) );
        REQUIRE( duk_to_number( ctx, -1 ) == (duk_double_t)value );
    }

    SECTION( "floats are pushed" )
    {
        const float value = 1234567.8910f;
        dukbind::Push( ctx, value, (float*)0 );

        REQUIRE( duk_is_number( ctx, -1 ) );
        REQUIRE( duk_to_number( ctx, -1 ) == (duk_double_t)value );
    }

    SECTION( "double are pushed" )
    {
        const double value = 1234567.8910f;
        dukbind::Push( ctx, value, (double*)0 );

        REQUIRE( duk_is_number( ctx, -1 ) );
        REQUIRE( duk_to_number( ctx, -1 ) == (duk_double_t)value );
    }

    duk_destroy_heap( ctx );
}