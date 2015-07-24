#include <catch.hpp>
#include <duktape.h>
#include <dukbind.h>
#include <limits>

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

template<>
struct dukbind::bind_as_copy<Test>
{
    static const bool value = true;
};

TEST_CASE( "Class can be passed as copy", "[binding][class]" )
{
    duk_context * ctx = duk_create_heap_default();

    dukbind::BindingInfo info;
    dukbind::Setup( ctx, info, "Module" );

    SECTION( "Instance is copied when pushed" )
    {
        Test data( 1234 );
        Test::CopyConstructorCount = 0;
        dukbind::Push( ctx, data );
        REQUIRE( Test::CopyConstructorCount == 1 );
    }

    SECTION( "Instance is copied when pushed" )
    {
        Test data( 5678 );

        dukbind::Push( ctx, data );

        const Test & result = dukbind::Get( ctx, -1, (Test*)0 );

        REQUIRE( result.Data == 5678 );
    }

    SECTION( "Instance is destructed when garbage collected" )
    {
        Test data( 1234 );
        dukbind::Push( ctx, data );
        Test::DestructorCount = 0;

        duk_pop( ctx );
        duk_gc( ctx, 0 );
        duk_gc( ctx, 0 );
        REQUIRE( Test::DestructorCount == 1 );
    }

    duk_destroy_heap( ctx );
}