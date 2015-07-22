#include <catch.hpp>
#include <utils/dukbind_identifier.h>

TEST_CASE( "Identifier behaves as expected", "[identifier]" )
{
    SECTION( "Empty identifier has a hash code of 0" )
    {
    	dukbind::Identifier a, b( "" ), c( (const char *)0 ), d( 0u );

    	REQUIRE( a.GetHashCode() == 0 );
    	REQUIRE( b.GetHashCode() == 0 );
    	REQUIRE( c.GetHashCode() == 0 );
    	REQUIRE( d.GetHashCode() == 0 );
        REQUIRE( dukbind::hash::GetHashCodeCT( "" ) == 0 );
    }

    SECTION( "Compile generated identifier is equal to run-time generated one" )
    {
        REQUIRE( dukbind::hash::GetHashCodeCT( "Hello world" ) == dukbind::Identifier( "Hello world" ).GetHashCode() );
    }
}
