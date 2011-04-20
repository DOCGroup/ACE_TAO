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

#include "ace/Basic_Types.h"
#include "ace/Global_Macros.h"
#include "ace/Condition_Thread_Mutex.h"
#include "ace/Synch_Traits.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

#define TAO_INVALID_PRIORITY -1

#if !defined (TAO_REACTOR)
#define TAO_REACTOR ACE_Select_Reactor
#endif /* TAO_REACTOR */

#if !defined (TAO_NULL_LOCK_REACTOR)
#define TAO_NULL_LOCK_REACTOR ACE_Select_Reactor_T< ACE_Reactor_Token_T<ACE_Noop_Token> >
#endif /* TAO_NULL_LOCK_REACTOR */

//FUZZ: disable check_for_ACE_SYNCH_MUTEX
// Define this to modify the default mutex type used throughout TAO.
#if !defined (TAO_SYNCH_MUTEX)
#define TAO_SYNCH_MUTEX ACE_SYNCH_MUTEX
#endif /* TAO_SYNCH_MUTEX */
//FUZZ: enable check_for_ACE_SYNCH_MUTEX

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
// the server side of a TAO-based CORBA application. If we are on Win32
// the DNS is general broken. Set it to 1 regardless.
#if !defined (TAO_USE_DOTTED_DECIMAL_ADDRESSES) && defined (ACE_WIN32)
# define TAO_USE_DOTTED_DECIMAL_ADDRESSES 1
#endif /*TAO_USE_DOTTED_DECIMAL_ADDRESSES*/

#if !defined (TAO_USE_DOTTED_DECIMAL_ADDRESSES)
# define TAO_USE_DOTTED_DECIMAL_ADDRESSES 0
#endif /* TAO_USE_DOTTED_DECIMAL_ADDRESSES */

// If set the ORB will use codeset negotiation. This means adding a
// Codeset component to each profile in an IOR, and a codeset context
// to the first request. The use of codeset negotiation also causes
// libTAO_Codeset to be linked in.
#if !defined (TAO_NEGOTIATE_CODESETS)
# define TAO_NEGOTIATE_CODESETS 1
#endif /*TAO_NEGOTIATE_CODESETS*/

// The default size of TAO's ORB table, i.e. the one used as the
// underlying implementation for the CORBA::ORB_init() function.
#if !defined (TAO_DEFAULT_ORB_TABLE_SIZE)
const size_t TAO_DEFAULT_ORB_TABLE_SIZE = 16;
#endif  /* !TAO_DEFAULT_ORB_TABLE_SIZE */

// The default size of TAO's adapter registry.
#if !defined (TAO_DEFAULT_ADAPTER_REGISTRY_SIZE)
const size_t TAO_DEFAULT_ADAPTER_REGISTRY_SIZE = 16;
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

// Should we use parallel connect
#if !defined (TAO_USE_PARALLEL_CONNECT)
const bool TAO_USE_PARALLEL_CONNECT = false;
#endif /* !TAO_USE_PARALLEL_CONNECT */

#if !defined (TAO_ACCEPT_ERROR_DELAY)
const time_t TAO_ACCEPT_ERROR_DELAY = 5;
#endif /* TAO_ACCEPT_ERROR_DELAY */

#if !defined (TAO_SO_LINGER)
const int TAO_SO_LINGER = -1;
#endif /* TAO_SO_LINGER */

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
# define TAO_DEFAULT_SERVER_ACTIVE_OBJECT_MAP_SIZE 64
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
// @@ This should go away!
#if !defined (TAO_DEFAULT_SERVER_PORT)
#define TAO_DEFAULT_SERVER_PORT 0
#endif /* TAO_DEFAULT_SERVER_PORT */

// TAO's max buffer size
#if !defined (TAO_MAXBUFSIZE)
#define TAO_MAXBUFSIZE 1024
#endif /* TAO_MAXBUFSIZE */

#if !defined (TAO_CONNECTION_PURGING_STRATEGY)
# define TAO_CONNECTION_PURGING_STRATEGY TAO_Resource_Factory::LRU
#endif /* TAO_CONNECTION_PURGING_STRATEGY */

#if !defined (TAO_PURGE_PERCENT)
# define TAO_PURGE_PERCENT 20
#endif /* TAO_PURGE_PERCENT */

