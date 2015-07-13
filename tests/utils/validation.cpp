#include <catch.hpp>
#include "utils/dukbind_validation.h"

TEST_CASE( "Identifier is checked", "[utils][validation]" )
{
    REQUIRE( dukbind::validation::IsValidIdentifier( "a" ) );
    REQUIRE( dukbind::validation::IsValidIdentifier( "A" ) );
    REQUIRE( dukbind::validation::IsValidIdentifier( "$" ) );
    REQUIRE( dukbind::validation::IsValidIdentifier( "_" ) );
    REQUIRE( dukbind::validation::IsValidIdentifier( "abcdefghijklmnopqrstuvwxyz" ) );
    REQUIRE( dukbind::validation::IsValidIdentifier( "_$ABCDEFGHIJKLMNOPQRSTUVWXYZ" ) );
    REQUIRE( dukbind::validation::IsValidIdentifier( "$0123456789" ) );
    REQUIRE( !dukbind::validation::IsValidIdentifier( "1" ) );
    REQUIRE( !dukbind::validation::IsValidIdentifier( "2abc" ) );
    REQUIRE( !dukbind::validation::IsValidIdentifier( "a b" ) );
    REQUIRE( !dukbind::validation::IsValidIdentifier( "a'" ) );
    REQUIRE( !dukbind::validation::IsValidIdentifier( "+" ) );
    REQUIRE( !dukbind::validation::IsValidIdentifier( "-" ) );
    REQUIRE( !dukbind::validation::IsValidIdentifier( "*" ) );
    REQUIRE( !dukbind::validation::IsValidIdentifier( "/" ) );
}