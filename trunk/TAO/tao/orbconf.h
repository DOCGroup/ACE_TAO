// This may look like C, but it's really -*- C++ -*-
// @ (#) $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     orbconf.h
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
#include "ace/pre.h"

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

// Define this if you don't want POA timestamps in the IOR.  Remember,
// without timestamps, transient and persistent POA cannot be
// distinguished
#if !defined (POA_NO_TIMESTAMP)
# define POA_NO_TIMESTAMP 0
#endif /* POA_NO_TIMESTAMP */

// If set the ORB will use dotted decimal addresses in the IORs it
// exports, this is useful for platforms or environments that cannot
// depend on a DNS beign available.
#if !defined (TAO_USE_DOTTED_DECIMAL_ADDRESSES)
# define TAO_USE_DOTTED_DECIMAL_ADDRESSES 0
#endif /* TAO_USE_DOTTED_DECIMAL_ADDRESSES */

// The default arguments of the resource factory for the fake service
// configurator
#if !defined (TAO_DEFAULT_RESOURCE_FACTORY_ARGS)
#define TAO_DEFAULT_RESOURCE_FACTORY_ARGS "static Resource_Factory \"-ORBresources tss -ORBReactorType select_st\""
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
#endif /* TAO_DEFAULT_TRADING_SERVER_REQUEST_PORT */

// The default UDP port number for replying to a location request to
// the TAO Trading Service.
#if !defined (TAO_DEFAULT_TRADING_SERVER_REPLY_PORT)
#define TAO_DEFAULT_TRADING_SERVER_REPLY_PORT 10017
#endif /* TAO_DEFAULT_TRADING_SERVER_REPLY_PORT */

// The default UDP multicast port number for locating the TAO
// Implementation Repository Service.
#if !defined (TAO_DEFAULT_IMPLREPO_SERVER_REQUEST_PORT)
#define TAO_DEFAULT_IMPLREPO_SERVER_REQUEST_PORT 10018
#endif /* TAO_DEFAULT_IMPLREPO_SERVER_REQUEST_PORT */

// The default UDP port number for replying to a location request to
// the TAO Implementation Repository Service.
#if !defined (TAO_DEFAULT_IMPLREPO_SERVER_REPLY_PORT)
#define TAO_DEFAULT_IMPLREPO_SERVER_REPLY_PORT 10019
#endif /* TAO_DEFAULT_IMPLREPO_SERVER_REPLY_PORT */


// The default timeout receiving the location request to the TAO
// Naming, Trading and other servicesService.
#if !defined (TAO_DEFAULT_SERVICE_RESOLUTION_TIMEOUT)
#define TAO_DEFAULT_SERVICE_RESOLUTION_TIMEOUT 10
#endif /* TAO_DEFAULT_SERVICE_RESOLUTION_TIMEOUT */

// The default starting port number for TAO servers.
// @@ This assumes we are using one or more protocols that will all
// @@ 1) know what to do with a port number and
// @@ 2) use the same default port number.
// @@ This shou8ld go away!
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

// By default connection purging and recycling are done to provide robust
// connection management in TAO.
#if !defined (ACE_MVS)
# define TAO_USES_ROBUST_CONNECTION_MGMT
#endif /* ! ACE_MVS */

// This deals with the strategies for connection caching. By default
// it is the Null Strategy. Although it shall be Least Recently Used
// (LRU) with the default purging percentage of 20% once this feature
// has been thoroughly tested.
#if !defined (TAO_CONNECTION_CACHING_STRATEGY)
# define TAO_CONNECTION_CACHING_STRATEGY TAO_Resource_Factory::LRU
#endif /* TAO_CONNECTION_CACHING_STRATEGY */

#if !defined (TAO_PURGE_PERCENT)
# define TAO_PURGE_PERCENT 20
#endif /* TAO_PURGE_PERCENT */

// MINIMUM_CONNECTION_CACHING_STRATEGY support is disabled by default if TAO is not
// configured for minimum CORBA.  If TAO is configured for minimum
// CORBA, then MINIMUM_CONNECTION_CACHING_STRATEGY will be enabled by default.
// To explicitly enable MINIMUM_CONNECTION_CACHING_STRATEGY support uncomment the following
// #define TAO_HAS_MINIMUM_CONNECTION_CACHING_STRATEGY 1
// To explicitly disable MINIMUM_CONNECTION_CACHING_STRATEGY support uncomment the following
// #define TAO_HAS_MINIMUM_CONNECTION_CACHING_STRATEGY 0

