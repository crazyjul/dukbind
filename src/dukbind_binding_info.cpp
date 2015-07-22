#include <duktape.h>
#include "dukbind_binding_info.h"
#include "dukbind.h"
#include "dukbind_assert.h"
#include "utils/dukbind_validation.h"

#include <map>
#include <string>

namespace dukbind
{
    namespace internal
    {
        struct BindingInfoData
        {
            std::map<std::string,duk_c_function> //:TODO: Replace by custom collection and custom string(internalized)
                FunctionTable;
        };
    }


    BindingInfo::BindingInfo() : Data( new internal::BindingInfoData )
    {

    }

    BindingInfo::~BindingInfo()
    {
        delete Data;
    }

    void BindingInfo::AddFunction(
        const char * name,
        const duk_c_function function
        )
    {
        dukbind_assert( name && name[ 0 ], "Function name cannot be empty" );
        dukbind_assert( dukbind::validation::IsValidIdentifier( name ), "Function name should be a valid identifier" );
        dukbind_assert( function, "Function cannot be null" );
        auto result = Data->FunctionTable.insert( std::make_pair( std::string( name ), function ) );

        dukbind_assert( result.second == true, "Function already exists" );
    }

    duk_c_function BindingInfo::GetFunction(
        const char * name
        ) const
    {
        auto it = Data->FunctionTable.find( name );

        if( it != Data->FunctionTable.end() )
        {
            return it->second;
        }

        return 0;
    }
}