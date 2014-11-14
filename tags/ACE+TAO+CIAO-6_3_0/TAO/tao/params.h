// -*- C++ -*-

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
#include "tao/Invocation_Retry_Params.h"
#include "ace/Unbounded_Queue.h"
#include "ace/Array_Map.h"
#include "ace/Synch.h"
#include "ace/Time_Value.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SString.h"

#include "tao/objectid.h"
#include "tao/CORBA_String.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward decls.

// @@ Using an ACE_Unbounded_Queue to contain the endpoints and
//    preconnects may not be the best container to use.  However, it
//    will only be used during ORB bootstrapping.  Also, a template
//    instantiation of ACE_Unbounded_Queue<ACE_CString> already exists
//    in ACE so we do not have to worry about increasing TAO's
//    footprint by using this container.
typedef ACE_Unbounded_Queue<ACE_CString> TAO_EndpointSet;
typedef ACE_Unbounded_Queue_Const_Iterator<ACE_CString> TAO_EndpointSetIterator;

// -------------------------------------------------------------------



/**
 * @class TAO_ORB_Parameters
 *
 * @brief Parameters that are specific to the ORB.  These parameters can
 * be for the client, the server, or for both.
 */
class TAO_Export TAO_ORB_Parameters
{
public:

  typedef ACE_Array_Map<ACE_CString, ACE_CString> endpoints_map_type;

  /// Constructor.
  TAO_ORB_Parameters (void);

  /// Specifies the endpoints on which this server is willing to
  /// listen for requests.
  int add_endpoints (const ACE_CString &lane,
                     const ACE_CString &endpoints);
  void get_endpoint_set (const ACE_CString &lane,
                         TAO_EndpointSet &endpoint_set);

  /// Set/Get the port of services locatable through multicast.
  CORBA::UShort service_port (TAO::MCAST_SERVICEID service_id) const;
  void service_port (TAO::MCAST_SERVICEID service_id, CORBA::UShort port);

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

  /// Set/Get whether we should set SO_KEEPALIVE on the socket or not.
  int sock_keepalive (void);
  void sock_keepalive (int);

  /// Set/Get whether we should set SO_DONTROUTE on the socket or not.
  int sock_dontroute (void);
  void sock_dontroute (int);

  /// Set/Get the number of hops to be used for datagrams sent through socket.
  int ip_hoplimit (void);
  void ip_hoplimit (int);

  /// Set/Get whether we should set IP_MULTICAST_LOOP on the socket or not.
  bool ip_multicastloop (void);
  void ip_multicastloop (bool);

  /// Set/Get client side port span values
  u_short iiop_client_port_base (void);
  void iiop_client_port_base (u_short);
  u_short iiop_client_port_span (void);
  void iiop_client_port_span (u_short);

  /**
   * Octet sequences are marshalled without doing any copies, we
   * simply append a block to the CDR message block chain. When the
   * octet sequence is small enough and there is room in the current
   * message block it is more efficient just to copy the buffer.
   */
  int cdr_memcpy_tradeoff (void) const;
  void cdr_memcpy_tradeoff (int);

  /**
   * Maximum size of a GIOP message before outgoing fragmentation
   * kicks in.
   */
  //@{
  ACE_CDR::ULong max_message_size (void) const;
  void max_message_size (ACE_CDR::ULong size);
  //@}

  /// The ORB will use the dotted decimal notation for addresses. By
  /// default we use the full ascii names.
  int use_dotted_decimal_addresses (void) const;
  void use_dotted_decimal_addresses (int);

  /// The ORB will cache incoming connections against the dotted
  /// decimal form of the peer's address
  int cache_incoming_by_dotted_decimal_address (void) const;
  void cache_incoming_by_dotted_decimal_address (int);

  /// The ORB will turn off SO_LINGER if this is zero.
  int linger (void) const;
  void linger (int);

  /// The amount of time desired by the user to wait to accept
  /// connections after a particular type of accept() error.
  time_t accept_error_delay (void) const;
  void accept_error_delay (time_t);

  /// Set/Get the Init Reference of an arbitrary ObjectID.
  char *default_init_ref (void) const;
  void default_init_ref (const char *default_init_ref);

  /// Disable the OMG standard profile components, useful for
  /// homogenous environments.
  bool std_profile_components (void) const;
  void std_profile_components (bool x);

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

  /// Create shared profiles without priority
  int shared_profile (void) const;
  void shared_profile (int x);

  /// Want to use parallel connection attempts when profiles have multiple
  /// endpoints.
  bool use_parallel_connects(void) const;
  void use_parallel_connects (bool x);

  /// The milliseconds delay used to stagger individual connection starts
  /// when using parallel connects.
  unsigned long parallel_connect_delay (void) const;
  void parallel_connect_delay (unsigned long x);

  /// Mutators and accessors for rt_collocation_resolver
  bool disable_rt_collocation_resolver (void) const;
  void disable_rt_collocation_resolver (bool);

