#include "catch.hpp"
#include "duktape.h"
#include "dukbind.h"
#include "dukbind_binding_info.h"

static duk_ret_t dummy_function( duk_context * )
{
    return 0;
}

static duk_ret_t dummy_function2( duk_context * )
{
    return 1;
}

TEST_CASE( "Bindings handle function correctly ", "[bindings][functions]" )
{
    dukbind::BindingInfo info;

    SECTION( "Adding function with empty name asserts" )
    {
        REQUIRE_THROWS( info.AddFunction( "", dummy_function ) );
        REQUIRE_THROWS( info.AddFunction( 0, dummy_function ) );
    }

    SECTION( "Adding null function asserts" )
    {
        REQUIRE_THROWS( info.AddFunction( "Hello", 0 ) );
    }

    SECTION( "Adding function with invalid name asserts" )
    {
        REQUIRE_THROWS( info.AddFunction( "123", dummy_function ) );
        REQUIRE_THROWS( info.AddFunction( "A b", dummy_function ) );
        REQUIRE_THROWS( info.AddFunction( "$1%", dummy_function ) );
    }

    SECTION( "Functions are queried by name" )
    {
        info.AddFunction( "One", dummy_function );
        info.AddFunction( "Two", dummy_function2 );

        REQUIRE( info.GetFunction( "One" ) == &dummy_function );
        REQUIRE( info.GetFunction( "Two" ) == &dummy_function2 );
    }

    SECTION( "GetFunction returns 0 if not found" )
    {
        REQUIRE( info.GetFunction( "One" ) == 0 );
        REQUIRE( info.GetFunction( "Two" ) == 0 );
    }
}