// -*- C++ -*-

//=============================================================================
/**
 *  @file     orbconf.h
 *
 *  $Id$
 *
 *  Build configuration file.
 *
 *  @author  Copyright 1995 by Sun Microsystems, Inc.
 *  @author  Chris Cleeland
 *  @author  Douglas C. Schmidt
 */
//=============================================================================

#ifndef TAO_ORBCONF_H
#define TAO_ORBCONF_H
#include /**/ "ace/pre.h"

// "ace/OS.h" is overkill.  "ace/Basic_Types.h" is enough.  In
// particular, it is needed for the definition of ACE_LITTLE_ENDIAN.
#include "ace/Basic_Types.h"
#include "ace/Global_Macros.h"
#include "ace/Synch_Traits.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (TAO_REACTOR)
#define TAO_REACTOR ACE_Select_Reactor
#endif /* TAO_REACTOR */

#if !defined (TAO_NULL_LOCK_REACTOR)
#define TAO_NULL_LOCK_REACTOR ACE_Select_Reactor_T< ACE_Select_Reactor_Token_T<ACE_Noop_Token> >
#endif /* TAO_NULL_LOCK_REACTOR */

// Define this to modify the default mutex type used throughout TAO.
#if !defined (TAO_SYNCH_MUTEX)
#define TAO_SYNCH_MUTEX ACE_SYNCH_MUTEX
#endif /* TAO_SYNCH_MUTEX */

#if !defined (TAO_SYNCH_RECURSIVE_MUTEX)
#define TAO_SYNCH_RECURSIVE_MUTEX ACE_SYNCH_RECURSIVE_MUTEX
#endif /* TAO_SYNCH_RECURSIVE_MUTEX */

#if !defined (TAO_SYNCH_RW_MUTEX)
#define TAO_SYNCH_RW_MUTEX ACE_SYNCH_RW_MUTEX
#endif /* TAO_SYNCH_RW_MUTEX */

#if !defined (TAO_SYNCH_CONDITION)
#define TAO_SYNCH_CONDITION ACE_SYNCH_CONDITION
#endif /* TAO_SYNCH_CONDITION */

// Define this if you don't want POA timestamps in the IOR.  Remember,
// without timestamps, transient and persistent POA cannot be
// distinguished.
#if !defined (POA_NO_TIMESTAMP)
# define POA_NO_TIMESTAMP 0
#endif /* POA_NO_TIMESTAMP */

// If set the ORB will use dotted decimal addresses in the IIOP IORs
// it exports.  This is useful for platforms or environments that
// cannot depend on a DNS being available.  This macro only affects
// the server side of a TAO-based CORBA application.
#if !defined (TAO_USE_DOTTED_DECIMAL_ADDRESSES)
# define TAO_USE_DOTTED_DECIMAL_ADDRESSES 0
#endif /* TAO_USE_DOTTED_DECIMAL_ADDRESSES */

// The default size of TAO's ORB table, i.e. the one used as the
// underlying implementation for the CORBA::ORB_init() function.
#if !defined (TAO_DEFAULT_ORB_TABLE_SIZE)
const size_t TAO_DEFAULT_ORB_TABLE_SIZE = 16;
#endif  /* !TAO_DEFAULT_ORB_TABLE_SIZE */

// The default size of the reply dispatcher table, ie. if the reply
// dispatcher strategy needs a table to maintain association between
// request id's and reply dispatchers
#if !defined (TAO_RD_TABLE_SIZE)
const size_t TAO_RD_TABLE_SIZE = 16;
#endif  /* !TAO_RD_TABLE_SIZE */

// The default size of TAO's policy factory registry, i.e. the map
// used as the underlying implementation for the
// PortableInterceptor::ORBInitInfo::register_policy_factory() method.
#if !defined (TAO_DEFAULT_POLICY_FACTORY_REGISTRY_SIZE)
const size_t TAO_DEFAULT_POLICY_FACTORY_REGISTRY_SIZE = 64;
#endif  /* !TAO_DEFAULT_ORB_TABLE_SIZE */

// The default size of TAO's initial object reference table, i.e. the
// one used as the underlying implementation for the
// CORBA::ORB::register_initial_reference() and
// PortableInterceptor::ORBInitInfo::register_initial_reference()
// methods.
#if !defined (TAO_DEFAULT_OBJECT_REF_TABLE_SIZE)
const size_t TAO_DEFAULT_OBJECT_REF_TABLE_SIZE = 256;
#endif  /* !TAO_DEFAULT_ORB_TABLE_SIZE */

