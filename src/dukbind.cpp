#include "duktape.h"
#include "dukbind.h"
#include "dukbindconf.h"
#include "dukbind_proxy_function.h"
#include "dukbind_private.h"
#include "utils/dukbind_debug.h"

namespace dukbind
{

    struct Box
    {
        void * ObjectPointer;
        size_t ClassIndex;
        finalizer_t Finalizer;
    };

    void Push( duk_context * ctx, const bool value, const bool* )
    {
        duk_push_boolean( ctx, value );
    }

    void Push( duk_context * ctx, const char * value, const char ** )
    {
        duk_push_string( ctx, value );
    }

    void Push( duk_context * ctx, const int value, const int * )
    {
        dukbind_assert(
            ( (int)(duk_double_t) value ) == value,
            "Value does not fit into a duk_double_t"
            );

        duk_push_number( ctx, (duk_double_t)value );
    }

    void Push( duk_context * ctx, const float value, const float * )
    {
        dukbind_assert(
            ( (float)(duk_double_t) value ) == value,
            "Value does not fit into a duk_double_t"
            );

         duk_push_number( ctx, (duk_double_t)value );
    }

    void Push( duk_context * ctx, const double value, const double * )
    {
        dukbind_assert(
            ( (double)(duk_double_t) value ) == value,
            "Value does not fit into a duk_double_t"
            );
         duk_push_number( ctx, (duk_double_t)value );
    }

    bool Get( duk_context * ctx, const int index, const bool * )
    {
        dukbind_assert( duk_is_boolean( ctx, index ), "No conversion is allowed in Get methods" );
        return duk_to_boolean( ctx, index );
    }

    const char * Get( duk_context * ctx, const int index, const char ** )
    {
        dukbind_assert(
            duk_is_null_or_undefined( ctx, index ) ||  duk_is_string( ctx, index ),
            "No conversion is allowed in Get methods" );

        if( duk_is_null_or_undefined( ctx, index ) )
        {
            return 0;
        }
        else
        {
            return duk_to_string( ctx, index );
        }
    }

    int Get( duk_context * ctx, const int index, const int * )
    {
        dukbind_assert( duk_is_number( ctx, index ), "No conversion is allowed in Get methods" );

        duk_double_t double_result = duk_to_number( ctx, index );
        int int_result = static_cast<int>( double_result );

        dukbind_assert( double_result == (duk_double_t)int_result, "Number is not a valid integer" );

        return int_result;
    }

    float Get( duk_context * ctx, const int index, const float * )
    {
        dukbind_assert( duk_is_number( ctx, index ), "No conversion is allowed in Get methods" );

        duk_double_t double_result = duk_to_number( ctx, index );
        float float_result = static_cast<float>( double_result );

        #if DUKBIND_NO_FLOAT_CONVERSION
            dukbind_assert( double_result == (duk_double_t)float_result, "Number is not a valid integer" );
        #endif

        return float_result;
    }

    double Get( duk_context * ctx, const int index, const double * )
    {
        dukbind_assert( duk_is_number( ctx, index ), "No conversion is allowed in Get methods" );

        return duk_to_number( ctx, index );
    }

    static Box * PrivatePush( duk_context * ctx, const size_t class_index, const size_t object_size, finalizer_t finalizer )
    {
        duk_push_global_object( ctx );
        duk_get_prop_string( ctx, -1, "Proxy" );
        duk_remove( ctx, -2 );

        duk_push_object( ctx );

        size_t require_size = sizeof( Box ) + object_size;

        Box * box = reinterpret_cast<Box*>( duk_push_fixed_buffer( ctx, require_size ) );
        box->ClassIndex = class_index;
        box->Finalizer = finalizer;

        duk_put_prop_string( ctx, -2, "\xFF" "Box" );

        duk_push_c_function( ctx, &internal::ClassFinalizer, 1 );
        duk_set_finalizer( ctx, -2 );

        duk_push_heap_stash( ctx );
        duk_get_prop_string( ctx, -1, "InstanceHandler" );
        duk_remove( ctx, -2 );
        duk_new( ctx, 2 );

        return box;
    }

