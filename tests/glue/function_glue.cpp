#include <catch.hpp>
#include <duktape.h>
#include <dukbind.h>
#include <dukbind_glue.h>

static float GetFloat()
{
    return 1.234f;
}

static int GetInt()
{
    return 1234;
}

static const char * GetText()
{
    return "Hello world";
}

static void SetText( const char * value )
{
    REQUIRE( strcmp( value, "Hello world" ) == 0 );
}

static void SetFloat( float value )
{
    REQUIRE( value == 1.25f );
}

static void SetInt( const int & value )
{
    REQUIRE( value == 1234 );
}

static float GetMinimum( float a, const float & b )
{
    return ( a > b ) ? b : a;
}

TEST_CASE( "Glue functions are working", "[binding][glue]" )
{
    duk_context * ctx = duk_create_heap_default();

    dukbind::BindingInfo info;

    SECTION( "Result functions" )
    {
        info.AddFunction(
            "GetFloat",
            &dukbind::glue::function_glue<decltype(&GetFloat)>::function<&GetFloat>
            );
        info.AddFunction(
            "GetInt",
            &dukbind::glue::function_glue<decltype(&GetInt)>::function<&GetInt>
            );

        info.AddFunction(
            "GetText",
            &dukbind::glue::function_glue<decltype(&GetText)>::function<&GetText>
            );

        dukbind::Setup( ctx, info, "Module" );
        duk_eval_string( ctx, "x = Module.GetFloat(); y = Module.GetInt(); z = Module.GetText()" );
        duk_push_global_object( ctx );

        duk_get_prop_string( ctx, -1, "x" );
        REQUIRE( duk_is_number( ctx, -1 ) );
        REQUIRE( 1.234f == duk_to_number( ctx, -1 ) );
        duk_pop( ctx );

        duk_get_prop_string( ctx, -1, "y" );
        REQUIRE( duk_is_number( ctx, -1 ) );
        REQUIRE( 1234.0f == duk_to_number( ctx, -1 ) );
        duk_pop( ctx );

        duk_get_prop_string( ctx, -1, "z" );
        REQUIRE( duk_is_string( ctx, -1 ) );
        REQUIRE( strcmp( "Hello world", duk_to_string( ctx, -1 ) ) == 0 );
        duk_pop( ctx );
    }

    SECTION( "Void functions with 1 arg" )
    {
        info.AddFunction(
            "SetFloat",
            &dukbind::glue::function_glue<decltype(&SetFloat)>::function<&SetFloat>
            );
        info.AddFunction(
            "SetInt",
            &dukbind::glue::function_glue<decltype(&SetInt)>::function<&SetInt>
            );

        info.AddFunction(
            "SetText",
            &dukbind::glue::function_glue<decltype(&SetText)>::function<&SetText>
            );

        dukbind::Setup( ctx, info, "Module" );
        duk_eval_string_noresult( ctx, "Module.SetFloat(1.25); Module.SetInt(1234); Module.SetText('Hello world')" );
    }

    SECTION( "Result function with 2 arguments")
    {
        info.AddFunction(
            "GetMinimum",
            &dukbind::glue::function_glue<decltype(&GetMinimum)>::function<&GetMinimum>
            );

        dukbind::Setup( ctx, info, "Module" );
        duk_eval_string( ctx, "x = Module.GetMinimum( 1, 2 );" );
        duk_push_global_object( ctx );
        duk_get_prop_string( ctx, -1, "x" );

        REQUIRE( duk_is_number( ctx, -1 ) );
        REQUIRE( 1.0 == duk_to_number( ctx, -1 ) );
    }

    duk_destroy_heap( ctx );
}