// The default size of TAO's ValueFactory table, i.e. the
// one used as the underlying implementation for the
// CORBA::ORB::register_value_factory() method.
#if !defined (TAO_DEFAULT_VALUE_FACTORY_TABLE_SIZE)
const size_t TAO_DEFAULT_VALUE_FACTORY_TABLE_SIZE = 128;
#endif  /* !TAO_DEFAULT_ORB_TABLE_SIZE */


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

// @@todo: The connection caching strategy definition should probably
// go.
#if !defined (TAO_CONNECTION_CACHING_STRATEGY)
# define TAO_CONNECTION_CACHING_STRATEGY TAO_Resource_Factory::LRU
#endif /* TAO_CONNECTION_CACHING_STRATEGY */

#if !defined (TAO_CONNECTION_PURGING_STRATEGY)
# define TAO_CONNECTION_PURGING_STRATEGY TAO_Resource_Factory::LRU
#endif /* TAO_CONNECTION_PURGING_STRATEGY */


#if !defined (TAO_PURGE_PERCENT)
# define TAO_PURGE_PERCENT 20
#endif /* TAO_PURGE_PERCENT */

#if !defined (TAO_CONNECTION_CACHE_MAXIMUM)
# define TAO_CONNECTION_CACHE_MAXIMUM (ACE::max_handles () / 2)
#endif /* TAO_CONNECTION_CACHE_MAXIMUM */


// This definition theoretically is not required. Just leaving it here
// for backward compatibility
#define TAO_NAMESPACE namespace
#define TAO_NAMESPACE_CLOSE
#define TAO_NAMESPACE_STORAGE_CLASS extern TAO_EXPORT_MACRO
#define TAO_NAMESPACE_BEGIN(NS)  namespace NS {
#define TAO_NAMESPACE_END  }
#define TAO_NAMESPACE_TYPE(TYPE)
#define TAO_NAMESPACE_DEFINE(TYPE,NAME,RHS) TYPE NAME = RHS;


# if defined (_MSC_VER) && defined (__ACE_INLINE__)
#   define TAO_NAMESPACE_INLINE_FUNCTION inline
# else
#   define TAO_NAMESPACE_INLINE_FUNCTION TAO_NAMESPACE_STORAGE_CLASS
# endif

// Instead of replacing this with the ACE macro
// in 20+ files, define it conditionally.
// The TAO_OutputCDR class uses the ACE macro, which
// is defined by default.
#if !defined(TAO_NO_COPY_OCTET_SEQUENCES)
# define TAO_NO_COPY_OCTET_SEQUENCES 1
#endif /* TAO_NO_COPY_OCTET_SEQUENCES */

#if defined (ACE_HAS_EXCEPTIONS)
# define TAO_HAS_EXCEPTIONS
// TAO_USE_EXCEPTIONS is only here for backwards compatibility and
// will go away shortly so don't use it...
# define TAO_USE_EXCEPTIONS
#elif (defined (TAO_HAS_EXCEPTIONS) || defined (TAO_USE_EXCEPTIONS)) && !defined (ACE_HAS_EXCEPTIONS)
# error "tao/orbconf.h: You can only use exceptions in TAO if ACE supports them"
#endif /* TAO_HAS_EXCEPTIONS */

//-------- begin deprecated -----------------------------------------------
// The TAO_ENV_ macros have been renamed to ACE_ENV_
// and have been moved to $ACE_ROOT/ace/CORBA_macros.h.
// Please only use the latter.
// The macros below are deprecated and may be removed soon.
#define TAO_ENV_EMIT_CODE(X)                  ACE_ENV_EMIT_CODE(X)
#define TAO_ENV_EMIT_CODE2(X,Y)               ACE_ENV_EMIT_CODE2(X,Y)
#define TAO_ENV_ARG_DECL                      ACE_ENV_ARG_DECL
#define TAO_ENV_ARG_DECL_WITH_DEFAULTS        ACE_ENV_ARG_DECL_WITH_DEFAULTS
#define TAO_ENV_ARG_DECL_NOT_USED             ACE_ENV_ARG_DECL_NOT_USED
#define TAO_ENV_SINGLE_ARG_DECL               ACE_ENV_SINGLE_ARG_DECL
#define TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
#define TAO_ENV_SINGLE_ARG_DECL_NOT_USED      ACE_ENV_SINGLE_ARG_DECL_NOT_USED
#define TAO_ENV_ARG_PARAMETER                 ACE_ENV_ARG_PARAMETER
#define TAO_ENV_SINGLE_ARG_PARAMETER          ACE_ENV_SINGLE_ARG_PARAMETER
#define TAO_ENV_ARG_NOT_USED                  ACE_ENV_ARG_NOT_USED
#define TAO_ENV_RAISE(ex)                     ACE_ENV_RAISE(ex)
#define TAO_ENV_DECLARE_NEW_ENV               ACE_DECLARE_NEW_CORBA_ENV
//-------- end deprecated -------------------------------------------------

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

