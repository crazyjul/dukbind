#include <catch.hpp>
#include <duktape.h>
#include <dukbind.h>
#include "dukbind_binding_info.h"

static duk_ret_t dummy( duk_context * )
{
    return 0;
}

TEST_CASE( "Bindings handle class correctly ", "[bindings][class]" )
{
    dukbind::BindingInfo info;

    SECTION( "Class can't be registered twice" )
    {
        info.AddClass( "A", 1234 );

        REQUIRE_THROWS( info.AddClass( "A", 5678 ) );
        REQUIRE_THROWS( info.AddClass( "B", 1234 ) );
    }

    SECTION( "Class should be valid" )
    {
        REQUIRE_THROWS( info.AddClass( 0, 5678 ) );
        REQUIRE_THROWS( info.AddClass( "", 5678 ) );
        REQUIRE_THROWS( info.AddClass( "A", 0 ) );
    }

    SECTION( "Class Method can't be registered twice" )
    {
        info.AddClass( "A", 1234 );
        info.AddMethod( 1234, "SomeMethod", &dummy );
        REQUIRE_THROWS( info.AddMethod( 1234, "SomeMethod", &dummy ) );
    }

    SECTION( "Class Method requires registered class" )
    {
        REQUIRE_THROWS( info.AddMethod( 1234, "SomeMethod", &dummy ) );
    }

    SECTION( "Class Method cannot be null" )
    {
        info.AddClass( "A", 1234 );
        REQUIRE_THROWS( info.AddMethod( 1234, "SomeMethod", 0 ) );
    }

    SECTION( "Class Method should have a valid name" )
    {
        info.AddClass( "A", 1234 );
        REQUIRE_THROWS( info.AddMethod( 1234, 0, &dummy ) );
        REQUIRE_THROWS( info.AddMethod( 1234, "", &dummy ) );
        REQUIRE_THROWS( info.AddMethod( 1234, "Some Method", &dummy ) );
        REQUIRE_THROWS( info.AddMethod( 1234, "Some+Method", &dummy ) );
    }
}

TEST_CASE( "Querying class methods works", "[bindings][class]" )
{
    dukbind::BindingInfo info;
    info.AddClass( "A", 1234 );
    info.AddMethod( 1234, "SomeMethod", &dummy );

    SECTION( "Recover correct function" )
    {
        REQUIRE( info.GetClassMethod( 1234, "SomeMethod" ) == &dummy );
    }

    SECTION( "Does not crash when class id is wrong" )
    {
        REQUIRE( info.GetClassMethod( 5678, "SomeMethod" ) == 0 );
    }

    SECTION( "Does not crash when function name is wrong" )
    {
        REQUIRE( info.GetClassMethod( 1234, "SomeMethod2" ) == 0 );
    }
}