// Default MINIMUM_CONNECTION_CACHING_STRATEGY settings
#if !defined (TAO_HAS_MINIMUM_CONNECTION_CACHING_STRATEGY)
#  if (TAO_HAS_MINIMUM_CORBA == 1)
#    define TAO_HAS_MINIMUM_CONNECTION_CACHING_STRATEGY 1
#  else
#    define TAO_HAS_MINIMUM_CONNECTION_CACHING_STRATEGY 0
#  endif  /* TAO_HAS_MINIMUM_CORBA */
#endif  /* !TAO_HAS_MINIMUM_CONNECTION_CACHING_STRATEGY */

// This deals with platforms that support namespaces vs platforms that
// don't.  @@ MSVC's namespace implementation is somehow broken.
// The following macros are required to deal with the most bizarre and insane
// behavior of the MSVC++ compiler
#if defined (ACE_HAS_USING_KEYWORD)
#define TAO_NAMESPACE namespace
#define TAO_NAMESPACE_CLOSE
#define TAO_NAMESPACE_STORAGE_CLASS extern TAO_EXPORT_MACRO
#define TAO_NAMESPACE_BEGIN(NS)  namespace NS {
#define TAO_NAMESPACE_END  }
#define TAO_NAMESPACE_TYPE(TYPE)
#define TAO_NAMESPACE_DEFINE(TYPE,NAME,RHS) TYPE NAME = RHS;
#else
#define TAO_NAMESPACE struct TAO_EXPORT_MACRO
#define TAO_NAMESPACE_CLOSE ;
#define TAO_NAMESPACE_STORAGE_CLASS static
#define TAO_NAMESPACE_BEGIN(NS)  NS##::
#define TAO_NAMESPACE_END
#define TAO_NAMESPACE_TYPE(TYPE) TYPE
#define TAO_NAMESPACE_DEFINE(TYPE,NAME,RHS) NAME = RHS;
#endif /* ACE_HAS_USING_KEYWORD */

# if defined (_MSC_VER) && defined (__ACE_INLINE__)
#   define TAO_NAMESPACE_INLINE_FUNCTION inline
# else
#   define TAO_NAMESPACE_INLINE_FUNCTION TAO_NAMESPACE_STORAGE_CLASS
# endif

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

#if !defined (TAO_EXPORT_NESTED_CLASSES) \
    && defined (ACE_EXPORT_NESTED_CLASSES)
#define TAO_EXPORT_NESTED_CLASSES
#endif /* !defined TAO_EXPORT_NESTED_CLASSES */

#if !defined (TAO_EXPORT_NESTED_MACRO)
#define TAO_EXPORT_NESTED_MACRO
#endif /* !defined (TAO_EXPORT_NESTED_MACRO) */

// Assume DOS/Windows if "configure" didn't get run.

#if defined (_WIN32)
// "C4355: 'this' : used in base member initializer list"
#  pragma warning (disable:4355) /* disable C4355 warning */
#endif /* defined (_WIN32) */

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
    (defined (ACE_HAS_TEMPLATE_SPECIALIZATION) || \
     (defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA) && defined (_UNICOS)))
#define TAO_USE_SEQUENCE_TEMPLATES
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

// ObjectIds recognized by CORBA_ORB::resolve_initial_references ()...
// of course, no guarantees are made that the call will return
// something useful.
#define TAO_OBJID_NAMESERVICE         "NameService"
#define TAO_OBJID_TRADINGSERVICE      "TradingService"
#define TAO_OBJID_IMPLREPOSERVICE     "ImplRepoService"
#define TAO_OBJID_ROOTPOA             "RootPOA"
#define TAO_OBJID_POACURRENT          "POACurrent"
#define TAO_OBJID_INTERFACEREP        "InterfaceRepository"
#define TAO_OBJID_POLICYMANAGER       "ORBPolicyManager"
#define TAO_OBJID_POLICYCURRENT       "PolicyCurrent"
#define TAO_OBJID_IORMANIPULATION     "IORManipulation"
#define TAO_OBJID_SECURITYCURRENT     "SecurityCurrent"
#define TAO_OBJID_TRANSACTIONCURRENT  "TransactionCurrent"
#define TAO_OBJID_DYNANYFACTORY       "DynAnyFactory"

// Comma separated list of the above ObjectIDs.
// DO NOT include unimplemented services!
// The CORBA_ORB::list_initial_services () method iterates through
// this list to determine which initial services are available.
#define TAO_LIST_OF_INITIAL_SERVICES \
        TAO_OBJID_NAMESERVICE, \
        TAO_OBJID_TRADINGSERVICE, \
        TAO_OBJID_IMPLREPOSERVICE, \
        TAO_OBJID_ROOTPOA, \
        TAO_OBJID_POACURRENT, \
        TAO_OBJID_POLICYMANAGER, \
        TAO_OBJID_POLICYCURRENT, \
        TAO_OBJID_IORMANIPULATION

