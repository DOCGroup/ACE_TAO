// This may look like C, but it's really -*- C++ -*-
// @ (#) $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     default_server.h
//
// = DESCRIPTION
//     Build configuration file for the Inter-ORB Engine, and application
//     level code using it via <corba.hh>
//
// = AUTHOR
//     Copyright 1995 by Sun Microsystems, Inc.
//
//     TAO-specific changes by Chris Cleeland and Douglas C. Schmidt
//
// ============================================================================

#if !defined (TAO_ORB_CONFIG_H)
#define TAO_ORB_CONFIG_H

#include "ace/OS.h"

// The default arguments of the resource factory for the fake service 
// configurator
#if !defined (TAO_DEFAULT_RESOURCE_FACTORY_ARGS)
#define TAO_DEFAULT_RESOURCE_FACTORY_ARGS { "-ORBresources", "tss" }
#endif /* TAO_DEFAULT_RESOURCE_FACTORY_ARGS */

// The default arguments of the client strategy factory for the fake service 
// configurator
#if !defined (TAO_DEFAULT_CLIENT_STRATEGY_FACTORY_ARGS)
#define TAO_DEFAULT_CLIENT_STRATEGY_FACTORY_ARGS { 0 }
#endif /* TAO_DEFAULT_CLIENT_STRATEGY_FACTORY_ARGS */

// The default arguments of the server strategy factory for the fake service 
// configurator
#if !defined (TAO_DEFAULT_SERVER_STRATEGY_FACTORY_ARGS)
#define TAO_DEFAULT_SERVER_STRATEGY_FACTORY_ARGS { "-ORBconcurrency", "reactive", \
                                                   "-ORBdemuxstrategy", "dynamic", \
                                                   "-ORBtablesize", "128" }
#endif /* TAO_DEFAULT_SERVER_STRATEGY_FACTORY_ARGS */

// The default size of TAO's server object table.
#if !defined (TAO_DEFAULT_SERVER_OBJECT_TABLE_SIZE)
#define TAO_DEFAULT_SERVER_OBJECT_TABLE_SIZE 64
#endif /* TAO_DEFAULT_SERVER_OBJECT_TABLE_SIZE */

// The default UDP multicast port number for locating the TAO Naming
// Service.
#if !defined (TAO_DEFAULT_NAME_SERVER_REQUEST_PORT)
#define TAO_DEFAULT_NAME_SERVER_REQUEST_PORT 10013
#endif /* TAO_DEFAULT_NAME_SERVER_REQUEST_PORT */

// The default UDP port number for replying to a location request to
// the TAO Naming Service.
#if !defined (TAO_DEFAULT_NAME_SERVER_REPLY_PORT)
#define TAO_DEFAULT_NAME_SERVER_REPLY_PORT 10014
#endif /* TAO_DEFAULT_NAME_SERVER_REPLY_PORT */

// The default UDP multicast port number for locating the TAO Trading
// Service.
#if !defined (TAO_DEFAULT_TRADING_SERVER_REQUEST_PORT)
#define TAO_DEFAULT_TRADING_SERVER_REQUEST_PORT 10016
#endif /* TAO_DEFAULT_NAME_SERVER_REQUEST_PORT */

// The default UDP port number for replying to a location request to
// the TAO Trading Service.
#if !defined (TAO_DEFAULT_TRADING_SERVER_REPLY_PORT)
#define TAO_DEFAULT_TRADING_SERVER_REPLY_PORT 10017
#endif /* TAO_DEFAULT_NAME_SERVER_REPLY_PORT */

// The default timeout receiving the location request to
// the TAO Naming Service.
#if !defined (TAO_DEFAULT_NAME_SERVER_TIMEOUT)
#define TAO_DEFAULT_NAME_SERVER_TIMEOUT 5
#endif /* TAO_DEFAULT_NAME_SERVER_TIMEOUT */

// The default starting port number for TAO servers.
#if !defined (TAO_DEFAULT_SERVER_PORT)
#define TAO_DEFAULT_SERVER_PORT 10015
#endif /* TAO_DEFAULT_SERVER_PORT */

// TAO's max buffer size
#if !defined (TAO_MAXBUFSIZE)
#define TAO_MAXBUFSIZE 1024
#endif /* TAO_MAXBUFSIZE */

// This controls the alignment for TAO structs.  It supports built-in
// types up to and including 16 bytes (128 bits) in size.
#if !defined (TAO_MAXIMUM_NATIVE_TYPE_SIZE)
# define TAO_MAXIMUM_NATIVE_TYPE_SIZE 128
#endif /* TAO_MAXIMUM_NATIVE_TYPE_SIZE */

// BC++ seems to have a different convention for detecting Win32 than
// VC++.

#if defined (__WIN32__)
# define _WIN32
#endif /* BC++ convention */

// Define to `int' if <sys/types.h> doesn't define.
/* #undef pid_t */

// Define if your processor does not store words with the most significant
// byte first.
#if defined (ACE_LITTLE_ENDIAN)
#  define TAO_ENCAP_BYTE_ORDER 1  /* little endian encapsulation byte order has
                                     the value = 1 */
#else  /* ! ACE_LITTLE_ENDIAN */
#  define TAO_ENCAP_BYTE_ORDER 0  /* big endian encapsulation byte order has
                                     the value = 0 */
#endif /* ! ACE_LITTLE_ENDIAN */

// Define as the return type of signal handlers (int or void).
#define RETSIGTYPE void

// Define if you don't have vprintf but do have _doprnt.
/* #undef HAVE_DOPRNT */

// Define if you have the vprintf function.
#define HAVE_VPRINTF 1

// Avoid namespace pollution that's common on older UNIXes...
#if defined (minor)
#undef minor
#endif /* minor */

#if defined (major)
#undef major
#endif /* major*/

// For Win16, near/far pointers reflect same/other segment addressing.
// Of course, ACE doesn't support Win16, so why bother?

// For now, we simply set the default to empty definition and let
// places that it fails bubble up.
#define _FAR

// Assume DOS/Windows if "configure" didn't get run.

#if defined (_WIN32)
// "C4355: 'this' : used in base member initializer list"
#  pragma warning (disable:4355) /* disable C4355 warning */
#endif /* defined (_WIN32) */

#endif  /* TAO_ORB_CONFIG_H */
