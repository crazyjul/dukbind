#include <string>
#include "duktape.h"
#include "dukbind.h"
#include "dukbind_std.h"


namespace dukbind
{
    std::string Get( duk_context * ctx, const int index, const std::string * )
    {
        return Get( ctx, index, (const char **)0 );
    }

    void Push( duk_context * ctx, const std::string & value, const std::string * )
    {
        Push( ctx, value.c_str(), (const char **)0 );
    }
}