// @todo: It seems to be that this definition of TAO_ENCAP_BYTE_ORDER
// should be  removed. We have an equivalent ACE definition in
// ACE_CDR_BYTE_ORDER. Today both of them are consistent. It would be
// a havoc if oneday this consistency is gone..
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

#if defined (_MSC_VER)
// "C4355: 'this' : used in base member initializer list"
#  pragma warning (disable:4355) /* disable C4355 warning */
#endif /* defined (_MSC_VER) */

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
#if defined (AIX) || \
    (!defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION) && \
     (defined (ACE_HAS_TEMPLATE_SPECIALIZATION) || \
      (defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA) && defined (_UNICOS))))
#define TAO_USE_SEQUENCE_TEMPLATES
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

// The Root POA default name.
#if !defined (TAO_DEFAULT_ROOTPOA_NAME)
#define TAO_DEFAULT_ROOTPOA_NAME   ""
#endif /* TAO_DEFAULT_ROOTPOA_NAME */

// Minimum CORBA
#if !defined (TAO_HAS_MINIMUM_CORBA)
# define TAO_HAS_MINIMUM_CORBA 0
#endif /* TAO_HAS_MINIMUM_CORBA */

// Default DIOP settings
#if !defined (TAO_HAS_DIOP)
#    define TAO_HAS_DIOP 1
#endif  /* !TAO_HAS_DIOP */

// SCIOP is disabled by default (i.e. TAO_HAS_SCIOP is undef)
// to enable SCIOP, make with sctp=openss7 option on command line.
// See $ACE_ROOT/performance-tests/SCTP/README for more info.

// Default SCIOP Settings
#if !defined (TAO_HAS_SCIOP)
#  if defined (ACE_HAS_SCTP)
#    define TAO_HAS_SCIOP 1
#    define TAO_DEF_SCIOP_MAJOR 1
#    define TAO_DEF_SCIOP_MINOR 0
#  else
#    define TAO_HAS_SCIOP 0
#  endif  /* ACE_HAS_SCTP */
#endif  /* !TAO_HAS_SCIOP */

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
#  else
#    define TAO_HAS_UIOP 1
#  endif  /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */
#endif  /* !TAO_HAS_UIOP */

#if (ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1)
# if !defined (TAO_HAS_SHMIOP)
#   define TAO_HAS_SHMIOP 1
# endif /* TAO_HAS_SHMIOP */
#else
# if defined (TAO_HAS_SHMIOP)
#   undef TAO_HAS_SHMIOP
# endif /* TAO_HAS_SHMIOP */
# define TAO_HAS_SHMIOP 0
#endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1 */

// NAMED_RT_MUTEX support is disabled by default.
// To explicitly enable NAMED_RT_MUTEX support uncomment the following
// #define TAO_HAS_NAMED_RT_MUTEXES 1
// To explicitly disable NAMED_RT_MUTEX support uncomment the following
// #define TAO_HAS_NAMED_RT_MUTEXES 0

// Default NAMED_RT_MUTEX settings
#if !defined (TAO_HAS_NAMED_RT_MUTEXES)
#  define TAO_HAS_NAMED_RT_MUTEXES 0
#endif  /* !TAO_HAS_NAMED_RT_MUTEXES */

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

// Default CONNECTION_TIMEOUT_POLICY settings. This is a proprietary
// TAO policy.
#if !defined (TAO_HAS_CONNECTION_TIMEOUT_POLICY)
#  if (TAO_HAS_CORBA_MESSAGING == 1) && \
      (TAO_DISABLE_CORBA_MESSAGING_POLICIES == 0)
#    define TAO_HAS_CONNECTION_TIMEOUT_POLICY 1
#  else
#    define TAO_HAS_CONNECTION_TIMEOUT_POLICY 0
#  endif  /* TAO_HAS_CORBA_MESSAGING == 1 &&
             TAO_DISABLE_CORBA_MESSAGING_POLICIES == 0 */
#endif  /* !TAO_HAS_CONNECTION_TIMEOUT_POLICY */


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

