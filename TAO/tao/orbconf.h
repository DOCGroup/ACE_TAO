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

#ifndef TAO_ORB_CONFIG_H
#define TAO_ORB_CONFIG_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (TAO_REACTOR)
#define TAO_REACTOR ACE_Select_Reactor
#endif /* TAO_REACTOR */

#if !defined (TAO_NULL_LOCK_REACTOR)
#define TAO_NULL_LOCK_REACTOR ACE_Select_Reactor_T< ACE_Select_Reactor_Token_T<ACE_Noop_Token> >
#endif /* TAO_NULL_LOCK_REACTOR */

// Users should override this stuff if they don't want to use the
// default ACE Socket wrappers.  This makes it possible to change
// TAO's transport mechanism wholesale by making a few minor changes
// to the ACE config.h file.
#if !defined (TAO_SOCK_STREAM)
#define TAO_SOCK_STREAM ACE_SOCK_STREAM
#define TAO_SOCK_Stream ACE_SOCK_Stream
#endif /* TAO_SOCK_STREAM */

#if !defined (TAO_SOCK_ACCEPTOR)
#define TAO_SOCK_ACCEPTOR ACE_SOCK_ACCEPTOR
#endif /* TAO_SOCK_ACCEPTOR */

#if !defined (TAO_SOCK_CONNECTOR)
#define TAO_SOCK_CONNECTOR ACE_SOCK_CONNECTOR
#endif /* TAO_SOCK_CONNECTOR */

//#define POA_NO_TIMESTAMP
//
// Define this if you don't want POA timestamps in the IOR.  Remember,
// without timestamps, transient and persistent POA cannot be
// distinguished

// The default arguments of the resource factory for the fake service
// configurator
#if !defined (TAO_DEFAULT_RESOURCE_FACTORY_ARGS)
#define TAO_DEFAULT_RESOURCE_FACTORY_ARGS "static TAO_Resource_Factory \"-ORBresources tss -ORBreactorlock null\""
#endif /* TAO_DEFAULT_RESOURCE_FACTORY_ARGS */

// The default arguments of the client strategy factory for the fake service
// configurator
#if !defined (TAO_DEFAULT_CLIENT_STRATEGY_FACTORY_ARGS)
#define TAO_DEFAULT_CLIENT_STRATEGY_FACTORY_ARGS "static TAO_Default_Client_Strategy_Factory \"\""
#endif /* TAO_DEFAULT_CLIENT_STRATEGY_FACTORY_ARGS */

// The default arguments of the server strategy factory for the fake service
// configurator
#if !defined (TAO_DEFAULT_SERVER_STRATEGY_FACTORY_ARGS)
#define TAO_DEFAULT_SERVER_STRATEGY_FACTORY_ARGS  "static TAO_Default_Server_Strategy_Factory \"-ORBconcurrency reactive -ORBdemuxstrategy dynamic -ORBtablesize 128\""
#endif /* TAO_DEFAULT_SERVER_STRATEGY_FACTORY_ARGS */

// The default size of TAO's server active object map.
#if !defined (TAO_DEFAULT_SERVER_ACTIVE_OBJECT_MAP_SIZE)
# if defined (TAO_DEFAULT_SERVER_OBJECT_TABLE_SIZE)
//
// TAO_DEFAULT_SERVER_OBJECT_TABLE_SIZE is deprecated. Please use
// TAO_DEFAULT_SERVER_ACTIVE_OBJECT_MAP_SIZE.
//
#  define TAO_DEFAULT_SERVER_ACTIVE_OBJECT_MAP_SIZE TAO_DEFAULT_SERVER_OBJECT_TABLE_SIZE
# else
#  define TAO_DEFAULT_SERVER_ACTIVE_OBJECT_MAP_SIZE 64
# endif /* TAO_DEFAULT_SERVER_OBJECT_TABLE_SIZE */
#endif /* ! TAO_DEFAULT_SERVER_ACTIVE_OBJECT_MAP_SIZE */

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

// The default timeout receiving the location request to the TAO
// Naming Service and Trading Service.
#if !defined (TAO_DEFAULT_SERVICE_RESOLUTION_TIMEOUT)
#define TAO_DEFAULT_SERVICE_RESOLUTION_TIMEOUT 1
#endif /* TAO_DEFAULT_SERVICE_RESOLUTION_TIMEOUT */

