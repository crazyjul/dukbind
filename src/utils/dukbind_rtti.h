#pragma once

#include <typeindex>

namespace dukbind
{
    namespace rtti
    {
        #if DUKBIND_RTTI_TYPE == DUKBIND_RTTI_TYPE_NATIVE

            template<typename _Type_>
            size_t GetTypeIndex()
            {
                return std::type_index( typeid( _Type_ ) ).hash_code();
            }

            template<typename _Type_>
            size_t GetInstanceIndex( const _Type_ & type )
            {
                ( void ) type; // Tricky, VStudion think type is unused, while typeid is really a function
                return std::type_index( typeid( type ) ).hash_code();
            }
        #else

        #error Implement other mode here

        #endif
    }
}