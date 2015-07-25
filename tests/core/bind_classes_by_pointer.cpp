#include <catch.hpp>
#include <duktape.h>
#include <dukbind.h>

struct TestPointer
{
    TestPointer( const int data ) : Data( data ) {}
    TestPointer( const TestPointer & other )
    {
        Data = other.Data;
        ++CopyConstructorCount;
    }

    ~TestPointer()
    {
        ++DestructorCount;
    }

    int Data;

    static size_t CopyConstructorCount;
    static size_t DestructorCount;
};

size_t TestPointer::CopyConstructorCount = 0;
size_t TestPointer::DestructorCount = 0;

dukbind_bind_as_raw_pointer( TestPointer )

static bool DoStuffIsCalled = false;

static duk_ret_t DoStuff( duk_context * )
{
    DoStuffIsCalled = true;
    return 0;
}

static duk_ret_t CheckThis( duk_context * ctx )
{
    duk_push_this( ctx );

    const TestPointer & result = dukbind::Get( ctx, -1, (TestPointer*)0 );

    REQUIRE( result.Data == 5678 );

    return 0;
}

TEST_CASE( "Class can be passed as pointer", "[binding][class]" )
{
    duk_context * ctx = duk_create_heap_default();

    dukbind::BindingInfo info;

    info.AddClass( "TestPointer", dukbind::rtti::GetTypeIndex<TestPointer>() );
    info.AddMethod( dukbind::rtti::GetTypeIndex<TestPointer>(), "DoStuff", DoStuff );
    info.AddMethod( dukbind::rtti::GetTypeIndex<TestPointer>(), "CheckThis", CheckThis );

    dukbind::Setup( ctx, info, "Module" );

    SECTION( "Instance is not copied when pushed" )
    {
        TestPointer data( 1234 );
        TestPointer::CopyConstructorCount = 0;
        dukbind::Push( ctx, data );
        REQUIRE( TestPointer::CopyConstructorCount == 0 );
    }

    SECTION( "Instance is not copied when pushed" )
    {
        TestPointer data( 5678 );

        dukbind::Push( ctx, data );

        const TestPointer & result = dukbind::Get( ctx, -1, (TestPointer*)0 );

        REQUIRE( &result == &data );
    }

    SECTION( "Instance is not destructed when garbage collected" )
    {
        TestPointer data( 1234 );
        dukbind::Push( ctx, data );
        TestPointer::DestructorCount = 0;

        duk_pop( ctx );
        duk_gc( ctx, 0 );
        duk_gc( ctx, 0 );
        REQUIRE( TestPointer::DestructorCount == 0 );
    }

    SECTION( "Binding is called" )
    {
        TestPointer data( 1234 );
        duk_push_global_object( ctx );
        dukbind::Push( ctx, data );
        duk_put_prop_string( ctx, -2, "data" );

        duk_eval_string_noresult( ctx, "data.DoStuff()" );

        REQUIRE( DoStuffIsCalled );
    }

    SECTION( "This is valid" )
    {
        TestPointer data( 5678 );
        duk_push_global_object( ctx );
        dukbind::Push( ctx, data );
        duk_put_prop_string( ctx, -2, "data" );

        duk_eval_string_noresult( ctx, "data.CheckThis()" );
    }

    duk_destroy_heap( ctx );
}

