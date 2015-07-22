#include <duktape.h>
#include "dukbind_binding_info.h"
#include "dukbind.h"
#include "dukbind_assert.h"
#include "utils/dukbind_identifier.h"
#include "utils/dukbind_validation.h"

#include <map>
#include <string>

namespace dukbind
{
    namespace internal
    {
        struct BindingInfoData
        {
            std::map<Identifier,duk_c_function> //:TODO: Replace by custom collection
                FunctionTable;
            std::map<size_t, std::string>
                IdToNameTable;
            std::map<Identifier, size_t>
                NameToIdTable;
            std::map<size_t, std::map<Identifier, duk_c_function> >
                ClassMethodTable;
            std::map<size_t, duk_c_function>
                ClassDeleteTable;
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
        auto result = Data->FunctionTable.insert( std::make_pair( Identifier( name ), function ) );

        dukbind_assert( result.second == true, "Function already exists" );
    }

    duk_c_function BindingInfo::GetFunction(
        const char * name
        ) const
    {
        auto it = Data->FunctionTable.find( Identifier( name ) );

        if( it != Data->FunctionTable.end() )
        {
            return it->second;
        }

        return 0;
    }

    void BindingInfo::AddClass(
        const char * name,
        const size_t identifier
        )
    {
        dukbind_assert( name && dukbind::validation::IsValidIdentifier( name ), "Class name is not valid" );
        dukbind_assert( identifier, "Identifier is not valid" );
        auto result = Data->IdToNameTable.insert( std::make_pair( identifier, std::string( name ) ) );
        auto result2 = Data->NameToIdTable.insert( std::make_pair( Identifier( name ), identifier ) );
        Data->ClassMethodTable.insert( std::make_pair( identifier, std::map<Identifier, duk_c_function>() ) );

        dukbind_assert( result.second && result2.second, "Class already registered" );
    }

    void BindingInfo::AddMethod(
        const size_t class_identifier,
        const char * method_name,
        const duk_c_function method
        )
    {
        dukbind_assert( method != 0, "Method must not be null" );
        dukbind_assert( method_name && dukbind::validation::IsValidIdentifier( method_name ), "Method name is not valid" );
        auto function_table = Data->ClassMethodTable.find( class_identifier );
        dukbind_assert( function_table != Data->ClassMethodTable.end(), "Class not found" );

        auto result = ( *function_table ).second.insert( std::make_pair( Identifier( method_name ), method ) );
        dukbind_assert( result.second, "Method already exists" );
    }
}