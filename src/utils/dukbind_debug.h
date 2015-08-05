#pragma once

#include "dukbind_assert.h"

namespace dukbind
{
    namespace debug
    {
        class StackMonitor
        {
        public:
            StackMonitor( duk_context * ctx ) :
                TopIndex( duk_get_top_index( ctx ) ),
                Context( ctx )
            {
            }

            ~StackMonitor()
            {
                dukbind_assert( TopIndex == duk_get_top_index( Context ), "Stack is not restored properly" );
            }
        private:

            duk_idx_t
                TopIndex;
            duk_context
                * Context;
        };
    }
}