// AMI support is enabled by default, unless TAO is compiled with
// Minimum CORBA.  In that case AMI is disabled by default.
// If disabled you can also disable the CORBA MESSAGING support.
// To explicitly enable AMI support uncomment the following
// #define TAO_HAS_AMI 1
// To explicitly disable AMI support uncomment the following
// #define TAO_HAS_AMI 0

// Default AMI settings
#if !defined (TAO_HAS_AMI)
#  if (TAO_HAS_MINIMUM_CORBA == 1)
#    define TAO_HAS_AMI 0
#  else
#    define TAO_HAS_AMI 1
#  endif  /* TAO_HAS_MINIMUM_CORBA */
#else
#  if (TAO_HAS_AMI == 1) && \
      (TAO_HAS_CORBA_MESSAGING == 0)
#  error "tao/orbconf.h: You need CORBA_MESSAGING for AMI support"
#  endif /* TAO_HAS_AMI == 1 &&
            TAO_HAS_CORBA_MESSAGING == 0 */
#endif  /* !TAO_HAS_AMI */

// We dont have AMI_POLLER support in TAO. Just prevent anyone from
// using it.

/***********************************************************/
// All these are for future use. Commenting it..

// #if !defined (TAO_HAS_AMI_POLLER)
// #  if (TAO_HAS_AMI == 1)
// #    define TAO_HAS_AMI_POLLER 1
// #  else
// #    define TAO_HAS_AMI_POLLER 0
// #  endif  /* TAO_HAS_AMI == 1 */
// #else
// #  if (TAO_HAS_AMI_POLLER == 1) && (TAO_HAS_CORBA_MESSAGING == 0)
// #  error "tao/orbconf.h: You need CORBA_MESSAGING for AMI support"
// #  endif /* TAO_HAS_AMI_POLLER == 1 &&
//             TAO_HAS_CORBA_MESSAGING == 0 */
// #endif  /* !TAO_HAS_AMI_POLLER */
/***********************************************************/

// AMI_CALLBACK support is disabled by default if TAO is not
// configured for AMI.  If TAO is configured for AMI, then
// AMI_CALLBACK will be enabled by default. If enabled, it requires
// CORBA_MESSAGING support.
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
#else
#  if (TAO_HAS_AMI_CALLBACK == 1) && \
      (TAO_HAS_CORBA_MESSAGING == 0)
#  error "tao/orbconf.h: You need CORBA_MESSAGING for AMI support"
#  endif /* TAO_HAS_AMI_CALLBACK == 1 &&
            TAO_HAS_CORBA_MESSAGING == 0 */
#endif  /* !TAO_HAS_AMI_CALLBACK */

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
#define TAO_CONNECTION_TIMEOUT_POLICY_TYPE 0x54410008

//  #define TAO_RT_PRIORITY_MODEL_POLICY_TYPE 0x54410002
//  #define TAO_RT_THREADPOOL_POLICY_TYPE 0x54410003
//  #define TAO_RT_PRIVATE_CONNECTION_POLICY_TYPE 0x54410006
//  #define TAO_RT_PRIORITY_BANDED_CONNECTION_POLICY_TYPE 0x54410007
//  #define TAO_RT_SERVER_PROTOCOL_POLICY_TYPE 0x54410004
//  #define TAO_RT_CLIENT_PROTOCOL_POLICY_TYPE 0x54410005


#define TAO_RT_PRIORITY_MODEL_POLICY_TYPE 40
#define TAO_RT_THREADPOOL_POLICY_TYPE 41
#define TAO_RT_SERVER_PROTOCOL_POLICY_TYPE 42
#define TAO_RT_CLIENT_PROTOCOL_POLICY_TYPE 43
#define TAO_RT_PRIVATE_CONNECTION_POLICY_TYPE 44
#define TAO_RT_PRIORITY_BANDED_CONNECTION_POLICY_TYPE 45

/// Policies that are accessed on the critical path and need to be
/// incur minimal retrieval overhead.
enum TAO_Cached_Policy_Type
{
  TAO_CACHED_POLICY_UNCACHED = -1,
  TAO_CACHED_POLICY_PRIORITY_MODEL = 0,
  TAO_CACHED_POLICY_THREADPOOL,
  TAO_CACHED_POLICY_RT_SERVER_PROTOCOL,
  TAO_CACHED_POLICY_RT_CLIENT_PROTOCOL,
  TAO_CACHED_POLICY_RT_PRIVATE_CONNECTION,
  TAO_CACHED_POLICY_RT_PRIORITY_BANDED_CONNECTION,

