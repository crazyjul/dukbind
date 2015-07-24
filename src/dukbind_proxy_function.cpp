#include <duktape.h>
#include "dukbind.h"
#include "dukbind_proxy_function.h"
#include "dukbind_binding_info.h"
#include "dukbind_private.h"
#include "dukbind_assert.h"

namespace dukbind
{
    namespace internal
    {
        static BindingInfo * GetBinding( duk_context * ctx )
        {
            duk_push_global_stash( ctx );
            duk_get_prop_string( ctx, -1, DUKBIND_BINDING_NAME );

            duk_size_t buffer_size;
            void * buffer = duk_to_buffer( ctx, -1, &buffer_size);
            dukbind_assert( buffer_size == sizeof( BindingInfo * ), "Invalid buffer size" );

            duk_pop_2( ctx );

            return *reinterpret_cast<BindingInfo**>( buffer );
        }

        duk_ret_t GlobalsGet( duk_context * ctx )
        {
            duk_dup( ctx, 1 );
            duk_get_prop( ctx, 0 );

            if( !duk_is_null_or_undefined( ctx, -1 ) )
            {
                return 1;
            }

            duk_pop( ctx );

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
            BindingInfo * info = GetBinding( ctx );
            const char * name = duk_require_string( ctx, 1 );

            dukbind_assert( info, "No binding info" );

            duk_c_function function = info->GetFunction( name );

            if( function )
            {
                duk_push_c_function( ctx, function, DUK_VARARGS );
                return 1;
            }

            return 0;
        }

        duk_ret_t BindingHas( duk_context * )
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