// -*- C++ -*-

// $Id$
// ===================================================================
/*
 *  @file   SCIOP_Acceptor.h
 *
 *  SCIOP specific acceptor processing
 *
 *  @author  Jason Cohen, Lockheed Martin ATL  <jcohen@atl.lmco.com>
 *  @author  Keith O'Hara, Lockheed Martin ATL
 */
// ===================================================================


#ifndef TAO_SCIOP_ACCEPTOR_H
#define TAO_SCIOP_ACCEPTOR_H
#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if TAO_HAS_SCIOP == 1

#include "tao/Transport_Acceptor.h"
#include "tao/Strategies/SCIOP_Connection_Handler.h"
#include "tao/Acceptor_Impl.h"
#include "tao/GIOP_Message_Version.h"

#include "ace/SOCK_SEQPACK_Acceptor.h"
#include "ace/Acceptor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// TAO SCIOP_Acceptor concrete call definition

/**
 * @class TAO_SCIOP_Acceptor
 *
 * @brief TAO_SCIOP_Acceptor
 *
 * The SCIOP-specific bridge class for the concrete acceptor.
 */
class TAO_Strategies_Export TAO_SCIOP_Acceptor : public TAO_Acceptor
{
public:
  /// Constructor.
  TAO_SCIOP_Acceptor (void);

  /// Destructor.
  ~TAO_SCIOP_Acceptor (void);

  /// @@ Helper method for the implementation repository, should go
  ///    away
  const ACE_INET_Addr& address (void) const;

  /// Returns the array of endpoints in this acceptor
  const ACE_INET_Addr *endpoints (void);

  typedef TAO_Strategy_Acceptor<TAO_SCIOP_Connection_Handler, ACE_SOCK_SEQPACK_ACCEPTOR> TAO_SCIOP_BASE_ACCEPTOR;
  typedef TAO_Creation_Strategy<TAO_SCIOP_Connection_Handler> TAO_SCIOP_CREATION_STRATEGY;
  typedef TAO_Concurrency_Strategy<TAO_SCIOP_Connection_Handler> TAO_SCIOP_CONCURRENCY_STRATEGY;
  typedef TAO_Accept_Strategy<TAO_SCIOP_Connection_Handler, ACE_SOCK_SEQPACK_ACCEPTOR> TAO_SCIOP_ACCEPT_STRATEGY;

  /**
   * The TAO_Acceptor methods, check the documentation in
   * Transport_Acceptor.h for details.
   */
  virtual int open (TAO_ORB_Core *orb_core,
                    ACE_Reactor *reactor,
                    int version_major,
                    int version_minor,
                    const char *address,
                    const char *options = 0);
  virtual int open_default (TAO_ORB_Core *orb_core,
                            ACE_Reactor *reactor,
                            int version_major,
                            int version_minor,
                            const char *options = 0);
  virtual int close (void);
  virtual int create_profile (const TAO::ObjectKey &object_key,
                              TAO_MProfile &mprofile,
                              CORBA::Short priority);
  virtual int is_collocated (const TAO_Endpoint *endpoint);
  virtual CORBA::ULong endpoint_count (void);

  virtual int object_key (IOP::TaggedProfile &profile,
                          TAO::ObjectKey &key);

  /**
   * Set the host name for the given addr.
   * A hostname may be forced by using specified_hostname.  This
   * is useful if the given address corresponds to more than one
   * hostname and the desired one cannot be determined in any
   * other way.
   */
  int hostname (TAO_ORB_Core *orb_core,
                ACE_INET_Addr &addr,
                char *&host,
                const char *specified_hostname = 0);

  /**
   * Set the host name for the given address using the dotted decimal
   * format.
   */
  int dotted_decimal_address (ACE_INET_Addr &addr,
                              char *&host);

protected:

  /**
   * Implement the common part of the open*() methods.  This method is
   * virtual to allow a derived class implementation to be invoked
   * instead.
   */
  virtual int open_i (const ACE_Multihomed_INET_Addr &addr,
                      ACE_Reactor *reactor);

  /**
   * Probe the system for available network interfaces, and initialize
   * the <addrs_> array with an ACE_INET_Addr for each network
   * interface.  The port for each initialized ACE_INET_Addr will be
   * set in the open_i() method.  This method only gets invoked when
   * no explicit hostname is provided in the specified endpoint.
   */
  int probe_interfaces (TAO_ORB_Core *orb_core);

  /**
   * Split the string into hostnames separated by the plus character
   * ('+').
   */
  int parse_multiple_hostnames (const char *hostnames,
                                ACE_Array<ACE_CString> &hostnames_out);

  /**
   * Parse protocol specific options.
   *
   * Currently supported:
   *    portspan -- specifies the range of ports over which the acceptor
   *                should scan looking for a free port (this is convenient
   *                for situations where you might normally use an ephemeral
   *                port but can't because you're behind a firewall and don't
   *                want to permit passage on all ephemeral ports)
   */
  virtual int parse_options (const char *options);

  /// Obtain tcp properties that must be used by this acceptor, i.e.,
  /// initialize <tcp_properties_>.
  int init_tcp_properties (void);

  /// Helper method to add a new profile to the mprofile for
  /// each endpoint.
  int create_new_profile (const TAO::ObjectKey &object_key,
                          TAO_MProfile &mprofile,
                          CORBA::Short priority);

  /// Helper method to create a profile that contains all of
  /// our endpoints.
  int create_shared_profile (const TAO::ObjectKey &object_key,
                             TAO_MProfile &mprofile,
                             CORBA::Short priority);

protected:

  /// Array of ACE_INET_Addr instances, each one corresponding to a
  /// given network interface.
  ACE_INET_Addr *addrs_;

  /**
   * The number of ports over which the acceptor should search (starting
   * at the port specified in each element of addrs_) for an available
   * port.  This is specified via the "portspan=" option to the endpoint.
   */
  u_short port_span_;

  /**
   * Cache the information about the endpoints serviced by this
   * acceptor.
   * There may in fact be multiple hostnames for this endpoint. For
   * example, if the IP address is INADDR_ANY (0.0.0.0) then there
   * will be possibly a different hostname for each interface.
   */
  char **hosts_;

  /// The number of host names cached in the hosts_ array (equivalent
  /// to the number of endpoints opened by this Acceptor).
  CORBA::ULong endpoint_count_;

  /**
   * Override the hostname used in the ORBEndPoint.
   */
  char *hostname_in_ior_;

  /**
   * The GIOP version for this endpoint
   * @@ Theoretically they shouldn't be here!! We need to look at a
   * way to move this out
   */
  TAO_GIOP_Message_Version version_;

  /// ORB Core.
  TAO_ORB_Core *orb_core_;

private:

  /// the concrete acceptor, as a pointer to it's base class.
  TAO_SCIOP_BASE_ACCEPTOR base_acceptor_;

  /// Acceptor strategies.
  TAO_SCIOP_CREATION_STRATEGY *creation_strategy_;
  TAO_SCIOP_CONCURRENCY_STRATEGY *concurrency_strategy_;
  TAO_SCIOP_ACCEPT_STRATEGY *accept_strategy_;

};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(__ACE_INLINE__)
#include "tao/Strategies/SCIOP_Acceptor.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_SCIOP == 1 */

#include /**/ "ace/post.h"
#endif  /* TAO_SCIOP_ACCEPTOR_H */
