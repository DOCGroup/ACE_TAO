/* -*- C++ -*- */


//=============================================================================
/**
 *  @file    params.h
 *
 *  $Id$
 *
 *  @author Chris Cleeland
 */
//=============================================================================


#ifndef TAO_PARAMS_H
#define TAO_PARAMS_H
#include /**/ "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "objectid.h"
#include "CORBA_String.h"
#include "ace/SString.h"
#include "ace/Unbounded_Queue.h"


// Forward decls.

class TAO_Active_Object_Map_Impl;
class TAO_Reverse_Active_Object_Map_Impl;

// This is a quick hack to avoid having to unravel the intricacies of
// the all the hairy order interdepencies that currently exist in TAO.
// #if ! defined (__ACE_INLINE__)
// #define TAO_LOCAL_INLINE
// #else
// #define TAO_LOCAL_INLINE ACE_INLINE
// #endif /* ! __ACE_INLINE__ */


// @@ Using an ACE_Unbounded_Queue to contain the endpoints and
//    preconnects may not be the best container to use.  However, it
//    will only be used during ORB bootstrapping.  Also, a template
//    instantiation of ACE_Unbounded_Queue<ACE_CString> already exists
//    in ACE so we do not have to worry about increasing TAO's
//    footprint by using this container.
typedef ACE_Unbounded_Queue<ACE_CString> TAO_EndpointSet;
typedef ACE_Unbounded_Queue_Iterator<ACE_CString> TAO_EndpointSetIterator;


// -------------------------------------------------------------------

/**
 * @class TAO_ORB_Parameters
 *
 * @brief Parameters that are specific to the ORB.  These parameters can
 * be for the client, the server, or for both.
 * = NOTE
 * Some of the functions have inline definitions in the class header
 * below.  Please do not move them back to the .i file.
 * cygnus-2.7.2-960126, distributed with Tornado 1.0.1, can't deal
 * with returing a const reference here.  It raises an internal
 * compiler error at ace/Svc_Handler.cpp:46, of all places.
 */
class TAO_Export TAO_ORB_Parameters
{
public:
  /// Constructor.
  TAO_ORB_Parameters (void);

  /// Destructor.
  ~TAO_ORB_Parameters (void);

#if 0
  /*
   *  TODO: Need to be removed since preconnects are being
   *  discontinued.
   *
   */
  /// Specifies the endpoints to pre-establish connections on.
  int preconnects (ACE_CString &preconnects);
  TAO_EndpointSet &preconnects (void);
  void add_preconnect (ACE_CString &preconnect);
#endif /*if 0*/

  /// Specifies the endpoints on which this server is willing to
  /// listen for requests.
  int endpoints (ACE_CString &endpoints);
  TAO_EndpointSet &endpoints (void);
  void add_endpoint (ACE_CString &endpoint);

  /// Set/Get the port of services locatable through multicast.
  CORBA::UShort service_port (TAO_MCAST_SERVICEID service_id) const;
  void service_port (TAO_MCAST_SERVICEID service_id, CORBA::UShort port);

  /// Get/Set address:port for Multicast Discovery Protocol for
  /// the Naming Service.
  const char *mcast_discovery_endpoint (void) const;
  void mcast_discovery_endpoint (const char *mde);

  /// Set/Get the size to be used for a socket's receive buffer.
  int sock_rcvbuf_size (void) const;
  void sock_rcvbuf_size (int);

  /// Set/Get the size to be used for a socket's send buffer.
  int sock_sndbuf_size (void) const;
  void sock_sndbuf_size (int);

  /// Set/Get the status of whether to use TCP_NODELAY or not.
  int nodelay (void) const;
  void nodelay (int);

  /**
   * Octet sequences are marshalled without doing any copies, we
   * simply append a block to the CDR message block chain. When the
   * octet sequence is small enough and there is room in the current
   * message block it is more efficient just to copy the buffer.
   */
  int cdr_memcpy_tradeoff (void) const;
  void cdr_memcpy_tradeoff (int);

  /// The ORB will use the dotted decimal notation for addresses. By
  /// default we use the full ascii names.
  int use_dotted_decimal_addresses (void) const;
  void use_dotted_decimal_addresses (int);

  /// Set/Get the Init Reference of an arbitrary ObjectID.
  char *default_init_ref (void) const;
  void default_init_ref (const char *default_init_ref);

  /// Disable the OMG standard profile components, useful for
  /// homogenous environments.
  int std_profile_components (void) const;
  void std_profile_components (int x);