    void * Push( duk_context * ctx, const size_t class_index, const size_t object_size, finalizer_t finalizer )
    {
        Box * box = PrivatePush( ctx, class_index, object_size, finalizer );
        box->ObjectPointer = box + 1;

        return box->ObjectPointer;
    }

    void * Push( duk_context * ctx, const size_t class_index, void * object_pointer, finalizer_t finalizer )
    {
        Box * box = PrivatePush( ctx, class_index, 0, finalizer );
        box->ObjectPointer = object_pointer;

        return box->ObjectPointer;
    }

    void * Get( duk_context * ctx, duk_idx_t index, size_t & class_index, finalizer_t & finalizer )
    {
        duk_size_t size;
        duk_get_prop_string( ctx, index, "\xFF" "Box" );
        dukbind_assert( duk_is_fixed_buffer( ctx, -1 ), "Boxing is always implemented as fixed buffer" );
        Box * box = reinterpret_cast<Box*>( duk_to_buffer( ctx, -1, &size ) );
        duk_pop( ctx );

        dukbind_assert( sizeof( Box ) <= size, "Fixed buffer is not a box" );

        class_index = box->ClassIndex;
        finalizer = box->Finalizer;
        return box->ObjectPointer;
    }

    void Setup( duk_context * ctx, const BindingInfo & info, const char * module )
    {
        debug::StackMonitor monitor( ctx );
        duk_push_global_stash( ctx );
        void * info_buffer = duk_push_fixed_buffer( ctx, sizeof( BindingInfo * ) );
        *reinterpret_cast<const BindingInfo**>( info_buffer ) = &info; // :TODO: Ref count

        duk_put_prop_string( ctx, -2, DUKBIND_BINDING_NAME );
        duk_pop( ctx );

        if( module )
        {
            duk_push_global_object( ctx );
            duk_get_prop_string( ctx, -1, "Proxy" );
            duk_push_object( ctx );
            duk_push_object( ctx );

            duk_push_c_function( ctx, internal::BindingGet, 2 );
            duk_put_prop_string( ctx, -2, "get" );

            duk_push_c_function( ctx, internal::BindingHas, 1 );
            duk_put_prop_string( ctx, -2, "has" );

            duk_push_c_function( ctx, internal::ForbidSet, 3 );
            duk_put_prop_string( ctx, -2, "set" );

            duk_push_c_function( ctx, internal::ForbidDelete, 1 );
            duk_put_prop_string( ctx, -2, "deleteProperty" );
            duk_new( ctx, 2 );

            duk_put_prop_string( ctx, -2, module );
            duk_pop( ctx );
        }
        else
        {

            duk_push_global_object( ctx );

            duk_get_prop_string( ctx, -1, "Proxy" );

            duk_dup( ctx, -2 );
            duk_push_object( ctx );

            duk_push_c_function( ctx, internal::GlobalsGet, 2 );
            duk_put_prop_string( ctx, -2, "get" );

            duk_push_c_function( ctx, internal::GlobalsHas, 1 );
            duk_put_prop_string( ctx, -2, "has" );

            duk_new( ctx, 2 );
            duk_set_global_object( ctx );

            duk_pop( ctx );
        }

        duk_push_heap_stash( ctx );
        duk_push_object( ctx );

        duk_push_c_function( ctx, internal::ClassGet, 2 );
        duk_put_prop_string( ctx, -2, "get" );

        duk_push_c_function( ctx, internal::ClassHas, 1 );
        duk_put_prop_string( ctx, -2, "has" );

        duk_push_c_function( ctx, internal::ClassSet, 3 );
        duk_put_prop_string( ctx, -2, "set" );

        duk_push_c_function( ctx, internal::ForbidDelete, 1 );
        duk_put_prop_string( ctx, -2, "deleteProperty" );

        duk_put_prop_string( ctx, -2, "InstanceHandler" );

        duk_pop( ctx );
    }

}
