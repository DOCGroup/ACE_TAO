// -*- C++ -*-

//=============================================================================
/**
 *  @file    DIOP_Acceptor.h
 *
 *  $Id$
 *
 *  DIOP specific acceptor processing
 *
 *  @author  Michael Kircher
 */
//=============================================================================


#ifndef TAO_DIOP_ACCEPTOR_H
#define TAO_DIOP_ACCEPTOR_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_DIOP) && (TAO_HAS_DIOP != 0)

#include "tao/Transport_Acceptor.h"
#include "tao/Strategies/DIOP_Connection_Handler.h"
#include "tao/Acceptor_Impl.h"

#include "tao/GIOP_Message_Version.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// TAO DIOP_Acceptor concrete call definition

/**
 * @class TAO_DIOP_Acceptor
 *
 * @brief TAO_DIOP_Acceptor
 *
 * The DIOP-specific bridge class for the concrete acceptor.
 */
class TAO_Strategies_Export TAO_DIOP_Acceptor : public TAO_Acceptor
{
public:
  /// Constructor.
  TAO_DIOP_Acceptor (void);

  /// Destructor.
  ~TAO_DIOP_Acceptor (void);

  /// @@ Helper method for the implementation repository, should go
  ///    away
  const ACE_INET_Addr& address (void) const;

  /// Returns the array of endpoints in this acceptor
  const ACE_INET_Addr *endpoints (void);

  /// Returns address for default endpoint
  const ACE_INET_Addr& default_address (void) const;

  /// Set address for default endpoint
  void set_default_address (const ACE_INET_Addr& addr);

  /**
   * @name The TAO_Acceptor Methods
   *
   * Please check the documentation in Transport_Acceptor.h for details.
   */
  //@{
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
  //@}

  /**
   * Set the host name for the given addr.
   * A hostname may be forced by using specified_hostname.  This
   * is useful if the given address corresponds to more than one
   * hostname and the desired one cannot be determined in any
   * other way.
   */
  virtual int hostname (TAO_ORB_Core *orb_core,
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
   * Helper method
   * Clear out 'addr' & 'specified_hostname' and initialize them based
   * upon 'address'. If a non-zero pointer is passed in for def_type,
   * this will be set to AF_INET6 if IPv6 support is enabled and
   * supplied hostname is either [] or [::]. It will be set to AF_INET
   * if the hostname is 0.0.0.0, otherwise it is set to
   * AF_UNSPEC. This value is then passed to probe_interfaces by open.
   */
  int parse_address (const char *address,
                     ACE_INET_Addr &addr,
                     ACE_CString &specified_hostname,
                     int *def_type = 0);

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

  /// Parse protocol specific options.
  virtual int parse_options (const char *options);

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
   * The GIOP version for this endpoint
   * @@ Theoretically they shouldn't be here!! We need to look at a
   * way to move this out
   */
  TAO_GIOP_Message_Version version_;

  /// ORB Core.
  TAO_ORB_Core *orb_core_;

  /// Address for default endpoint
  ACE_INET_Addr default_address_;

private:
  // @@ Frank: From DIOP_Acceptor.h
  TAO_DIOP_Connection_Handler *connection_handler_;

};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(__ACE_INLINE__)
#include "tao/Strategies/DIOP_Acceptor.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_DIOP && TAO_HAS_DIOP != 0 */

#include /**/ "ace/post.h"

#endif  /* TAO_DIOP_ACCEPTOR_H */
