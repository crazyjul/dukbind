#include "catch.hpp"
#include "duktape.h"
#include "dukbind.h"

static duk_ret_t PassTrough( duk_context * )
{
    return 1;
}

TEST_CASE( "Global functions binding", "[binding][globals]" )
{
    duk_context * ctx = duk_create_heap_default();
    const char * value = "Hello world";

    duk_push_global_object( ctx );
    duk_push_string( ctx, value );
    duk_put_prop_string( ctx, -2, "Text" );

    duk_push_int( ctx, 2 );
    duk_put_prop_string( ctx, -2, "Int" );

    dukbind::BindingInfo info;

    info.AddFunction( "PassTrough", PassTrough );


    SECTION( "Global setup does not prevent accessing globals" )
    {

        dukbind::Setup( ctx, info );
        // :TODO: This won't work due to a bug in duktape, restore when fixed
        //duk_eval_string(ctx, "print('');");
    }

    SECTION( "Module setup does not prevent accessing globals" )
    {
        dukbind::Setup( ctx, info, "Module" );
        duk_eval_string(ctx, "print('');");
    }


    SECTION( "Global setup allow access to function" )
    {
        dukbind::Setup( ctx, info );
        // :TODO: This won't work due to a bug in duktape, restore when fixed
        //duk_eval_string(ctx, "PassTrough( a );");
    }

    SECTION( "Module access to globlas")
    {
        dukbind::Setup( ctx, info, "Module" );
        duk_eval_string( ctx, "x = Module.PassTrough( 1 );" );
        duk_push_global_object( ctx );
        duk_get_prop_string( ctx, -1, "x" );

        REQUIRE( duk_is_number( ctx, -1 ) );
        REQUIRE( 1.0 == duk_to_number( ctx, -1 ) );
    }

    duk_destroy_heap( ctx );
}