  TAO_CACHED_POLICY_LIFESPAN,
  TAO_CACHED_POLICY_ID_UNIQUENESS,
  TAO_CACHED_POLICY_ID_ASSIGNMENT,

#if (TAO_HAS_MINIMUM_POA == 0)
  TAO_CACHED_POLICY_THREAD,
  TAO_CACHED_POLICY_IMPLICIT_ACTIVATION,
  TAO_CACHED_POLICY_SERVANT_RETENTION,
  TAO_CACHED_POLICY_REQUEST_PROCESSING,
#endif /* TAO_HAS_MINIMUM_POA == 0 */

#if (TAO_HAS_SYNC_SCOPE_POLICY == 1)
  TAO_CACHED_POLICY_SYNC_SCOPE,
#endif /* TAO_HAS_SYNC_SCOPE_POLICY == 1 */

#if (TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1)
  TAO_CACHED_POLICY_RELATIVE_ROUNDTRIP_TIMEOUT,
#endif /* TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1 */

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)
  TAO_CACHED_POLICY_BUFFERING_CONSTRAINT,
#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

#if (TAO_HAS_CONNECTION_TIMEOUT_POLICY == 1)
  TAO_CACHED_POLICY_CONNECTION_TIMEOUT,
#endif /* TAO_HAS_CONNECTION_TIMEOUT_POLICY == 1 */

  TAO_CACHED_POLICY_BIDIRECTIONAL_GIOP,

  TAO_CACHED_POLICY_MAX_CACHED
};

// This enum provides a mask that represent the scope at which a given
// policy can be applied
enum TAO_Policy_Scope
{
  TAO_POLICY_OBJECT_SCOPE   = 0x01,
  TAO_POLICY_THREAD_SCOPE   = 0x02,
  TAO_POLICY_ORB_SCOPE      = 0x04,
  TAO_POLICY_POA_SCOPE      = 0x08,
  TAO_POLICY_CLIENT_EXPOSED = 0x10
};

#define TAO_POLICY_DEFAULT_SCOPE \
  (ACE_static_cast(TAO_Policy_Scope, \
                   TAO_POLICY_OBJECT_SCOPE | \
                   TAO_POLICY_THREAD_SCOPE | \
                   TAO_POLICY_ORB_SCOPE | \
                   TAO_POLICY_POA_SCOPE))

// Control the default version of GIOP used by TAO.
// The ORB is always able to communicate with 1.0, 1.1 and 1.2
// servers, and it creates 1.2 endpoints (and profiles).  If you need
// to talk to old clients that only understand 1.0 or 1.1 (and do not
// attempt to use 1.0 or 1.1 with 1.2 servers), then change the values
// below.
#if !defined (TAO_DEF_GIOP_MAJOR)
#define TAO_DEF_GIOP_MAJOR 1
#endif /* TAO_DEF_GIOP_MAJOR */
#if !defined (TAO_DEF_GIOP_MINOR)
#define TAO_DEF_GIOP_MINOR 2
#endif /* TAO_DEF_GIOP_MINOR */


// @@todo: this needs to be deprecated. Use TAO_MAXBUFSIZE instead.
#if !defined (TAO_CONNECTION_HANDLER_STACK_BUF_SIZE)
#   define TAO_CONNECTION_HANDLER_STACK_BUF_SIZE 1024
#endif /*TAO_CONNECTION_HANDLER_STACK_BUF_SIZE */

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

/// By default we use Muxed Transports
#if !defined (TAO_USE_MUXED_TRANSPORT_MUX_STRATEGY)
#  define TAO_USE_MUXED_TRANSPORT_MUX_STRATEGY 1
#endif /* TAO_USE_MUXED_TRANSPORT_MUX_STRATEGY */

/// By default we use multicast lookup
#if !defined(TAO_DEFAULT_INIT_REFERENCE_INITIALIZER)
#  define TAO_DEFAULT_INIT_REFERENCE_INITIALIZER "mcast://:::"
#endif /* TAO_DEFAULT_INIT_REFERENCE_INITIALIZER */

/// By default we dont use lazy strategy
#if !defined (TAO_USE_LAZY_RESOURCE_USAGE_STRATEGY)
#  define TAO_USE_LAZY_RESOURCE_USAGE_STRATEGY 0
#endif /* TAO_USE_LAZY_RESOURCE_USAGE_STRATEGY*/

#include /**/ "ace/post.h"
#endif  /* TAO_ORBCONF_H */