// Service IDs for the services that are located through Multicast.
enum MCAST_SERVICEID
{
  NAMESERVICE,
  TRADINGSERVICE,
  IMPLREPOSERVICE
};

// No. of services locatable through multicast.
#define NO_OF_MCAST_SERVICES 3

// TAO Naming Service.

// Poa id of the root Naming Context in a Naming server.
#if !defined (TAO_ROOT_NAMING_CONTEXT)
#  define TAO_ROOT_NAMING_CONTEXT "NameService"
#endif /* ! TAO_ROOT_NAMING_CONTEXT */

// The name under which the index of naming contexts is stored in
// persistent naming service.
#if !defined (TAO_NAMING_CONTEXT_INDEX)
#  define TAO_NAMING_CONTEXT_INDEX "Naming_Context_Index"
#endif /* ! TAO_NAMING_CONTEXT_INDEX */

// The Root POA default name.
#define TAO_DEFAULT_ROOTPOA_NAME   ""

// OBV is in experimental stage
#define TAO_HAS_VALUETYPE

// Minimum CORBA
#if !defined (TAO_HAS_MINIMUM_CORBA)
# define TAO_HAS_MINIMUM_CORBA 0
#endif /* TAO_HAS_MINIMUM_CORBA */

// UIOP support is enabled by default if the platform supports UNIX
// domain sockets, and TAO is not configured for minimum CORBA.
// If TAO is configured for minimum CORBA, then UIOP will be disabled
// by default.
// To explicitly enable UIOP support uncomment the following
// #define TAO_HAS_UIOP 1
// To explicitly disable UIOP support uncomment the following
// #define TAO_HAS_UIOP 0

// Default UIOP settings
#if !defined (TAO_HAS_UIOP)
#  if defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)
#    define TAO_HAS_UIOP 0
#  elif (TAO_HAS_MINIMUM_CORBA==1)
#    define TAO_HAS_UIOP 0
#  else
#    define TAO_HAS_UIOP 1
#  endif  /* TAO_HAS_MINIMUM_CORBA */
#endif  /* !TAO_HAS_UIOP */

#if !defined (TAO_HAS_SHMIOP)
# if (TAO_HAS_MINIMUM_CORBA==1)
#   define TAO_HAS_SHMIOP 0
# else
#   define TAO_HAS_SHMIOP 1
# endif /* TAO_HAS_MINIMUM_CORBA */
#endif /* TAO_HAS_SHMIOP */

// RT_CORBA support is enabled by default if TAO is not configured for
// minimum CORBA.  If TAO is configured for minimum CORBA, then
// RT_CORBA will be disabled by default.
// To explicitly enable RT_CORBA support uncomment the following
// #define TAO_HAS_RT_CORBA 1
// To explicitly disable RT_CORBA support uncomment the following
// #define TAO_HAS_RT_CORBA 0

// Default RT_CORBA settings
#if !defined (TAO_HAS_RT_CORBA)
#  if (TAO_HAS_MINIMUM_CORBA == 1)
#    define TAO_HAS_RT_CORBA 0
#  else
#    define TAO_HAS_RT_CORBA 1
#  endif  /* TAO_HAS_MINIMUM_CORBA */
#endif  /* !TAO_HAS_RT_CORBA */

// MINIMUM_POA support is disabled by default if TAO is not
// configured for minimum CORBA.  If TAO is configured for minimum
// CORBA, then MINIMUM_POA will be enabled by default.
// To explicitly enable MINIMUM_POA support uncomment the following
// #define TAO_HAS_MINIMUM_POA 1
// To explicitly disable MINIMUM_POA support uncomment the following
// #define TAO_HAS_MINIMUM_POA 0

// Default MINIMUM_POA settings
#if !defined (TAO_HAS_MINIMUM_POA)
#  if (TAO_HAS_MINIMUM_CORBA == 1)
#    define TAO_HAS_MINIMUM_POA 1
#  else
#    define TAO_HAS_MINIMUM_POA 0
#  endif  /* TAO_HAS_MINIMUM_CORBA */
#endif  /* !TAO_HAS_MINIMUM_POA */

