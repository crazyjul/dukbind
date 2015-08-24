#include "catch.hpp"
#include "duktape.h"
#include "dukbind.h"
#include "dukbind_glue.h"

struct TestPointerCustom
{
    TestPointerCustom( int data ) :Data(data) {}

    int GetData() const{ return Data; }

    void SetData( int data ) { Data = data; }

    int Data;
};

static bool OnPushCalled = false;
static bool IsValidCalled = false;
static bool IsValidValue = true;
static bool OnFinalizeCalled = false;

namespace dukbind {
  template<> struct bind_as_pointer_traits<TestPointerCustom>
  {
      static const bool value = true;
      static void OnPush( TestPointerCustom & )
      {
          OnPushCalled = true;
      }

      static bool IsValid( TestPointerCustom & )
      {
          IsValidCalled = true;
          return IsValidValue;
      }

      static void OnFinalize( TestPointerCustom & )
      {
          OnFinalizeCalled = true;
      }
  };
}

static bool DoStuffIsCalled = false;

static duk_ret_t DoStuff( duk_context * )
{
    DoStuffIsCalled = true;
    return 0;
}

static duk_ret_t CheckThis( duk_context * ctx )
{
    duk_push_this( ctx );

    const TestPointerCustom & result = dukbind::Get( ctx, -1, (TestPointerCustom*)0 );

    REQUIRE( result.Data == 5678 );

    return 0;
}

TEST_CASE( "Class can be passed as custom pointer", "[binding][class]" )
{
    duk_context * ctx = duk_create_heap_default();

    dukbind::BindingInfo info;

    info.AddClass( "TestPointerCustom", dukbind::rtti::GetTypeIndex<TestPointerCustom>() );
    info.AddMethod( dukbind::rtti::GetTypeIndex<TestPointerCustom>(), "DoStuff", DoStuff );
    info.AddMethod( dukbind::rtti::GetTypeIndex<TestPointerCustom>(), "CheckThis", CheckThis );

    dukbind::Setup( ctx, info, "Module" );

    SECTION( "Instance is not copied when pushed" )
    {
        TestPointerCustom data( 1234 );
        OnPushCalled = false;
        dukbind::Push( ctx, data );
        REQUIRE( OnPushCalled == true );
    }

    SECTION( "Instance is validated when got" )
    {
        TestPointerCustom data( 5678 );

        dukbind::Push( ctx, data );
        IsValidValue = true;
        IsValidCalled = false;
        const TestPointerCustom & result = dukbind::Get( ctx, -1, (TestPointerCustom*)0 );
        REQUIRE( IsValidCalled );
    }

    SECTION( "Invalid instance is raising assert" )
    {
        TestPointerCustom data( 5678 );

        dukbind::Push( ctx, data );
        IsValidValue = false;
        REQUIRE_THROWS( dukbind::Get( ctx, -1, (TestPointerCustom*)0 ) );
    }

    SECTION( "OnFinalize is called when Instance is garbage collected" )
    {
        TestPointerCustom data( 1234 );
        dukbind::Push( ctx, data );

        duk_pop( ctx );
        duk_gc( ctx, 0 );
        duk_gc( ctx, 0 );
        REQUIRE( OnFinalizeCalled == true );
    }

    SECTION( "Binding is called" )
    {
        TestPointerCustom data( 1234 );
        duk_push_global_object( ctx );
        dukbind::Push( ctx, data );
        duk_put_prop_string( ctx, -2, "data" );

        duk_eval_string_noresult( ctx, "data.DoStuff()" );

        REQUIRE( DoStuffIsCalled );
    }

    SECTION( "This is (in)valid" )
    {
        TestPointerCustom data( 5678 );
        duk_push_global_object( ctx );
        dukbind::Push( ctx, data );
        duk_put_prop_string( ctx, -2, "data" );

        IsValidValue = true;
        duk_eval_string_noresult( ctx, "data.CheckThis()" );
        IsValidValue = false;
        REQUIRE_THROWS( duk_eval_string_noresult( ctx, "data.CheckThis()" ) );
    }

    SECTION( "Call object method" )
    {
        TestPointerCustom data( 5678 );

        info.AddMethod( dukbind::rtti::GetTypeIndex<TestPointerCustom>(), "GetData", dukbind_GetGlue( TestPointerCustom::GetData ) );
        info.AddMethod( dukbind::rtti::GetTypeIndex<TestPointerCustom>(), "SetData", dukbind_GetGlue( TestPointerCustom::SetData ) );

        duk_push_global_object( ctx );
        dukbind::Push( ctx, data );
        duk_put_prop_string( ctx, -2, "data" );

        IsValidValue = true;
        duk_eval_string_noresult( ctx, "value = data.GetData();" );
        duk_get_prop_string( ctx, -1, "value" );
        REQUIRE( duk_to_number( ctx, -1 ) == 5678 );

        duk_eval_string_noresult( ctx, "data.SetData( 9012 )" );

        REQUIRE( data.Data == 9012 );
    }

    duk_destroy_heap( ctx );
}
