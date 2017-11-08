#include "dukbind_validation.h"
#include <regex>

namespace dukbind
{
    namespace validation
    {
        const std::regex & getIndentifierRegex()
        {
            static std::regex identifer_regex( "[a-zA-Z$_][a-zA-Z0-9$_]*" );

            return identifer_regex;
        }

        bool IsValidIdentifier( const char * identifier )
        {
            return std::regex_match( identifier, getIndentifierRegex() );
        }
    }
}