// MINIMUM_POA_MAPS support is disabled by default if TAO is not
// configured for minimum POA.  If TAO is configured for minimum
// POA, then MINIMUM_POA_MAPS will be enabled by default.
// To explicitly enable MINIMUM_POA_MAPS support uncomment the following
// #define TAO_HAS_MINIMUM_POA_MAPS 1
// To explicitly disable MINIMUM_POA_MAPS support uncomment the following
// #define TAO_HAS_MINIMUM_POA_MAPS 0

// Default MINIMUM_POA_MAPS settings
#if !defined (TAO_HAS_MINIMUM_POA_MAPS)
#  if (TAO_HAS_MINIMUM_POA == 1)
#    define TAO_HAS_MINIMUM_POA_MAPS 1
#  else
#    define TAO_HAS_MINIMUM_POA_MAPS 0
#  endif  /* TAO_HAS_MINIMUM_POA */
#endif  /* !TAO_HAS_MINIMUM_POA_MAPS */

// CORBA_MESSAGING support is enabled by default if TAO is not
// configured for minimum CORBA.  If TAO is configured for minimum
// CORBA, then CORBA_MESSAGING will be disabled by default.
// To explicitly enable CORBA_MESSAGING support uncomment the following
// #define TAO_HAS_CORBA_MESSAGING 1
// To explicitly disable CORBA_MESSAGING support uncomment the following
// #define TAO_HAS_CORBA_MESSAGING 0

// Default CORBA_MESSAGING settings
#if !defined (TAO_HAS_CORBA_MESSAGING)
#  if (TAO_HAS_MINIMUM_CORBA == 1)
#    define TAO_HAS_CORBA_MESSAGING 0
#  else
#    define TAO_HAS_CORBA_MESSAGING 1
#  endif  /* TAO_HAS_MINIMUM_CORBA */
#endif  /* !TAO_HAS_CORBA_MESSAGING */

// For all the policies, support is enabled by default if TAO is
// configured for CORBA Messaging.  If TAO is not configured for CORBA
// Messaging, then policies cannot be enabled.  Default support can be
// turned off by setting TAO_DISABLE_CORBA_MESSAGING_POLICIES to 1.

#if !defined (TAO_DISABLE_CORBA_MESSAGING_POLICIES)
# define TAO_DISABLE_CORBA_MESSAGING_POLICIES 0
#endif  /* !TAO_DISABLE_CORBA_MESSAGING_POLICIES */

// To explicitly disable REBIND_POLICY support uncomment the following
// #define TAO_HAS_REBIND_POLICY 0

// Default REBIND_POLICY settings
#if !defined (TAO_HAS_REBIND_POLICY)
#  if (TAO_HAS_CORBA_MESSAGING == 1) && \
      (TAO_DISABLE_CORBA_MESSAGING_POLICIES == 0)
#    define TAO_HAS_REBIND_POLICY 1
#  else
#    define TAO_HAS_REBIND_POLICY 0
#  endif  /* TAO_HAS_CORBA_MESSAGING == 1 &&
             TAO_DISABLE_CORBA_MESSAGING_POLICIES == 0 */
#endif  /* !TAO_HAS_REBIND_POLICY */

// To explicitly disable SYNC_SCOPE_POLICY support uncomment the following
// #define TAO_HAS_SYNC_SCOPE_POLICY 0

// Default SYNC_SCOPE_POLICY settings
#if !defined (TAO_HAS_SYNC_SCOPE_POLICY)
#  if (TAO_HAS_CORBA_MESSAGING == 1) && \
      (TAO_DISABLE_CORBA_MESSAGING_POLICIES == 0)
#    define TAO_HAS_SYNC_SCOPE_POLICY 1
#  else
#    define TAO_HAS_SYNC_SCOPE_POLICY 0
#  endif  /* TAO_HAS_CORBA_MESSAGING == 1 &&
             TAO_DISABLE_CORBA_MESSAGING_POLICIES == 0 */
#endif  /* !TAO_HAS_SYNC_SCOPE_POLICY */

// To explicitly disable PRIORITY_POLICIES support uncomment the following
// #define TAO_HAS_PRIORITY_POLICIES 0

// Default PRIORITY_POLICIES settings
#if !defined (TAO_HAS_PRIORITY_POLICIES)
#  if (TAO_HAS_CORBA_MESSAGING == 1) && \
      (TAO_DISABLE_CORBA_MESSAGING_POLICIES == 0)
#    define TAO_HAS_PRIORITY_POLICIES 1
#  else
#    define TAO_HAS_PRIORITY_POLICIES 0
#  endif  /* TAO_HAS_CORBA_MESSAGING == 1 &&
             TAO_DISABLE_CORBA_MESSAGING_POLICIES == 0 */
#endif  /* !TAO_HAS_PRIORITY_POLICIES */

