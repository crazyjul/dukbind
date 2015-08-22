#include "duktape.h"
#include "dukbind_helpers.h"
#include "dukbind_binding_info.h"

namespace dukbind
{

    void DefineClassBase::Method( const char * name, const duk_c_function function )
    {
        Info.AddMethod( ClassIdentifier, name, function );
    }

    DefineClassBase::DefineClassBase( BindingInfo & info, const char * class_name, const size_t class_identifier ) :
        Info( info ), ClassIdentifier( class_identifier )
    {
        Info.AddClass( class_name, ClassIdentifier );
    }
}