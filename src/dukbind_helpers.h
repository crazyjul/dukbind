#pragma once

#include "utils/dukbind_rtti.h"

namespace dukbind
{
    class BindingInfo;

    class DefineClassBase
    {
    public:

        void Method( const char * name, const duk_c_function function );

	protected:

        DefineClassBase( BindingInfo & info, const char * class_name, const size_t class_identifier );

        BindingInfo
            & Info;
		size_t
			ClassIdentifier;
	};

	template<typename _Class_>
	class DefineClass : public DefineClassBase
	{
	public:
		DefineClass( BindingInfo & info, const char * class_name )
            : DefineClassBase( info, class_name, rtti::GetTypeIndex<_Class_>() )
        {
        }

    };


}