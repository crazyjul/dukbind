#pragma once

#ifndef DUKTAPE_H_INCLUDED
    #error Duktape.h should be included before this header
#endif

namespace dukbind
{
    namespace internal
    {

        duk_ret_t GlobalsGet( duk_context * ctx );
        duk_ret_t GlobalsHas( duk_context * ctx );

        duk_ret_t BindingGet( duk_context * ctx );
        duk_ret_t BindingHas( duk_context * ctx );

        duk_ret_t ClassGet( duk_context * ctx );
        duk_ret_t ClassHas( duk_context * ctx );
        duk_ret_t ClassSet( duk_context * ctx );

        duk_ret_t ClassFinalizer( duk_context * ctx );

        duk_ret_t ForbidSet( duk_context * ctx );
        duk_ret_t ForbidDelete( duk_context * ctx );
    }
}