  /// Scheduling policy.
  /**
   * Scheduling policy specified by the user through the
   * -ORBSchedPolicy option.  This value is typically used by
   * functions like ACE_OS::thr_setprio() and
   * ACE_Sched_Params::priority_min(). Legal values are ACE_SCHED_RR,
   * ACE_SCHED_FIFO, and ACE_SCHED_OTHER.
   */
  int ace_sched_policy (void) const;
  void ace_sched_policy (int x);

  /// Scheduling policy flag.
  /**
   * Scheduling policy specified by the user through the
   * -ORBSchedPolicy option.  This value is typically used by ACE
   * thread creation functions. Legal values are THR_SCHED_RR,
   * THR_SCHED_FIFO, and THR_SCHED_DEFAULT.
   */
  long sched_policy (void) const;
  void sched_policy (long x);

  /// Scheduling scope flag.
  /**
   * Scheduling policy specified by the user through the
   * -ORBScopePolicy option.  This value is typically used by ACE
   * thread creation functions. Legal values are THR_SCOPE_SYSTEM and
   * THR_SCOPE_PROCESS.
   */
  long scope_policy (void) const;
  void scope_policy (long x);

  /// Thread creation flags.
  /**
   * Shorthand for OR'ing together the scope_policy and sched_policy.
   */
  long thread_creation_flags (void) const;

  /// Single read optimization.
  int single_read_optimization (void) const;
  void single_read_optimization (int x);

private:
  // Each "endpoint" is of the form:
  //
  //   protocol://V.v@addr1,...,W.w@addrN/
  //
  // or:
  //
  //   protocol://addr1,addr2,...,addrN/
  //
  // where "V.v" and "W.w" are optional versions.
  //
  // Multiple sets of endpoints may be seperated by a semi-colon `;'.
  // For example:
  //
  //   iiop://space:2001,odyssey:2010;uiop://foo,bar
  //
  // All preconnect or endpoint strings should be of the above form(s).

  int parse_endpoints (ACE_CString &endpoints,
                       TAO_EndpointSet &endpoints_list);

  /// List of endpoints used to pre-establish connections.
  // TAO_EndpointSet preconnects_list_;

  /// List of endpoints this server is willing to accept requests
  /// on.
  TAO_EndpointSet endpoints_list_;

  /// Port numbers of the configured services.
  CORBA::UShort service_port_[TAO_NO_OF_MCAST_SERVICES];

  /// address:port for Multicast Discovery Protocol for the Naming
  /// Service.
  CORBA::String_var mcast_discovery_endpoint_;

  /// List of comma separated prefixes from ORBDefaultInitRef.
  ACE_CString default_init_ref_;

  /// Size to be used for a socket's receive buffer.
  int sock_rcvbuf_size_;

  /// Size to be used for a socket's send buffer.
  int sock_sndbuf_size_;

  /// 1 if we're using TCP_NODELAY and 0 otherwise.
  int nodelay_;

  /// Control the strategy for copying vs. appeding octet sequences in
  /// CDR streams.
  int cdr_memcpy_tradeoff_;

  /// For selecting a liteweight version of the GIOP protocol.
  int use_lite_protocol_;

  /// For selecting a address notation
  int use_dotted_decimal_addresses_;

  /// If true then the standard OMG components are not generated.
  int std_profile_components_;

  /// Scheduling policy.
  /**
   * Scheduling policy specified by the user through the
   * -ORBSchedPolicy option.  This value is typically used by
   * functions like ACE_OS::thr_setprio() and
   * ACE_Sched_Params::priority_min(). Legal values are ACE_SCHED_RR,
   * ACE_SCHED_FIFO, and ACE_SCHED_OTHER.
   */
  int ace_sched_policy_;

  /// Scheduling policy flag.
  /**
   * Scheduling policy specified by the user through the
   * -ORBSchedPolicy option.  This value is typically used by ACE
   * thread creation functions. Legal values are THR_SCHED_RR,
   * THR_SCHED_FIFO, and THR_SCHED_DEFAULT.
   */
  long sched_policy_;

  /// Scheduling scope flag.
  /**
   * Scheduling policy specified by the user through the
   * -ORBScopePolicy option.  This value is typically used by ACE
   * thread creation functions. Legal values are THR_SCOPE_SYSTEM and
   * THR_SCOPE_PROCESS.
   */
  long scope_policy_;

  /// Single read optimization.
  int single_read_optimization_;
};

#if defined (__ACE_INLINE__)
# include "tao/params.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_PARAMS_H */
