#include "dukbind_validation.h"
#include <regex>

namespace dukbind
{
    namespace validation
    {
        static std::regex identifer_regex( "[a-zA-Z$_][a-zA-Z0-9$_]*" );

        bool IsValidIdentifier( const char * identifier )
        {
            return std::regex_match( identifier, identifer_regex );
        }
    }
}