#if !defined (TAO_CONNECTION_CACHE_MAXIMUM)
// If for some reason you configure the maximum number of handles in
// your OS to some astronomical value, then you should override this
// macro to be a smaller value, e.g., NOFILES or FD_SETSIZE.
# define TAO_CONNECTION_CACHE_MAXIMUM (ACE::max_handles () / 2)
#endif /* TAO_CONNECTION_CACHE_MAXIMUM */

// This definition theoretically is not required. Just leaving it here
// for backward compatibility
#define TAO_NAMESPACE_STORAGE_CLASS extern TAO_EXPORT_MACRO

# if defined (_MSC_VER) && defined (__ACE_INLINE__)
#   define TAO_NAMESPACE_INLINE_FUNCTION inline
# else
#   define TAO_NAMESPACE_INLINE_FUNCTION TAO_NAMESPACE_STORAGE_CLASS
# endif

#if !defined(TAO_NO_COPY_OCTET_SEQUENCES)
# define TAO_NO_COPY_OCTET_SEQUENCES 1
#endif /* TAO_NO_COPY_OCTET_SEQUENCES */

#define TAO_HAS_EXCEPTIONS

// BC++ seems to have a different convention for detecting Win32 than
// VC++.

#if defined (__WIN32__)
# if !defined(_WIN32)
#   define _WIN32
# endif /* !defined(_WIN32) */
#endif /* __WIN32__ */

// Define if your processor does not store words with the most significant
// byte first.

// @todo It seems to be that this definition of TAO_ENCAP_BYTE_ORDER
// should be removed. We have an equivalent ACE definition in
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

// Comment out to enable only ACE monitors...
#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)
#  define TAO_HAS_MONITOR_FRAMEWORK 1
#else
#  define TAO_HAS_MONITOR_FRAMEWORK 0
#endif /* ACE_HAS_MONITOR_FRAMEWORK*/

#if !defined (TAO_HAS_MONITOR_POINTS)
# define TAO_HAS_MONITOR_POINTS ACE_HAS_MONITOR_POINTS
#endif

#if !defined (TAO_HAS_ZIOP)
# define TAO_HAS_ZIOP 0
#endif

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

// The Root POA default name.
#if !defined (TAO_DEFAULT_ROOTPOA_NAME)
#define TAO_DEFAULT_ROOTPOA_NAME   "RootPOA"
#endif /* TAO_DEFAULT_ROOTPOA_NAME */

#if !defined (TAO_DEFAULT_ROOTPOAMANAGER_NAME)
#define TAO_DEFAULT_ROOTPOAMANAGER_NAME   "RootPOAManager"
#endif /* TAO_DEFAULT_ROOTPOAMANAGER_NAME */

// The default lane name used when storing endpoints.
#if !defined (TAO_DEFAULT_LANE)
#define TAO_DEFAULT_LANE "default lane"
#endif /* TAO_DEFAULT_LANE */

// Minimum CORBA
#if !defined (TAO_HAS_MINIMUM_CORBA)
# define TAO_HAS_MINIMUM_CORBA 0
#endif /* TAO_HAS_MINIMUM_CORBA */

/// Default IIOP settings
#if !defined (TAO_HAS_IIOP)
#  define TAO_HAS_IIOP 1
#endif  /* !TAO_HAS_IIOP */

/// Default DIOP settings
#if !defined (TAO_HAS_DIOP)
#  define TAO_HAS_DIOP 1
#endif  /* !TAO_HAS_DIOP */

/// Default SCIOP Settings
/// SCIOP is disabled by default (i.e. TAO_HAS_SCIOP is undef)
/// to enable SCIOP, make with sctp=openss7 option on command line.
/// See $ACE_ROOT/performance-tests/SCTP/README for more info.
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

// NSKPW and NSKFS are Pluggable Protocols used on the Tandem
// platform.  These are disabled by default.

#if defined (TAO_HAS_NSKPW)
#define TAO_DEF_NSKPW_MAJOR 1
#define TAO_DEF_NSKPW_MINOR 2
#endif

#if defined (TAO_HAS_NSKFS)
#define TAO_DEF_NSKFS_MAJOR 1
#define TAO_DEF_NSKFS_MINOR 2
#endif

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
#    define TAO_HAS_CORBA_MESSAGING 1
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
# if (TAO_HAS_MINIMUM_CORBA == 1) || defined (CORBA_E_COMPACT) || defined (CORBA_E_MICRO)
#   define TAO_HAS_INTERCEPTORS 0
# else
#   define TAO_HAS_INTERCEPTORS 1
# endif /* TAO_HAS_MINIMUM_CORBA */
#endif

