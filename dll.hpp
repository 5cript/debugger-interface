#pragma once

#ifdef GDB_INTERFACE_BUILD_DYNAMIC
    #define DBG_DLL_EXPORT __declspec(dllexport)
#elif defined(GDB_INTERFACE_LINK_DYNAMIC)
    #define DBG_DLL_EXPORT __declspec(dllimport)
#else
    #define DBG_DLL_EXPORT
#endif