// The default starting port number for TAO servers.
#if !defined (TAO_DEFAULT_SERVER_PORT)
#define TAO_DEFAULT_SERVER_PORT 0
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

// This deals with platforms that support namespaces vs platforms that
// don't.  @@ MSVC's namespace implementation is somehow broken.
// The following macros are required to deal with the most bizarre and insane
// behavior of the MSVC++ compiler
#if defined (ACE_HAS_USING_KEYWORD)
#define TAO_NAMESPACE namespace
#define TAO_NAMESPACE_STORAGE_CLASS extern TAO_EXPORT_MACRO
#define TAO_NAMESPACE_BEGIN(NS)  namespace NS {
#define TAO_NAMESPACE_END  };
#define TAO_NAMESPACE_TYPE(TYPE)
#define TAO_NAMESPACE_DEFINE(TYPE,NAME,RHS) TYPE NAME = RHS;
#else
#define TAO_NAMESPACE struct TAO_EXPORT_MACRO
#define TAO_NAMESPACE_STORAGE_CLASS static
#define TAO_NAMESPACE_BEGIN(NS)  NS##::
#define TAO_NAMESPACE_END
#define TAO_NAMESPACE_TYPE(TYPE) TYPE
#define TAO_NAMESPACE_DEFINE(TYPE,NAME,RHS) NAME = RHS;
#endif /* ACE_HAS_USING_KEYWORD */

// In some environments it is useful to swap the bytes on write, for
// instance: a fast server can be feeding a lot of slow clients that
// happen to have the wrong byte order.
// This macro enables the functionality to support that, but we still
// need a way to activate it on a per-connection basis.
//
// #define TAO_ENABLE_SWAP_ON_WRITE

// In some environements we never need to swap bytes when reading, for
// instance embebbed systems (such as avionics) or homogenous
// networks.
// Setting this macro disables the capabilities to demarshall streams
// in the wrong byte order.
//
// #define TAO_DISABLE_SWAP_ON_READ

// For some applications it is important to optimize octet sequences
// and minimize the number of copies made of the sequence buffer.
// TAO supports this optimizations by sharing the CDR stream buffer
// and the octet sequences buffer via ACE_Message_Block's.
// This feature can be disabled for: debugging, performance
// comparisons, complaince checking (the octet sequences add an API to
// access the underlying message block).
//
#define TAO_NO_COPY_OCTET_SEQUENCES

// Even though the strategy above minimizes copies in some cases it is
// more efficient just to copy the octet sequence, for instance, while
// enconding a "small" octet sequence in a buffer that has enough
// space.
// This parameter controls the default value for "small enough", but
// can also be set using the command line option -ORBCDRtradeoff
#define TAO_DEFAULT_CDR_MEMCPY_TRADEOFF 256

#if defined (ACE_HAS_EXCEPTIONS)
# define TAO_HAS_EXCEPTIONS
// TAO_USE_EXCEPTIONS is only here for backwards compatibility and
// will go away shortly so don't use it...
# define TAO_USE_EXCEPTIONS
#elif (defined (TAO_HAS_EXCEPTIONS) || defined (TAO_USE_EXCEPTIONS)) && !defined (ACE_HAS_EXCEPTIONS)
# error "tao/orbconf.h: You can only use exceptions in TAO if ACE supports them"
#endif /* TAO_HAS_EXCEPTIONS */

// The CDR growing strategy is control by several parameters:
// + The default or initial CDR buffer size.
// + From that value the CDR buffer is grown exponentially (size
//   duplicated each time) until it reaches EXP_GROWTH_MAX.
// + From then on the buffer is grown linearly in chunks of
// LINEAR_GROWTH_MAX
#define TAO_DEFAULT_CDR_BUFSIZE 512
#define TAO_DEFAULT_CDR_EXP_GROWTH_MAX 4096
#define TAO_DEFAULT_CDR_LINEAR_GROWTH_CHUNK 4096

// BC++ seems to have a different convention for detecting Win32 than
// VC++.

#if defined (__WIN32__)
# if !defined(_WIN32)
#   define _WIN32
# endif /* !defined(_WIN32) */
#endif /* __WIN32__ */

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

// Assume DOS/Windows if "configure" didn't get run.

#if defined (_WIN32)
// "C4355: 'this' : used in base member initializer list"
#  pragma warning (disable:4355) /* disable C4355 warning */
#endif /* defined (_WIN32) */

#endif  /* TAO_ORB_CONFIG_H */
