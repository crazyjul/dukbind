#pragma once


#if !defined DUKBIND_DEBUG
	#if defined DEBUG || defined _DEBUG
		#define DUKBIND_DEBUG 1
	#else
		#define DUKBIND_DEBUG 0
	#endif
#endif

// DUKBIND_NO_FLOAT_CONVERSION configures float conversion mode in bindings
// When set to 1, if a double does not into a float when calling C++ methods, the call will assert
// When set to 0, conversion from any double to float will occurs in the binding

#ifndef DUKBIND_NO_FLOAT_CONVERSION
    #define DUKBIND_NO_FLOAT_CONVERSION 1
#endif

#define DUKBIND_RTTI_TYPE_NATIVE 0
#define DUKBIND_RTTI_TYPE_DUKBIND 1
#define DUKBIND_RTTI_TYPE_CUSTOM 2

#ifndef DUKBIND_RTTI_TYPE
	#define DUKBIND_RTTI_TYPE DUKBIND_RTTI_TYPE_NATIVE
#endif