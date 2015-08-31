#include "catch.hpp"
#include "duktape.h"
#include "dukbind.h"

struct Test
{
    Test( const int data ) : Data( data ) {}
    Test( const Test & other )
    {
        Data = other.Data;
        ++CopyConstructorCount;
    }

    ~Test()
    {
        ++DestructorCount;
    }

    int Data;

    static size_t CopyConstructorCount;
    static size_t DestructorCount;
};

size_t Test::CopyConstructorCount = 0;
size_t Test::DestructorCount = 0;

dukbind_bind_as_copy( Test )

static bool DoStuffIsCalled = false;

duk_ret_t DoStuff( duk_context * )
{
    DoStuffIsCalled = true;
    return 0;
}

duk_ret_t CheckThis( duk_context * ctx )
{

    duk_push_this( ctx );

    const Test & result = dukbind::Get( ctx, -1, (Test*)0 );

    REQUIRE( result.Data == 5678 );

    return 0;
}

TEST_CASE( "Class can be passed as copy", "[binding][class]" )
{
    duk_context * ctx = duk_create_heap_default();

    dukbind::BindingInfo info;

    info.AddClass( "Test", dukbind::rtti::GetTypeIndex<Test>() );
    info.AddMethod( dukbind::rtti::GetTypeIndex<Test>(), "DoStuff", DoStuff );
    info.AddMethod( dukbind::rtti::GetTypeIndex<Test>(), "CheckThis", CheckThis );

    dukbind::Setup( ctx, info, "Module" );

    SECTION( "Instance is copied when pushed" )
    {
        Test data( 1234 );
        Test::CopyConstructorCount = 0;
        dukbind::Push( ctx, data, (Test*)0 );
        REQUIRE( Test::CopyConstructorCount == 1 );
    }

    SECTION( "Instance is copied when pushed" )
    {
        Test data( 5678 );

        dukbind::Push( ctx, data, (Test*)0 );

        const Test & result = dukbind::Get( ctx, -1, (Test*)0 );

        REQUIRE( result.Data == 5678 );
    }

    SECTION( "Instance is destructed when garbage collected" )
    {
        Test data( 1234 );
        dukbind::Push( ctx, data, (Test*)0 );
        Test::DestructorCount = 0;

        duk_pop( ctx );
        duk_gc( ctx, 0 );
        duk_gc( ctx, 0 );
        REQUIRE( Test::DestructorCount == 1 );
    }

    SECTION( "Binding is called" )
    {
        Test data( 1234 );
        duk_push_global_object( ctx );
        dukbind::Push( ctx, data, (Test*)0 );
        duk_put_prop_string( ctx, -2, "data" );

        duk_eval_string_noresult( ctx, "data.DoStuff()" );

        REQUIRE( DoStuffIsCalled );
    }

    SECTION( "This is valid" )
    {
        Test data( 5678 );
        duk_push_global_object( ctx );
        dukbind::Push( ctx, data, (Test*)0 );
        duk_put_prop_string( ctx, -2, "data" );

        duk_eval_string_noresult( ctx, "data.CheckThis()" );
    }

    duk_destroy_heap( ctx );
}

