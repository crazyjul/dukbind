#include "dukbind_validation.h"
#include <regex>

namespace dukbind
{
    namespace validation
    {
        static std::regex identifer_regex( "[_a-zA-Z$][_a-zA-Z$0-9]*" );

        bool IsValidIdentifier( const char * identifier )
        {
            return std::regex_match( identifier, identifer_regex );
        }
    }
}