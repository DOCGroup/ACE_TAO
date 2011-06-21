// -*- C++ -*-

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

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)

#include "tao/Transport_Acceptor.h"
#include "tao/IIOP_Connection_Handler.h"
#include "tao/Acceptor_Impl.h"
#include "tao/GIOP_Message_Version.h"

#include "ace/SOCK_Acceptor.h"
#include "ace/Acceptor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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

  /*
   * Hook that marks begining of all concrete i.e. non virtual
   * methods implemented in IIOP_Acceptor class.
   */
  //@@ TAO_ACCEPTOR_SPL_CONCRETE_METHODS_COPY_HOOK_START

  /// Constructor.
  TAO_IIOP_Acceptor (void);

  /// Destructor.
  ~TAO_IIOP_Acceptor (void);

  /// @@ Helper method for the implementation repository, should go
  ///    away
  const ACE_INET_Addr& address (void) const;

  /// Returns the array of endpoints in this acceptor
  const ACE_INET_Addr *endpoints (void);

  /// Returns address for default endpoint
  const ACE_INET_Addr& default_address (void) const;

  /// Set address for default endpoint
  void set_default_address (const ACE_INET_Addr& addr);

  typedef TAO_Strategy_Acceptor<TAO_IIOP_Connection_Handler, ACE_SOCK_ACCEPTOR> BASE_ACCEPTOR;
  typedef TAO_Creation_Strategy<TAO_IIOP_Connection_Handler> CREATION_STRATEGY;
  typedef TAO_Concurrency_Strategy<TAO_IIOP_Connection_Handler> CONCURRENCY_STRATEGY;
  typedef TAO_Accept_Strategy<TAO_IIOP_Connection_Handler, ACE_SOCK_ACCEPTOR> ACCEPT_STRATEGY;

  //@@ TAO_ACCEPTOR_SPL_CONCRETE_METHODS_COPY_HOOK_END

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

  //@@ TAO_ACCEPTOR_SPL_CONCRETE_METHODS_COPY_HOOK_START
  /**
   * Set the host name for the given @a addr.
   * A hostname may be forced by using @a specified_hostname.  This
   * is useful if the given address corresponds to more than one
   * hostname and the desired one cannot be determined in any
   * other way.
   * This method is used both when constructing IOR endpoints and LPL
   * (listen point lists).
   *
   * The algorithm used is:
   * - If hostname_in_ior_ has been set, it is used "as is";
   * - else if use_dotted_decimal_addresses_ is set, the text representaion
   *   of the IP is used;
   * - else if specified_hostname is given, it used "as is";
   * - else a reverse (address to name) lookup is used to obtain the
   *   hostname or the IP if no reverse mapping exists.
   */
  virtual int hostname (TAO_ORB_Core *orb_core,
                        const ACE_INET_Addr &addr,
                        char *&host,
                        const char *specified_hostname = 0);

protected:

  /**
   * Helper method
   * Clear out 'addr' & 'specified_hostname' and initialize them based
   * upon 'address'. If a non-zero pointer is passed in for def_type,
   * this will be set to AF_INET6 if IPv6 support is enabled and
   * supplied hostname is either [] or [::]. It will be set to AF_INET
   * if the hostname is 0.0.0.0, otherwise it is set to
   * AF_UNSPEC. This value is then passed to probe_interfaces by open.
   */
  int
  parse_address (const char *address,
                 ACE_INET_Addr &addr,
                 ACE_CString &specified_hostname,
                 int *def_type = 0);

  /**
   * Set the host name for the given address using the dotted decimal
   * format.
   */
  int dotted_decimal_address (const ACE_INET_Addr &addr,
                              char *&host);

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
   *
   * The optional argument def_type is used to constrain the resulting
   * list of interfaces to be either only IPv6 or IPv4, or both, when
   * ACE_HAS_IPV6 is enabled and the source endpoint was an explicitly
   * declared wildcard.
   */
  int probe_interfaces (TAO_ORB_Core *orb_core, int def_type = AF_UNSPEC);

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
  //@@ TAO_ACCEPTOR_SPL_CONCRETE_METHODS_COPY_HOOK_END

private:
  void operator= (const TAO_IIOP_Acceptor &);
  TAO_IIOP_Acceptor (const TAO_IIOP_Acceptor &);

  //@@ TAO_ACCEPTOR_SPL_DATA_MEMBERS_COPY_HOOK_START
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

  /// Enable socket option SO_REUSEADDR to be set
  int reuse_addr_;

  /// Address for default endpoint
  ACE_INET_Addr default_address_;

private:

  /// The concrete acceptor, as a pointer to it's base class.
  BASE_ACCEPTOR base_acceptor_;

  /// Acceptor strategies.
  CREATION_STRATEGY *creation_strategy_;
  CONCURRENCY_STRATEGY *concurrency_strategy_;
  ACCEPT_STRATEGY *accept_strategy_;

  //@@ TAO_ACCEPTOR_SPL_DATA_MEMBERS_COPY_HOOK_END
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(__ACE_INLINE__)
#include "tao/IIOP_Acceptor.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_IIOP && TAO_HAS_IIOP != 0 */

#include /**/ "ace/post.h"
#endif  /* TAO_IIOP_ACCEPTOR_H */