// To explicitly disable REQUEST_START_TIME_POLICY support uncomment the following
// #define TAO_HAS_REQUEST_START_TIME_POLICY 0

// Default REQUEST_START_TIME_POLICY settings
#if !defined (TAO_HAS_REQUEST_START_TIME_POLICY)
#  if (TAO_HAS_CORBA_MESSAGING == 1) && \
      (TAO_DISABLE_CORBA_MESSAGING_POLICIES == 0)
#    define TAO_HAS_REQUEST_START_TIME_POLICY 1
#  else
#    define TAO_HAS_REQUEST_START_TIME_POLICY 0
#  endif  /* TAO_HAS_CORBA_MESSAGING == 1 &&
             TAO_DISABLE_CORBA_MESSAGING_POLICIES == 0 */
#endif  /* !TAO_HAS_REQUEST_START_TIME_POLICY */

// To explicitly disable REQUEST_END_TIME_POLICY support uncomment the following
// #define TAO_HAS_REQUEST_END_TIME_POLICY 0

// Default REQUEST_END_TIME_POLICY settings
#if !defined (TAO_HAS_REQUEST_END_TIME_POLICY)
#  if (TAO_HAS_CORBA_MESSAGING == 1) && \
      (TAO_DISABLE_CORBA_MESSAGING_POLICIES == 0)
#    define TAO_HAS_REQUEST_END_TIME_POLICY 1
#  else
#    define TAO_HAS_REQUEST_END_TIME_POLICY 0
#  endif  /* TAO_HAS_CORBA_MESSAGING == 1 &&
             TAO_DISABLE_CORBA_MESSAGING_POLICIES == 0 */
#endif  /* !TAO_HAS_REQUEST_END_TIME_POLICY */

// To explicitly disable REPLY_START_TIME_POLICY support uncomment the following
// #define TAO_HAS_REPLY_START_TIME_POLICY 0

// Default REPLY_START_TIME_POLICY settings
#if !defined (TAO_HAS_REPLY_START_TIME_POLICY)
#  if (TAO_HAS_CORBA_MESSAGING == 1) && \
      (TAO_DISABLE_CORBA_MESSAGING_POLICIES == 0)
#    define TAO_HAS_REPLY_START_TIME_POLICY 1
#  else
#    define TAO_HAS_REPLY_START_TIME_POLICY 0
#  endif  /* TAO_HAS_CORBA_MESSAGING == 1 &&
             TAO_DISABLE_CORBA_MESSAGING_POLICIES == 0 */
#endif  /* !TAO_HAS_REPLY_START_TIME_POLICY */

// To explicitly disable REPLY_END_TIME_POLICY support uncomment the following
// #define TAO_HAS_REPLY_END_TIME_POLICY 0

// Default REPLY_END_TIME_POLICY settings
#if !defined (TAO_HAS_REPLY_END_TIME_POLICY)
#  if (TAO_HAS_CORBA_MESSAGING == 1) && \
      (TAO_DISABLE_CORBA_MESSAGING_POLICIES == 0)
#    define TAO_HAS_REPLY_END_TIME_POLICY 1
#  else
#    define TAO_HAS_REPLY_END_TIME_POLICY 0
#  endif  /* TAO_HAS_CORBA_MESSAGING == 1 &&
             TAO_DISABLE_CORBA_MESSAGING_POLICIES == 0 */
#endif  /* !TAO_HAS_REPLY_END_TIME_POLICY */

// To explicitly disable RELATIVE_REQUEST_TIMEOUT_POLICY support uncomment the following
// #define TAO_HAS_RELATIVE_REQUEST_TIMEOUT_POLICY 0

// Default RELATIVE_REQUEST_TIMEOUT_POLICY settings
#if !defined (TAO_HAS_RELATIVE_REQUEST_TIMEOUT_POLICY)
#  if (TAO_HAS_CORBA_MESSAGING == 1) && \
      (TAO_DISABLE_CORBA_MESSAGING_POLICIES == 0)
#    define TAO_HAS_RELATIVE_REQUEST_TIMEOUT_POLICY 1
#  else
#    define TAO_HAS_RELATIVE_REQUEST_TIMEOUT_POLICY 0
#  endif  /* TAO_HAS_CORBA_MESSAGING == 1 &&
             TAO_DISABLE_CORBA_MESSAGING_POLICIES == 0 */
#endif  /* !TAO_HAS_RELATIVE_REQUEST_TIMEOUT_POLICY */

// To explicitly disable RELATIVE_ROUNDTRIP_TIMEOUT_POLICY support uncomment the following
// #define TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY 0

