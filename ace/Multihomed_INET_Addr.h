/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Multihomed_INET_Addr.h
 *
 *  $Id$
 *
 *  @author Edward R. Mulholland <emulholl@atl.lmco.com>
 */
//=============================================================================

#ifndef ACE_MULTIHOMED_INET_ADDR_H
#define ACE_MULTIHOMED_INET_ADDR_H
#include /**/ "ace/pre.h"

#include /**/ "ace/INET_Addr.h"
#include /**/ "ace/Containers_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ACE_Multihomed_INET_Addr
 *
 * @brief Extends ACE_INET_Addr with support for multi-homed
 * addresses.
 */
class ACE_Export ACE_Multihomed_INET_Addr : public ACE_INET_Addr
{
public:
  // = Initialization methods.

  /// Default constructor.
  ACE_Multihomed_INET_Addr (void);

  /**
   * Constructs an <ACE_Multihomed_INET_Addr> from a <port_number>, a
   * <primary_host_name>, and an array of <secondary_host_names>.
   * <size> is taken to be the length of the array.  If <encode> is
   * non-zero, then <port_number> is converted into network byte
   * order; otherwise it is assumed to be in network byte order
   * already and is passed straight through.  <address_family> can be
   * used to select IPv4/IPv6 if the OS has IPv6 capability
   * (ACE_HAS_IPV6 is defined). To specify IPv6, use the value
   * AF_INET6. To specify IPv4, use AF_INET.
   */
    ACE_Multihomed_INET_Addr(u_short port_number,
                           const char primary_host_name[],
                           int encode = 1,
                           int address_family = AF_UNSPEC,
                           const char *(secondary_host_names[]) = 0,
                           size_t size = 0);

  /**
   * Constructs an <ACE_Multihomed_INET_Addr> from a @a port_number,
   * a @a primary_ip_addr, and an array of @a secondary_ip_addrs.  @a
   * size is taken to be the length of the array.  If @a encode is
   * non-zero, then the port number and the IP addresses are converted
   * into network byte order; otherwise they are assumed to be in
   * network byte order already and are passed straight through.
   */
  ACE_Multihomed_INET_Addr(u_short port_number,
                           ACE_UINT32 primary_ip_addr = INADDR_ANY,
                           int encode = 1,
                           const ACE_UINT32 *secondary_ip_addrs = 0,
                           size_t size = 0);

  /// Use compiler-generated copy constructor.

  /// Use compiler-generated assignment operator.

  /// Default dtor.
  ~ACE_Multihomed_INET_Addr (void);

  // = Direct initialization methods.

  // These methods are useful after the object has been constructed.

  /**
   * Initializes an <ACE_Multihomed_INET_Addr> from a <port_number>, a
   * <primary_host_name>, and an array of <secondary_host_names>.
   * <size> is taken to be the length of the array.  If <encode> is
   * non-zero, then <port_number> is converted into network byte
   * order; otherwise it is assumed to be in network byte order
   * already and is passed straight through.  <address_family> can be
   * used to select IPv4/IPv6 if the OS has IPv6 capability
   * (ACE_HAS_IPV6 is defined). To specify IPv6, use the value
   * AF_INET6. To specify IPv4, use AF_INET.
   */
  int set (u_short port_number,
           const char primary_host_name[],
           int encode = 1,
           int address_family = AF_UNSPEC,
           const char *(secondary_host_names[]) = 0,
           size_t size = 0);

  /**
   * Initializes an <ACE_Multihomed_INET_Addr> from a @a port_number,
   * a @a primary_ip_addr, and an array of @a secondary_ip_addrs.  @a
   * size is taken to be the length of the array.  If @a encode is
   * non-zero, then the port number and the IP addresses are converted
   * into network byte order; otherwise they are assumed to be in
   * network byte order already and are passed straight through.
   */
  int set (u_short port_number,
           ACE_UINT32 primary_ip_addr = INADDR_ANY,
           int encode = 1,
           const ACE_UINT32 *secondary_ip_addrs = 0,
           size_t size = 0);

  // = Accessor methods.

  /**
   * Returns the number of secondary addresses.
   */
  size_t get_num_secondary_addresses () const;

  /**
   * Initialize user-supplied array <secondary_addrs> with the current
   * secondary addresses.  <size> is taken as the size of this array.
   */
  int get_secondary_addresses(ACE_INET_Addr *secondary_addrs,
                              size_t size) const;

  /**
   * Initialize user-supplied array <addrs> with the the current
   * primary and secondary addresses.  <size> is taken as the size of
   * this array.
   */
  void get_addresses(sockaddr_in *addrs,
                     size_t size) const;

private:

  ACE_Array<ACE_INET_Addr> secondaries;
};

#if defined (__ACE_INLINE__)
#include /**/ "ace/Multihomed_INET_Addr.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* ACE_MULTIHOMED_INET_ADDR_H */

