#include "catch.hpp"
#include "utils/dukbind_rtti.h"

namespace
{
    struct A
    {

    };

    struct B
    {

    };

    struct C
    {
        virtual ~C(){}
    };

    struct D : C
    {
    };
}

TEST_CASE( "Rtti system behaves as expected", "[rtti]" )
{
    SECTION( "Non virtual classes have valid rtti index" )
    {
        A a;
        B b;

        REQUIRE( dukbind::rtti::GetTypeIndex<A>() != dukbind::rtti::GetTypeIndex<B>() );
        REQUIRE( dukbind::rtti::GetTypeIndex<A>() == dukbind::rtti::GetInstanceIndex( a ) );
        REQUIRE( dukbind::rtti::GetTypeIndex<B>() == dukbind::rtti::GetInstanceIndex( b ) );
    }

    SECTION( "Virtual classes have valid rtti index" )
    {
        C c;
        D d;
        C * c_or_d = &d;

        REQUIRE( dukbind::rtti::GetTypeIndex<C>() != dukbind::rtti::GetTypeIndex<D>() );
        REQUIRE( dukbind::rtti::GetTypeIndex<C>() == dukbind::rtti::GetInstanceIndex( c ) );
        REQUIRE( dukbind::rtti::GetTypeIndex<D>() == dukbind::rtti::GetInstanceIndex( d ) );
        REQUIRE( dukbind::rtti::GetTypeIndex<D>() == dukbind::rtti::GetInstanceIndex( *c_or_d ) );
    }
}