  /// Accepts the list of preferred interfaces and does a simple
  /// semantic check on the string before setting. Also the getter.
  bool preferred_interfaces (const char *s);
  const char *preferred_interfaces (void) const;

  /// Utility function called by the preferred interfaces setter
  static bool check_preferred_interfaces_string (const char *);

  void enforce_pref_interfaces (bool p);
  bool enforce_pref_interfaces (void) const;

#if defined (ACE_HAS_IPV6)
  void prefer_ipv6_interfaces (bool p);
  bool prefer_ipv6_interfaces (void) const;

  void connect_ipv6_only (bool p);
  bool connect_ipv6_only (void) const;

  void use_ipv6_link_local (bool p);
  bool use_ipv6_link_local (void) const;
#endif /* ACE_HAS_IPV6 */

  void negotiate_codesets (bool c);
  bool negotiate_codesets (void) const;

  void ami_collication (bool opt);
  bool ami_collication (void) const;

  void protocols_hooks_name (const char *s);
  const char *protocols_hooks_name (void) const;

  void thread_lane_resources_manager_factory_name (const char *s);
  const char *thread_lane_resources_manager_factory_name (void) const;

  void dynamic_thread_pool_config_name (const char *s);
  const char *dynamic_thread_pool_config_name (void) const;

  void stub_factory_name (const char *s);
  const char *stub_factory_name (void) const;

  void poa_factory_name (const char *s);
  const char *poa_factory_name (void) const;

  void poa_factory_directive (const ACE_TCHAR *s);
  const ACE_TCHAR *poa_factory_directive (void) const;

  void endpoint_selector_factory_name (const char *s);
  const char *endpoint_selector_factory_name (void) const;

  void collocation_resolver_name (const char *s);
  const char *collocation_resolver_name (void) const;

  void forward_invocation_on_object_not_exist (bool opt);
  bool forward_invocation_on_object_not_exist (void) const;

  void forward_on_exception_limit (const int ef, const int limit);
  void forward_on_exception_delay (const ACE_Time_Value &delay);

  TAO::Invocation_Retry_Params &invocation_retry_params (void);
  const TAO::Invocation_Retry_Params &invocation_retry_params (void) const;

  void forward_once_exception (const int ef);
  int forward_once_exception () const;

  void allow_ziop_no_server_policies (bool opt);
  bool allow_ziop_no_server_policies (void) const;

private:
  /// Each "endpoint" is of the form:
  ///
  ///   protocol://V.v@addr1,...,W.w@addrN/
  ///
  /// or:
  ///
  ///   protocol://addr1,addr2,...,addrN/
  ///
  /// where "V.v" and "W.w" are optional versions.
  ///
  /// Multiple sets of endpoints may be separated by a semi-colon `;'.
  /// For example:
  ///
  ///   iiop://space:2001,odyssey:2010;uiop://foo,bar
  ///
  /// All preconnect or endpoint strings should be of the above form(s).
  int parse_and_add_endpoints (const ACE_CString &endpoints,
                               TAO_EndpointSet &endpoint_set);

  /// Map of endpoints this server is willing to accept requests on.
  endpoints_map_type endpoints_map_;

  /// Port numbers of the configured services.
  CORBA::UShort service_port_[TAO_NO_OF_MCAST_SERVICES];

  /// Address:port for Multicast Discovery Protocol for the Naming
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

  /// 1 if we're using SO_KEEPALIVE and 0 otherwise (default 0).
  int sock_keepalive_;

  /// 1 if we're using SO_DONTROUTE and 0 otherwise (default 0).
  int sock_dontroute_;

  /// Number of hops to be used for datagrams sent through socket.
  int ip_hoplimit_;

  /// 1 if we're using IP_MULTICAST_LOOP and 0 otherwise.
  bool ip_multicastloop_;

  /// define a range for local ports to use with IIOP connections
  /// default base 0 to use ephemeral port. Span will be 0 if base is 0.
  u_short iiop_client_port_base_;
  u_short iiop_client_port_span_;

  /// Control the strategy for copying vs. appending octet sequences in
  /// CDR streams.
  int cdr_memcpy_tradeoff_;

  /// Maximum GIOP message size to be sent over a given transport.
  /**
   * Setting a maximum message size will cause outgoing GIOP
   * fragmentation to be enabled.
   */
  ACE_CDR::ULong max_message_size_;

  /// For selecting a address notation
  int use_dotted_decimal_addresses_;

  /// If incoming connections should be cached against IP (true) or
  /// hostname (false).
  int cache_incoming_by_dotted_decimal_address_;

  /// For setting the SO_LINGER option
  int linger_;

  /// For setting the accept retry delay
  time_t accept_error_delay_;

  /// If true then the standard OMG components are not generated.
  bool std_profile_components_;

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

  /// Shared Profile - Use the same profile for multiple endpoints
  int shared_profile_;

