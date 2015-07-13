#pragma once

#ifndef DUKTAPE_H_INCLUDED
    #error Duktape.h should be included before this header
#endif

namespace dukbind
{
    namespace internal
    {
        struct BindingInfoData;

        class BindingInfo final
        {
        public:

            BindingInfo();
            BindingInfo( const BindingInfo & ) = delete;
            BindingInfo( BindingInfo && ) = delete;
            ~BindingInfo();

            BindingInfo & operator=( const BindingInfo & ) = delete;
            BindingInfo & operator=( BindingInfo && ) = delete;

            void AddFunction(
                const char * name,
                const duk_c_function function
                );

            duk_c_function GetFunction(
                const char * name
                ) const;

        private:

            BindingInfoData
                * Data;
        };
    }

}