// At the moment we have sendfile support in ACE we enabled this also by
// default for TAO, but we can suppress it also explicitly by set
// TAO_HAS_SENDFILE to 0.
#if !defined (TAO_HAS_SENDFILE)
# if defined ACE_HAS_SENDFILE && ACE_HAS_SENDFILE == 1
#  define TAO_HAS_SENDFILE 1
# else
#  define TAO_HAS_SENDFILE 0
# endif /* ACE_HAS_SENDFILE */
#endif /* !TAO_HAS_SENDFILE */

// Proprietary FT interception-point support is disabled by default.
#ifndef TAO_HAS_EXTENDED_FT_INTERCEPTORS
# define TAO_HAS_EXTENDED_FT_INTERCEPTORS 0
#endif  /* !TAO_HAS_EXTENEDED_FT_INTERCEPTORS */

// This is needed for a check in the Policy Set; Should really not be
// here though; We should find a different way to handle the check in
// the Policy Set.
#define TAO_RT_SERVER_PROTOCOL_POLICY_TYPE 42

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

  TAO_CACHED_POLICY_SERVER_NETWORK_PRIORITY,

  TAO_CACHED_POLICY_CLIENT_NETWORK_PRIORITY,

  TAO_CACHED_POLICY_NETWORK_PRIORITY,

  TAO_CACHED_POLICY_ENDPOINT,

  TAO_CACHED_COMPRESSION_ENABLING_POLICY,

  TAO_CACHED_COMPRESSION_LOW_VALUE_POLICY,

  TAO_CACHED_MIN_COMPRESSION_RATIO_POLICY,

  TAO_CACHED_COMPRESSION_ID_LEVEL_LIST_POLICY,

  /// NOTE: The "TAO_CACHED_POLICY_MAX_CACHED" should always be the last.
  ///       This value is used as the cached_policies_ array size in TAO_Policy_Set,
  ///       Any policy type defined after "TAO_CACHED_POLICY_MAX_CACHED" will cause
  ///       the policy access beyond bound problem.
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
  (static_cast<TAO_Policy_Scope> ( \
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

// Control the minimum required IIOP version for IPv6 support
#if defined (ACE_HAS_IPV6)
# if !defined (TAO_MIN_IPV6_IIOP_MAJOR)
#   define TAO_MIN_IPV6_IIOP_MAJOR 1
# endif /* TAO_MIN_IPV6_IIOP_MAJOR */
# if !defined (TAO_MIN_IPV6_IIOP_MINOR)
#   define TAO_MIN_IPV6_IIOP_MINOR 2
# endif /* TAO_MIN_IPV6_IIOP_MINOR */
#endif /* ACE_HAS_IPV6 */

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

#if !defined (TAO_USE_LOCAL_MEMORY_POOL)
#  define TAO_USE_LOCAL_MEMORY_POOL 1
#endif /* TAO_USE_LOCAL_MEMORY_POOL */

#if !defined (TAO_USE_OUTPUT_CDR_MMAP_MEMORY_POOL)
#  define TAO_USE_OUTPUT_CDR_MMAP_MEMORY_POOL 0
#endif /* TAO_USE_LOCAL_MEMORY_POOL */

// Enable TransportCurrent by default
#if !defined (TAO_HAS_TRANSPORT_CURRENT)
#    define TAO_HAS_TRANSPORT_CURRENT 1
#endif  /* ! TAO_HAS_TRANSPORT_CURRENT */

#if !defined (TAO_HAS_DDL_PARSER)
# define TAO_HAS_DDL_PARSER 1
#endif

#if !defined (TAO_HAS_FILE_PARSER)
# define TAO_HAS_FILE_PARSER 1
#endif

#if !defined (TAO_HAS_CORBALOC_PARSER)
# define TAO_HAS_CORBALOC_PARSER 1
#endif

#if !defined (TAO_HAS_CORBANAME_PARSER)
# define TAO_HAS_CORBANAME_PARSER 1
#endif

#if !defined (TAO_HAS_HTTP_PARSER)
# define TAO_HAS_HTTP_PARSER 1
#endif

#if !defined (TAO_HAS_MCAST_PARSER)
# if defined ACE_HAS_IP_MULTICAST
#  define TAO_HAS_MCAST_PARSER 1
# else
#  define TAO_HAS_MCAST_PARSER 0
# endif
#endif

#if !defined (TAO_HAS_SEQUENCE_ITERATORS)
#  define TAO_HAS_SEQUENCE_ITERATORS 0
#endif

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_ORBCONF_H */
