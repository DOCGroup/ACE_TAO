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

//#define TAO_USE_DOTTED_DECIMAL_ADDRESSES
//
// If set the ORB will use dotted decimal addresses in the IORs it
// exports, this is useful for platforms or environments that cannot
// depend on a DNS beign available.

// The default arguments of the resource factory for the fake service
// configurator
#if !defined (TAO_DEFAULT_RESOURCE_FACTORY_ARGS)
#define TAO_DEFAULT_RESOURCE_FACTORY_ARGS "static Resource_Factory \"-ORBresources tss -ORBreactorlock null\""
#endif /* TAO_DEFAULT_RESOURCE_FACTORY_ARGS */

// The default arguments of the client strategy factory for the fake service
// configurator
#if !defined (TAO_DEFAULT_CLIENT_STRATEGY_FACTORY_ARGS)
#define TAO_DEFAULT_CLIENT_STRATEGY_FACTORY_ARGS "static Client_Strategy_Factory \"\""
#endif /* TAO_DEFAULT_CLIENT_STRATEGY_FACTORY_ARGS */

// The default arguments of the server strategy factory for the fake service
// configurator
#if !defined (TAO_DEFAULT_SERVER_STRATEGY_FACTORY_ARGS)
#define TAO_DEFAULT_SERVER_STRATEGY_FACTORY_ARGS  "static Server_Strategy_Factory \"-ORBconcurrency reactive -ORBsystemidpolicydemuxstrategy dynamic -ORBtablesize 128\""
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

// The default size of TAO's server poa map.
#if !defined (TAO_DEFAULT_SERVER_POA_MAP_SIZE)
#  define TAO_DEFAULT_SERVER_POA_MAP_SIZE 24
#endif /* ! TAO_DEFAULT_SERVER_POA_MAP_SIZE */

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
// Naming, Trading and other servicesService.
#if !defined (TAO_DEFAULT_SERVICE_RESOLUTION_TIMEOUT)
#define TAO_DEFAULT_SERVICE_RESOLUTION_TIMEOUT 4
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

// Instead of replacing this with the ACE macro
// in 20+ files, define it conditionally.
// The TAO_OutputCDR class uses the ACE macro, which
// is defined by default.
//#if defined (ACE_NO_COPY_OCTET_SEQUENCES)
#define TAO_NO_COPY_OCTET_SEQUENCES
//#endif /* ACE_NO_COPY_OCTET_SEQUENCES */

#if defined (ACE_HAS_EXCEPTIONS)
# define TAO_HAS_EXCEPTIONS
// TAO_USE_EXCEPTIONS is only here for backwards compatibility and
// will go away shortly so don't use it...
# define TAO_USE_EXCEPTIONS
#elif (defined (TAO_HAS_EXCEPTIONS) || defined (TAO_USE_EXCEPTIONS)) && !defined (ACE_HAS_EXCEPTIONS)
# error "tao/orbconf.h: You can only use exceptions in TAO if ACE supports them"
#endif /* TAO_HAS_EXCEPTIONS */

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

#if !defined (TAO_CONST)
// Something to document the fact that we want to put 'const' in front
// of a type, but that it won't turn out the way we want, e.g., we
// really want to express that a CORBA_String is const, but since
// CORBA_String is a char*, the const modifies the pointer and not the
// pointed-to, and some compilers (like SGI's EDG-derived thang)
// complain.
#define TAO_CONST
#endif /* TAO_CONST */

// The IDL compiler can generate the classes corresponding to IDL
// sequences in two ways:
// + Use the TAO templates for sequences,
//   i.e. TAO_{Unb,B}ounded_Sequence<> 
// + Explicitly generate code for the sequence.
//
// The first approach can (potentially) produce smaller code, because
// the code for a sequence over a particular type (say sequence<long>) 
// can be shared across multiple IDL files.
// Unfortunately it is hard to manage the template instantiations on
// platforms that do not automatically generate them, mainly because
// it is hard to decide on which generated file are the templates
// instantiated.  Thus the second approach is more convenient for most 
// applications.
//
// On platforms that support automatic template instantiation we use
// the first approach.
// On platforms that require explicit template instantiations we use
// explicitly generated code for sequences if the platform does not.
// If the application requires it (such as embedded systems) the
// default can be changed, but then the application developer is
// responsible for instantiating the templates.
//
#if !defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION) && \
    defined (ACE_HAS_TEMPLATE_SPECIALIZATION)
#define TAO_USE_SEQUENCE_TEMPLATES
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

// ObjectIds recognized by CORBA_ORB::resolve_initial_references ()...
// of course, no guarantees are made that the call will return
// something useful.
#define TAO_OBJID_NAMESERVICE      "NameService"
#define TAO_OBJID_TRADINGSERVICE   "TradingService"
#define TAO_OBJID_ROOTPOA          "RootPOA"
#define TAO_OBJID_POACURRENT       "POACurrent"
#define TAO_OBJID_INTERFACEREP     "InterfaceRepository"
#define TAO_OBJID_POLICYMANAGER    "ORBPolicyManager"
#define TAO_OBJID_POLICYCURRENT    "PolicyCurrent"

// The Root POA default name.
#define TAO_DEFAULT_ROOTPOA_NAME   ""

// Minimum CORBA
// #define TAO_HAS_MINIMUM_CORBA

// CORBA Messaging
#if defined(ACE_WIN32)
// In other platforms this is defined in the command-line
#define TAO_HAS_CORBA_MESSAGING
#endif /* ACE_WIN32 */

// The maximum value for an standard PolicyType, we use this trick to
// pack the standard policies and the TAO extension in a single
// array.
// The motivation for such a low-level optimization is that policies
// can be set on a per-object level, allocating a complex data
// structure (such as a hash map) or a big array for each object is
// not feasible.
#define TAO_MAX_STANDARD_POLICIES 64

// The number of TAO specific policies
#define TAO_POLICIES_COUNT 32

// The size of the Policy array, the number of policies supported in
// TAO is limited by this number.
#define TAO_MAX_POLICIES (TAO_MAX_STANDARD_POLICIES+TAO_POLICIES_COUNT)

// TAO may define its own policies, they are defined in a range far
// from the standard policies.
#define TAO_MIN_PROPIETARY_POLICY 1024
#define TAO_MAX_PROPIETARY_POLICY (TAO_MIN_PROPIETARY_POLICY+TAO_POLICIES_COUNT)

#endif  /* TAO_ORB_CONFIG_H */
