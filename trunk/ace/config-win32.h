/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for Windows 95,
// Windows NT 3.51 and Windows NT 4.0 platforms using the Microsoft Visual C++ 
// compilers 2.0, 4.0, 4.1, and 4.2. 
// It should work with MSVC 5.0, but this has not been tested.

#if !defined (ACE_CONFIG_H)
#define ACE_CONFIG_H

// ----------------------- user configuration defines --------------------

// Define this if you're running NT 4.x
//  Setting applies to  : building ACE
//  Runtime restrictions: System must be Windows NT > 4.0
//  Additonal notes: Defining _WIN32_WINNT as 0x0400 implies ACE_HAS_WINSOCK2.
// #define _WIN32_WINNT 0x0400

// Uncomment this, if you want ACE to use CWinThread. This should
// be defined, if your application uses MFC. 
//  Setting applies to  : building ACE
//  Runtime restrictions: MFC DLLs must be installed
//  Additonal notes		: If both ACE_HAS_MFC and ACE_MT_SAFE are 
//                        defined, the MFC DLL (not the static lib)
//                        will be used from ACE.
// #define ACE_HAS_MFC

// Uncomment this out if you want to use STRICT type checking.
// It is disabled by default because it will break existing application code.
//  Setting applies to  : building ACE, linking with ACE 
//  Runtime restrictions: -
//  Additonal notes		: ACE_HAS_MFC implies ACE_HAS_STRICT
// #define ACE_HAS_STRICT

// Turn off the following define if you want to disable threading.
// Compile using multi-thread libraries.
//  Setting applies to  : building ACE, linking with ACE
//  Runtime restrictions: multithreaded runtime DLL must be installed
#define ACE_MT_SAFE

// Build as as a DLL.  Zap this line if you want to build a static
// lib.
//  Setting applies to  : building ACE, linking with ACE
//  Runtime restrictions: ACE DLL must be installed :-)
#define ACE_HAS_DLL

// Define the following macro if you're compiling with WinSock 2.0.
//  Setting applies to  : building ACE
//  Runtime restrictions: winsock2 must be installed.
// #define ACE_HAS_WINSOCK2

// Uncomment these if you want to integrate ACE and Orbix in Win32.
//  Setting applies to  : building ACE, linking with ACE
//  Runtime restrictions: system must have Orbix DLLs
// #define ACE_HAS_ORBIX
// #define ACE_HAS_MT_ORBIX

// ----------------------- platform specific defines --------------------

#include <ace/config-win32-common.h>

#endif /* ACE_CONFIG_H */
