#pragma once


// DUKBIND_NO_FLOAT_CONVERSION configures float conversion mode in bindings
// When set to 1, if a double does not into a float when calling C++ methods, the call will assert
// When set to 0, conversion from any double to float will occurs in the binding

#ifndef DUKBIND_NO_FLOAT_CONVERSION
    #define DUKBIND_NO_FLOAT_CONVERSION 1
#endif