// Default RELATIVE_ROUNDTRIP_TIMEOUT_POLICY settings
#if !defined (TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY)
#  if (TAO_HAS_CORBA_MESSAGING == 1) && \
      (TAO_DISABLE_CORBA_MESSAGING_POLICIES == 0)
#    define TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY 1
#  else
#    define TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY 0
#  endif  /* TAO_HAS_CORBA_MESSAGING == 1 &&
             TAO_DISABLE_CORBA_MESSAGING_POLICIES == 0 */
#endif  /* !TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY */

// To explicitly disable ROUTING_POLICY support uncomment the following
// #define TAO_HAS_ROUTING_POLICY 0

// Default ROUTING_POLICY settings
#if !defined (TAO_HAS_ROUTING_POLICY)
#  if (TAO_HAS_CORBA_MESSAGING == 1) && \
      (TAO_DISABLE_CORBA_MESSAGING_POLICIES == 0)
#    define TAO_HAS_ROUTING_POLICY 1
#  else
#    define TAO_HAS_ROUTING_POLICY 0
#  endif  /* TAO_HAS_CORBA_MESSAGING == 1 &&
             TAO_DISABLE_CORBA_MESSAGING_POLICIES == 0 */
#endif  /* !TAO_HAS_ROUTING_POLICY */

// To explicitly disable MAX_HOPS_POLICY support uncomment the following
// #define TAO_HAS_MAX_HOPS_POLICY 0

// Default MAX_HOPS_POLICY settings
#if !defined (TAO_HAS_MAX_HOPS_POLICY)
#  if (TAO_HAS_CORBA_MESSAGING == 1) && \
      (TAO_DISABLE_CORBA_MESSAGING_POLICIES == 0)
#    define TAO_HAS_MAX_HOPS_POLICY 1
#  else
#    define TAO_HAS_MAX_HOPS_POLICY 0
#  endif  /* TAO_HAS_CORBA_MESSAGING == 1 &&
             TAO_DISABLE_CORBA_MESSAGING_POLICIES == 0 */
#endif  /* !TAO_HAS_MAX_HOPS_POLICY */

// To explicitly disable QUEUE_ORDER_POLICY support uncomment the following
// #define TAO_HAS_QUEUE_ORDER_POLICY 0

// Default QUEUE_ORDER_POLICY settings
#if !defined (TAO_HAS_QUEUE_ORDER_POLICY)
#  if (TAO_HAS_CORBA_MESSAGING == 1) && \
      (TAO_DISABLE_CORBA_MESSAGING_POLICIES == 0)
#    define TAO_HAS_QUEUE_ORDER_POLICY 1
#  else
#    define TAO_HAS_QUEUE_ORDER_POLICY 0
#  endif  /* TAO_HAS_CORBA_MESSAGING == 1 &&
             TAO_DISABLE_CORBA_MESSAGING_POLICIES == 0 */
#endif  /* !TAO_HAS_QUEUE_ORDER_POLICY */

// To explicitly disable CLIENT_PRIORITY_POLICY support uncomment the following
// #define TAO_HAS_CLIENT_PRIORITY_POLICY 0

// Default CLIENT_PRIORITY_POLICY settings
#if !defined (TAO_HAS_CLIENT_PRIORITY_POLICY)
#  if (TAO_HAS_CORBA_MESSAGING == 1) && \
      (TAO_DISABLE_CORBA_MESSAGING_POLICIES == 0)
#    define TAO_HAS_CLIENT_PRIORITY_POLICY 1
#  else
#    define TAO_HAS_CLIENT_PRIORITY_POLICY 0
#  endif  /* TAO_HAS_CORBA_MESSAGING == 1 &&
             TAO_DISABLE_CORBA_MESSAGING_POLICIES == 0 */
#endif  /* !TAO_HAS_CLIENT_PRIORITY_POLICY */

// To explicitly disable BUFFERING_CONSTRAINT_POLICY support uncomment the following
// #define TAO_HAS_BUFFERING_CONSTRAINT_POLICY 0

// Default BUFFERING_CONSTRAINT_POLICY settings
#if !defined (TAO_HAS_BUFFERING_CONSTRAINT_POLICY)
#  if (TAO_HAS_CORBA_MESSAGING == 1) && \
      (TAO_DISABLE_CORBA_MESSAGING_POLICIES == 0)
#    define TAO_HAS_BUFFERING_CONSTRAINT_POLICY 1
#  else
#    define TAO_HAS_BUFFERING_CONSTRAINT_POLICY 0
#  endif  /* TAO_HAS_CORBA_MESSAGING == 1 &&
             TAO_DISABLE_CORBA_MESSAGING_POLICIES == 0 */
