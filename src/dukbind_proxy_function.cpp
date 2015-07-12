#include <duktape.h>
#include "dukbind_proxy_function.h"

namespace dukbind
{
    namespace internal
    {
        duk_ret_t GlobalsGet( duk_context * ctx )
        {
            duk_dup( ctx, 2 );

            if( duk_has_prop( ctx, 1 ) )
            {
                duk_dup( ctx, 2 );
                duk_get_prop( ctx, 1 );
                return 1;
            }

            return BindingGet( ctx );
        }

        duk_ret_t GlobalsHas( duk_context * ctx )
        {
            duk_dup( ctx, 2 );

            if( !duk_has_prop( ctx, 1 ) )
            {
                return BindingHas( ctx );
            }

            duk_push_boolean( ctx, true );

            return 1;
        }

        duk_ret_t BindingGet( duk_context * ctx )
        {
            return 0;
        }

        duk_ret_t BindingHas( duk_context * ctx )
        {
            return 0;
        }

        duk_ret_t BindingSet( duk_context * ctx )
        {
            duk_push_boolean( ctx, false );
            return 1;
        }

        duk_ret_t BindingDelete( duk_context * ctx )
        {
            duk_push_boolean( ctx, false );
            return 1;
        }
    }
}