  /// Use Parallel Connects - Try to connect to all endpoints in a
  /// shared profile at once, use the first to complete.
  int use_parallel_connects_;

  /// When using parallel connects, this delay is used to stagger connection
  /// attempts. This gives a trade-off between opening more potential
  /// connections than necessary vs increasing the potential time before
  /// a good connection is discovered. Time is expressed in milliseconds.
  unsigned long parallel_connect_delay_;

  /// Preferred network interfaces as a string
  ACE_CString pref_network_;

  /// Default collocation resolver
  /**
   * The vanilla ORB has only one collocation resolver. But if the
   * RTORB is in place, the RTORB can get in a new collocation
   * resolver. There are some applications that would like to use the
   * default collocation resolver with the RTORB. This boolean is the
   * value of the option that the application passes in to enable/disable
   * the use of RT collocation resolver with the RTORB. The default value
   * is false to indicate that the RT_Collocation_Resolver will be
   * loaded if the RTORB is used.
   */
  bool disable_rt_collocation_resolver_;

  bool enforce_preferred_interfaces_;

#if defined (ACE_HAS_IPV6)
  /// Prefer to connect IPv6 over IPv4 Yes or No.
  bool prefer_ipv6_interfaces_;

  /**
   * Only connect to (client) or listen on and accept from (server)
   * IPv6 interfaces Yes or No.
   */
  bool connect_ipv6_only_;

  /// Include Link Local IPv6 profiles to IOR Yes or No.
  bool use_ipv6_link_local_;
#endif /* ACE_HAS_IPV6 */

  /// Enable the use of codeset negotiation
  bool negotiate_codesets_;

  /// Do we make collocated ami calls
  bool ami_collication_;

  /**
   * Name of the protocols_hooks that needs to be instantiated.
   * The default value is "Protocols_Hooks". If RTCORBA option is
   * set, its value will be set to be "RT_Protocols_Hooks".
   */
  ACE_CString protocols_hooks_name_;

  /**
   * Name of the stub factory that needs to be instantiated.
   * The default value is "Default_Stub_Factory". If TAO_RTCORBA is
   * linked, the set_stub_factory will be called to set the value
   * to be "RT_Stub_Factory".
   */
  ACE_CString stub_factory_name_;

  /**
   * Name of the endpoint selector factory that needs to be instantiated.
   * The default value is "Default_Endpoint_Selector_Factory". If
   * TAO_RTCORBA is linked, the set_endpoint_selector_factory will be
   * called to set the value to be "RT_Endpoint_Selector_Factory".
   */
  ACE_CString endpoint_selector_factory_name_;

  /**
   * Name of the thread lane resources manager that needs to be
   * instantiated.  The default value is
   * "Default_Thread_Lane_Resources_Manager_Factory". If TAO_RTCORBA
   * is linked, the set_thread_lane_resources_manager will be called
   * to set the value to be
   * "RT_Thread_Lane_Resources_Manager_Factory".
   */
  ACE_CString thread_lane_resources_manager_factory_name_;

  /**
   * Name of the non-RT dynamic thread pool configuration set to load.
   * This is only used if the Dynamic_TP library is linked. Default
   * is an empty string.
   */
  ACE_CString dynamic_thread_pool_config_name_;

  /**
   * Name of the service object used to create the RootPOA.  The
   * default value is "TAO_POA".  If TAO_RTCORBA is loaded, this
   * will be changed to TAO_RT_POA so that a POA equipped with
   * realtime extensions will be returned.
   */
  ACE_CString poa_factory_name_;

  /**
   * The service configurator directive used to load
   * poa_factory_name_ dynamically.
   */
  ACE_TString poa_factory_directive_;


  /**
   * Do we need forward invocation to next avaiable profile upon
   * OBJECT_NOT_EXIST exception?
   */
  bool forward_invocation_on_object_not_exist_;

  TAO::Invocation_Retry_Params invocation_retry_params_;

  /**
   * The exceptions upon which the requests will be forwarded once.
   * This is retained for backward compatibility of behavior.
   */
  int forward_once_exception_;

/**
   * Name of the collocation resolver that needs to be instantiated.
   * The default value is "Default_Collocation_Resolver". If
   * TAO_RTCORBA is linked, the set_collocation_resolver will be
   * called to set the value to be "RT_Collocation_Resolver".
   */
  ACE_CString collocation_resolver_name_;

  // Allows a client to ZIOP compress without having any server
  // ZIOP available compressor's list policies. THIS IS GOING DIRECTLY
  // AGAINST THE CORBA Compressed GIOP (ZIOP) V1.0 specification, but
  // allows us to use ZIOP with MIOP and/or CORBALOCs.
  // We have to trust the end user knows what his system is configured to allow;
  // Any servers that cannot decompress the client's used ZIOP compressor will
  // reject the request as they simply cannot decode or handle it (comms will
  // simply timeout or lock-up at the client for any such incorrect two-way requests).
  bool allow_ziop_no_server_policies_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/params.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_PARAMS_H */
