#pragma once

#ifdef GDB_INTERFACE_BUILD_DYNAMIC
    #define GDBI_DLL_EXPORT __declspec(dllexport)
#elif defined(GDB_INTERFACE_LINK_DYNAMIC)
    #define GDBI_DLL_EXPORT __declspec(dllimport)
#else
    #define GDBI_DLL_EXPORT
#endif
