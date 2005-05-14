// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    IIOP_Acceptor.h
 *
 *  $Id$
 *
 *  IIOP specific acceptor processing
 *
 *
 *  @author Fred Kuhns <fredk@cs.wustl.edu>
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_IIOP_ACCEPTOR_H
#define TAO_IIOP_ACCEPTOR_H

#include /**/ "ace/pre.h"
#include "ace/Acceptor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SOCK_Acceptor.h"

#include "tao/Transport_Acceptor.h"
#include "tao/IIOP_Connection_Handler.h"
#include "tao/Acceptor_Impl.h"
#include "tao/GIOP_Message_Version.h"

/**
 * @class TAO_IIOP_Acceptor
 *
 * @brief TAO_IIOP_Acceptor
 *
 * The IIOP-specific bridge class for the concrete acceptor.
 */
class TAO_Export TAO_IIOP_Acceptor : public TAO_Acceptor
{
public:
  /// Constructor.
  TAO_IIOP_Acceptor (CORBA::Boolean flag = 0);

  /// Destructor.
  ~TAO_IIOP_Acceptor (void);

  /// @@ Helper method for the implementation repository, should go
  ///    away
  const ACE_INET_Addr& address (void) const;

  /// Returns the array of endpoints in this acceptor
  const ACE_INET_Addr *endpoints (void);

  typedef ACE_Strategy_Acceptor<TAO_IIOP_Connection_Handler, ACE_SOCK_ACCEPTOR> BASE_ACCEPTOR;
  typedef TAO_Creation_Strategy<TAO_IIOP_Connection_Handler> CREATION_STRATEGY;
  typedef TAO_Concurrency_Strategy<TAO_IIOP_Connection_Handler> CONCURRENCY_STRATEGY;
  typedef TAO_Accept_Strategy<TAO_IIOP_Connection_Handler, ACE_SOCK_ACCEPTOR> ACCEPT_STRATEGY;

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
  virtual int open_i (const ACE_INET_Addr &addr,
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
   * Parse protocol specific options.
   *
   * Currently supported:
   *    portspan -- specifies the range of ports over which the acceptor
   *                should scan looking for a free port (this is convenient
   *                for situations where you might normally use an ephemeral
   *                port but can't because you're behind a firewall and don't
   *                want to permit passage on all ephemeral ports)
   */
  int parse_options (const char *options);

  /**
   * Parse options splits the options list in to an argv array. This
   * allows manipulation of the list in a manner similar to
   * orb_init. By moving the consumed args to the tail of the list and
   * reducing the argc count, the base parse_options_i can be called
   * by derived parse_options_i. Method returns -1 if an ill-formed or
   * otherwise bogus arg is encountered. The only strictly bogus arg
   * was priority, which was a holdover from the early RT IIOP
   * implementations. an ill-formed option is one which is missing an
   * equal sign or something to the left of it.
   */
  virtual int parse_options_i (int &argc, ACE_CString ** argv);

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
  unsigned short port_span_;

  /**
   * Cache the information about the endpoints serviced by this
   * acceptor.
   * There may in fact be multiple hostnames for this endpoint. For
   * example, if the IP address is INADDR_ANY (0.0.0.0) then there
   * will be possibly a different hostname for each interface.
   */
  char **hosts_;

  /**
   * Override the hostname used in the ORBEndPoint.
   */
  char *hostname_in_ior_;

  /// The number of host names cached in the hosts_ array (equivalent
  /// to the number of endpoints opened by this Acceptor).
  CORBA::ULong endpoint_count_;

  /**
   * The GIOP version for this endpoint
   * @@ Theoretically they shouldn't be here!! We need to look at a
   * way to move this out
   */
  TAO_GIOP_Message_Version version_;

  /// ORB Core.
  TAO_ORB_Core *orb_core_;

  /// Should we use GIOP lite??
  const bool lite_flag_;

private:

  /// the concrete acceptor, as a pointer to it's base class.
  BASE_ACCEPTOR base_acceptor_;

  /// Acceptor strategies.
  CREATION_STRATEGY *creation_strategy_;
  CONCURRENCY_STRATEGY *concurrency_strategy_;
  ACCEPT_STRATEGY *accept_strategy_;

};

#if defined(__ACE_INLINE__)
#include "tao/IIOP_Acceptor.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif  /* TAO_IIOP_ACCEPTOR_H */