#endif  /* !TAO_HAS_BUFFERING_CONSTRAINT_POLICY */

// AMI support is disabled by default, irrespective of whether TAO is
// configured for minimum CORBA.
//To explicitly enable AMI support uncomment the following
// #define TAO_HAS_AMI 1
// To explicitly disable AMI support uncomment the following
// #define TAO_HAS_AMI 0

// Default AMI settings
#if !defined (TAO_HAS_AMI)
#  define TAO_HAS_AMI 0
#endif  /* !TAO_HAS_AMI */

// AMI_POLLER support is disabled by default if TAO is not configured
// for AMI.  If TAO is configured for AMI, then AMI_POLLER will be
// enabled by default.
// To explicitly enable AMI_POLLER support uncomment the following
// #define TAO_HAS_AMI_POLLER 1
// To explicitly disable AMI_POLLER support uncomment the following
// #define TAO_HAS_AMI_POLLER 0

// Default AMI_POLLER settings
#if !defined (TAO_HAS_AMI_POLLER)
#  if (TAO_HAS_AMI == 1)
#    define TAO_HAS_AMI_POLLER 1
#  else
#    define TAO_HAS_AMI_POLLER 0
#  endif  /* TAO_HAS_AMI == 1 */
#endif  /* !TAO_HAS_AMI_POLLER */

// AMI_CALLBACK support is disabled by default if TAO is not configured
// for AMI.  If TAO is configured for AMI, then AMI_CALLBACK will be
// enabled by default.
// To explicitly enable AMI_CALLBACK support uncomment the following
// #define TAO_HAS_AMI_CALLBACK 1
// To explicitly disable AMI_CALLBACK support uncomment the following
// #define TAO_HAS_AMI_CALLBACK 0

// Default AMI_CALLBACK settings
#if !defined (TAO_HAS_AMI_CALLBACK)
#  if (TAO_HAS_AMI == 1)
#    define TAO_HAS_AMI_CALLBACK 1
#  else
#    define TAO_HAS_AMI_CALLBACK 0
#  endif  /* TAO_HAS_AMI == 1 */
#endif  /* !TAO_HAS_AMI_CALLBACK */

// INTERFACE_REPOSITORY support is disabled by default, irrespective
// of whether TAO is configured for minimum CORBA.
// To explicitly enable INTERFACE_REPOSITORY support uncomment the following
// #define TAO_HAS_INTERFACE_REPOSITORY 1
// To explicitly disable INTERFACE_REPOSITORY support uncomment the following
// #define TAO_HAS_INTERFACE_REPOSITORY 0

// Default INTERFACE_REPOSITORY settings
#if !defined (TAO_HAS_INTERFACE_REPOSITORY)
#  define TAO_HAS_INTERFACE_REPOSITORY 0
#endif  /* !TAO_HAS_INTERFACE_REPOSITORY */

// REMOTE_POLICIES support is enabled by default if TAO is not
// configured for minimum CORBA.  If TAO is configured for minimum
// CORBA, then REMOTE_POLICIES will be disabled by default.
// To explicitly enable REMOTE_POLICIES support uncomment the following
// #define TAO_HAS_REMOTE_POLICIES 1
// To explicitly disable REMOTE_POLICIES support uncomment the following
// #define TAO_HAS_REMOTE_POLICIES 0

// Default REMOTE_POLICIES settings
#if !defined (TAO_HAS_REMOTE_POLICIES)
#  if (TAO_HAS_MINIMUM_CORBA == 1)
#    define TAO_HAS_REMOTE_POLICIES 0
#  else
#    define TAO_HAS_REMOTE_POLICIES 1
#  endif  /* TAO_HAS_MINIMUM_CORBA */
#endif  /* !TAO_HAS_REMOTE_POLICIES */

// TAO_HAS_LOCALITY_CONSTRAINT_POLICIES is an internal macro and
// should not be set by the user.
#if defined (TAO_HAS_LOCALITY_CONSTRAINT_POLICIES)
# undef TAO_HAS_LOCALITY_CONSTRAINT_POLICIES
# warning TAO_HAS_LOCALITY_CONSTRAINT_POLICIES is an internal macro \
and should not be set by the user. Please use TAO_HAS_REMOTE_POLICIES instead.
#endif /* TAO_HAS_LOCALITY_CONSTRAINT_POLICIES */

#if (TAO_HAS_REMOTE_POLICIES == 0)
# define TAO_HAS_LOCALITY_CONSTRAINT_POLICIES
#endif /* TAO_HAS_REMOTE_POLICIES */

