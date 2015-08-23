#include "catch.hpp"
#include "duktape.h"
#include "dukbind.h"
#include <limits>

static duk_ret_t dummy_function( duk_context * )
{
    return 0;
}

TEST_CASE( "When binding is registered, globals are still accessible", "[binding][globals]" )
{
    duk_context * ctx = duk_create_heap_default();
    const char * value = "Hello world";

    duk_push_global_object( ctx );
    duk_push_string( ctx, value );
    duk_put_prop_string( ctx, -2, "Text" );

    duk_push_int( ctx, 2 );
    duk_put_prop_string( ctx, -2, "Int" );

    dukbind::BindingInfo info;

    info.AddFunction( "FunctionName", dummy_function );

    dukbind::Setup( ctx, info );

    SECTION( "Setup does not prevent accessing globals" )
    {
        duk_get_prop_string( ctx, -1, "Text" );
        CHECK( strcmp( value, duk_to_string( ctx, -1 ) ) == 0 );
        duk_pop( ctx );

        duk_get_prop_string( ctx, -1, "Int" );
        CHECK( duk_to_number( ctx, -1 ) == 2 );
        duk_pop( ctx );
    }

    SECTION( "Setup does not prevent deleting globals" )
    {
        duk_del_prop_string( ctx, -1, "Text" );
        CHECK( !duk_has_prop_string( ctx, -1, "Text" ) );
    }

    SECTION( "Setup does not prevent setting globals" )
    {
        duk_push_global_object( ctx );
        duk_push_string( ctx, "Another world" );
        duk_put_prop_string( ctx, -2, "Text" );
        duk_pop( ctx );

        duk_get_prop_string( ctx, -1, "Text" );
        CHECK( strcmp( duk_to_string( ctx, -1 ), "Another world" ) == 0 );
    }

    SECTION( "Binding does not hide global variables" )
    {
        duk_push_global_object( ctx );
        duk_push_string( ctx, "Value" );
        duk_put_prop_string( ctx, -2, "FunctionName" );
        duk_pop( ctx );

        duk_get_prop_string( ctx, -1, "FunctionName" );

        CHECK( duk_is_string( ctx, -1 ) );
        CHECK( strcmp( duk_to_string( ctx, -1 ), "Value" ) == 0 );
    }

    duk_destroy_heap( ctx );
}