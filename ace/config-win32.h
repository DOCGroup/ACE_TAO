/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for Windows 95,
// Windows NT 3.51 and Windows NT 4.0 platforms using the Microsoft Visual C++
// compilers 2.0, 4.0, 4.1, 4.2, 5.0 and 6.0

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H

#if defined (ACE_HAS_WINCE) || defined (UNDER_CE)
# include "ace/config-WinCE.h"
#endif /* ACE_HAS_WINCE */

// ----------------------- user configuration defines --------------------

// Define this if you want to use the standard C++ library
//#define ACE_HAS_STANDARD_CPP_LIBRARY 1

// MSVC enforces the One Definition Rule
#define ACE_HAS_ONE_DEFINITION_RULE

// Define this if you're running NT 4.x
//  Setting applies to  : building ACE
//  Runtime restrictions: System must be Windows NT => 4.0
//  Additonal notes: Defining _WIN32_WINNT as 0x0400 implies ACE_HAS_WINSOCK2
//  unless you set ACE_HAS_WINSOCK2 to 0 in the config.h file.
#if !defined (ACE_HAS_WINNT4)
  #define ACE_HAS_WINNT4 1      // assuming Win NT 4.0 or greater
#endif

#if (defined (ACE_HAS_WINNT4) && ACE_HAS_WINNT4 != 0)
  #if !defined (_WIN32_WINNT)
    #define _WIN32_WINNT 0x0400
  #endif
#endif

// Define ACE_HAS_MFC to 1, if you want ACE to use CWinThread. This should
// be defined, if your application uses MFC.
//  Setting applies to  : building ACE
//  Runtime restrictions: MFC DLLs must be installed
//  Additonal notes             : If both ACE_HAS_MFC and ACE_MT_SAFE are
//                        defined, the MFC DLL (not the static lib)
//                        will be used from ACE.
#if !defined (ACE_HAS_MFC)
        #define ACE_HAS_MFC 0
#endif

// Define this if you're using MSVC 4.2 or above and your application
// relies on the standard c++ library.
//  Runtime restrictions: You must be using MSVC 4.2 or above and your
//                        application must link with the standard libraries.
#if defined (_MSC_VER) && (_MSC_VER >= 1020)
        #if !defined (ACE_HAS_STANDARD_CPP_LIBRARY)
                #define ACE_HAS_STANDARD_CPP_LIBRARY    0
        #endif
#else
        #if defined (ACE_HAS_STANDARD_CPP_LIBRARY)
                #undef ACE_HAS_STANDARD_CPP_LIBRARY
        #endif
        #define ACE_HAS_STANDARD_CPP_LIBRARY 0
#endif

// The STL that comes with ACE uses the std namespace. Note however, it is not
// part of the standard C++ library
#if !defined (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB)
        #define ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB   1
#endif /* ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB */

#if !defined (ACE_HAS_BROKEN_NESTED_TEMPLATES)
        #define ACE_HAS_BROKEN_NESTED_TEMPLATES
#endif /* ACE_HAS_BROKEN_NESTED_TEMPLATES */

// Define ACE_HAS_STRICT to 1 in your config.h file if you want to use
// STRICT type checking.  It is disabled by default because it will
// break existing application code.
//  Setting applies to  : building ACE, linking with ACE
//  Runtime restrictions: -
//  Additonal notes             : ACE_HAS_MFC implies ACE_HAS_STRICT
#if !defined (ACE_HAS_STRICT)
        #define ACE_HAS_STRICT 0
#endif

// Turn off the following define if you want to disable threading.
// Compile using multi-thread libraries.
//  Setting applies to  : building ACE, linking with ACE
//  Runtime restrictions: multithreaded runtime DLL must be installed
#if !defined (ACE_MT_SAFE)
        #define ACE_MT_SAFE 1
#endif

// Build as as a DLL.  Define ACE_HAS_DLL to 0 if you want to build a static
// lib.
//  Setting applies to  : building ACE, linking with ACE
//  Runtime restrictions: ACE DLL must be installed :-)
#if !defined (ACE_HAS_DLL)
        #define ACE_HAS_DLL 1
#endif

// Build ACE services as DLLs.  If you write a library and want it to
// use ACE_Svc_Export, this will cause those macros to build dlls.  If
// you want your ACE service to be a static library, comment out this
// line.  As far as I know, the only reason to have a library be an
// ACE "service" is to leverage the ACE_Svc_Export macros.  It's just
// as easy to define your own export macros.
#if !defined (ACE_HAS_SVC_DLL)
        #define ACE_HAS_SVC_DLL 1
#endif

// Define ACE_HAS_WINSOCK2 to 0 in your config.h file if you do *not*
// want to compile with WinSock 2.0.
//  Setting applies to  : building ACE
//  Runtime restrictions: winsock2 must be installed.
//      #define ACE_HAS_WINSOCK2 0

// Define ACE_HAS_ORBIX to 1 in your config.h file if you want to integrate
// ACE and Orbix in Win32.
//  Setting applies to  : building ACE, linking with ACE
//  Runtime restrictions: system must have Orbix DLLs
#if !defined (ACE_HAS_ORBIX)
        #define ACE_HAS_ORBIX 0
#endif

#if !defined (ACE_HAS_MT_ORBIX)
        #define ACE_HAS_MT_ORBIX 0
#endif

// @@ MSVC doesn't seem to handle namespace correctly.  Don't set it
//    by default.

// By default, you will get the proper namespace usage for Orbix.  If
// you don't like this, comment out the #define line or #undef
// ACE_ORBIX_HAS_NAMESPACES in your config.h file after including this
// file.
#if !defined (ACE_ORBIX_HAS_NAMESPACES)
#define ACE_ORBIX_HAS_NAMESPACES
#endif /* ACE_ORBIX_HAS_NAMESPACES */

// By default, we disable the C++ casting because
// it requires the RTTI support to be turned on which
// is not something we usually do.
#if !defined (ACE_HAS_ANSI_CASTS)
        #define ACE_HAS_ANSI_CASTS 0
#endif

// By default, we use non-static object manager on Win32.  That is,
// the object manager is allocated in main's stack memory.  If this
// does not suit your need, you can disable the behavior by defining
// ACE_HAS_NONSTATIC_OBJECT_MANAGER to 0.
//
// MFC users: Since the main function is defined withing MFC library,
// you'll need to instantiate the ACE_Object_Manager by doing either,
//
// 1. Using static object manager (as described above.)
// 2. Instantiate Object Manager in your CApplication derived class
//    and define ACE_DOESNT_INSTANTIATE_NONSTATIC_OBJECT_MANAGER.
//
#if !defined (ACE_HAS_NONSTATIC_OBJECT_MANAGER)
# define ACE_HAS_NONSTATIC_OBJECT_MANAGER
#elif (ACE_HAS_NONSTATIC_OBJECT_MANAGER == 0)
# undef ACE_HAS_NONSTATIC_OBJECT_MANAGER
#endif /* ACE_HAS_NONSTATIC_OBJECT_MANAGER */

// MSVC already defined __TEXT
#define ACE_HAS_TEXT_MACRO_CONFLICT

#define ACE_HAS_GPERF

// ----------------------- platform specific defines --------------------

#include <ace/config-win32-common.h>

#endif /* ACE_CONFIG_H */