// With minimum CORBA, we don't have the ForwardRequest exception.
// Therefore, we can't support the INS forwarding agent.  Otherwise,
// we allow user to supress it.
#if (TAO_HAS_MINIMUM_CORBA == 1)
# if defined (TAO_NO_IOR_TABLE)
#   undef TAO_NO_IOR_TABLE
# endif /* TAO_NO_IOR_TABLE */
# define TAO_NO_IOR_TABLE 1
#else
# if !defined (TAO_NO_IOR_TABLE)
#  define TAO_NO_IOR_TABLE 0
# endif /* TAO_NO_IOR_TABLE */
#endif /* TAO_HAS_MINIMUM_CORBA */

// Interceptors is supported by default if we are not building for
// MinimumCORBA.
#if !defined (TAO_HAS_INTERCEPTORS)
# if (TAO_HAS_MINIMUM_CORBA == 1)
#   define TAO_HAS_INTERCEPTORS 0
# else
#   define TAO_HAS_INTERCEPTORS 1
# endif /* TAO_HAS_MINIMUM_CORBA */
#endif

// Define the policy types as literals, so they can be used in switch
// statements

#define TAO_CLIENT_PRIORITY_POLICY_TYPE 0x54410000
#define TAO_BUFFERING_CONSTRAINT_POLICY_TYPE 0x54410001

#define TAO_MESSAGING_REBIND_POLICY_TYPE 23
#define TAO_MESSAGING_SYNC_SCOPE_POLICY_TYPE 24
#define TAO_MESSAGING_REQUEST_PRIORITY_POLICY_TYPE 25
#define TAO_MESSAGING_REPLY_PRIORITY_POLICY_TYPE 26
#define TAO_MESSAGING_REQUEST_START_TIME_POLICY_TYPE 27
#define TAO_MESSAGING_REQUEST_END_TIME_POLICY_TYPE 28
#define TAO_MESSAGING_REPLY_START_TIME_POLICY_TYPE 29
#define TAO_MESSAGING_REPLY_END_TIME_POLICY_TYPE 30
#define TAO_MESSAGING_RELATIVE_REQ_TIMEOUT_POLICY_TYPE 31
#define TAO_MESSAGING_RELATIVE_RT_TIMEOUT_POLICY_TYPE 32
#define TAO_MESSAGING_ROUTING_POLICY_TYPE 33
#define TAO_MESSAGING_MAX_HOPS_POLICY_TYPE 34
#define TAO_MESSAGING_QUEUE_ORDER_POLICY_TYPE 35

// Control the default version of GIOP used by TAO.
// The ORB is always able to communicate with 1.0 and 1.1 servers, and
// it creates 1.1 endpoints (and profiles).  If you need to talk to
// old clients that only understand 1.0 (and do not attempt to use 1.0
// with 1.1 servers), then change the values below.
#if !defined (TAO_DEF_GIOP_MAJOR)
#define TAO_DEF_GIOP_MAJOR 1
#endif /* TAO_DEF_GIOP_MAJOR */
#if !defined (TAO_DEF_GIOP_MINOR)
#define TAO_DEF_GIOP_MINOR 1
#endif /* TAO_DEF_GIOP_MINOR */

// By default TAO generate the OMG standard profile components
// (ORB_TYPE and CODE_SETS)
#define TAO_STD_PROFILE_COMPONENTS

#if !defined (TAO_DEFAULT_IOR_SIZE)
// This is the default size of the buffer used for processing IORs.
#define TAO_DEFAULT_IOR_SIZE 1024
#endif /* TAO_DEFAULT_IOR_SIZE */

#if !defined (TAO_DEFAULT_THREAD_PER_CONNECTION_TIMEOUT)
// The default timeout value for the thread-per-connection model, in
// milliseconds. The "INFINITE" value is magic and means blocking forever.
#define TAO_DEFAULT_THREAD_PER_CONNECTION_TIMEOUT "5000"
#endif /* TAO_DEFAULT_THREAD_PER_CONNECTION_TIMEOUT */

// By default SMART_PROXIES is disabled.
# if !defined (TAO_HAS_SMART_PROXIES)
#    define TAO_HAS_SMART_PROXIES 0
#endif /*TAO_HAS_SMART_PROXIES*/

// SMART PROXIES support is disabled by default.
// To explicitly enable SMART_PROXIES support uncomment the following
// #define TAO_HAS_SMART_PROXIES 1

#include "ace/post.h"
#endif  /* TAO_ORB_CONFIG_H */
