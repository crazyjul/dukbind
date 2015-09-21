#pragma once


#ifndef DUKTAPE_H_INCLUDED
    #error Duktape.h should be included before this header
#endif

#include <vector>

namespace dukbind
{
    template< typename _Type_ >
    void Push( duk_context * ctx, const std::vector<_Type_> & table, const std::vector<_Type_>* )
    {
        size_t index, end = table.size();
        duk_idx_t arr_idx = duk_push_array( ctx );
        for( index = 0; index < end; ++index )
        {
            Push( ctx, table[ index ], (_Type_*)0 );
            duk_put_prop_index( ctx, arr_idx, index );
        }
    }

    std::string Get( duk_context * ctx, const int index, const std::string * );
    void Push( duk_context * ctx, const std::string & value, const std::string * );
}
