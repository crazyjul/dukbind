#include "catch.hpp"
#include "duktape.h"
#include "dukbind.h"
#include <limits>

TEST_CASE( "Primitives can be recovered from a context", "[primitives][get]" )
{
    duk_context * ctx = duk_create_heap_default();

    SECTION( "Boolean are recovered" )
    {
        duk_push_boolean( ctx, true );

        REQUIRE( dukbind::Get( ctx, -1, (const bool*)0 ) );

        duk_push_int( ctx, 1 );
        REQUIRE_THROWS( dukbind::Get( ctx, -1, (const bool*)0 ) );

        duk_push_object( ctx );
        REQUIRE_THROWS( dukbind::Get( ctx, -1, (const bool*)0 ) );
    }

    SECTION( "Char strings are recovered" )
    {
        const char * value = "Hello world";

        duk_push_string( ctx, value );

        REQUIRE( strcmp( value, dukbind::Get( ctx, -1, (const char**)0 ) ) == 0 );

        duk_push_int( ctx, 1 );
        REQUIRE_THROWS( dukbind::Get( ctx, -1, (const char**)0 ) );

        duk_push_object( ctx );
        REQUIRE_THROWS( dukbind::Get( ctx, -1, (const char**)0 ) );
    }

    SECTION( "ints are recovered" )
    {
        const int value = 1234567;

        duk_push_int( ctx, value );

        REQUIRE( value == dukbind::Get( ctx, -1, (const int *)0 ) );

        duk_push_number( ctx, 0.1234 );
        REQUIRE_THROWS( dukbind::Get( ctx, -1, (const int *)0 ) );

        duk_push_string( ctx, "abc" );
        REQUIRE_THROWS( dukbind::Get( ctx, -1, (const int *)0 ) );

        duk_push_object( ctx );
        REQUIRE_THROWS( dukbind::Get( ctx, -1, (const int *)0 ) );
    }

    SECTION( "floats are recovered" )
    {
        const float value = 1234567.8910f;

        duk_push_number( ctx, value );

        REQUIRE( value == dukbind::Get( ctx, -1, (const float *)0 ) );

        #if DUKBIND_NO_FLOAT_CONVERSION
            duk_push_number( ctx, 0.3 );
            REQUIRE_THROWS( dukbind::Get( ctx, -1, (const float *)0 ) );
        #endif

        duk_push_string( ctx, "abc" );
        REQUIRE_THROWS( dukbind::Get( ctx, -1, (const float *)0 ) );

        duk_push_object( ctx );
        REQUIRE_THROWS( dukbind::Get( ctx, -1, (const float *)0 ) );
    }

    SECTION( "double are recovered" )
    {
        const double value = 0.3;

        duk_push_number( ctx, value );

        REQUIRE( value == dukbind::Get( ctx, -1, (const double *)0 ) );

        duk_push_string( ctx, "abc" );
        REQUIRE_THROWS( dukbind::Get( ctx, -1, (const double *)0 ) );

        duk_push_object( ctx );
        REQUIRE_THROWS( dukbind::Get( ctx, -1, (const double *)0 ) );
    }

    duk_destroy_heap( ctx );
}