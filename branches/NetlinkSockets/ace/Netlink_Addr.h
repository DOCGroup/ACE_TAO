// $Id$

//=============================================================================
/**
 *  @file    Netlink_Addr.h
 *
 *  Netlink_Addr.h,v 5.4.2 2004/10/10 
 *
 * @author Raz Ben Yehuda <raziebe@013.net.il> 
 */
//=============================================================================

#ifndef ACE_Netlink_ADDR_H
#define ACE_Netlink_ADDR_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /* */   "ace/os_include/sys/os_kernel_user.h" 
#include  /* */  "ace/Addr.h"


/**
 * @class ACE_Netlink_Addr
 *
 * @brief Defines a C++ wrapper facade for the netlink sockets
 */
class ACE_Export ACE_Netlink_Addr : public ACE_Addr
{
public:
  // = Initialization methods.
  
  // Default constructor.
  ACE_Netlink_Addr (void);

  /// Copy constructor.
  ACE_Netlink_Addr (const ACE_Netlink_Addr &);

  /// Creates an <ACE_INET_Addr> from a sockaddr_in structure.
  ACE_Netlink_Addr (const sockaddr_nl *, int len);


  ~ACE_Netlink_Addr (void);

  int set (const ACE_Netlink_Addr &);

  void set (int pid, int gid); 

  int set (const sockaddr_nl *, int len);

  int get_pid (void) const;

  int get_gid (void) const;

  virtual void *get_addr (void) const;
  
  int get_addr_size (void) const;

  /// Set a pointer to the address.
  virtual void set_addr (void *, int len= sizeof(sockaddr_nl) );

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:

  int determine_type (void) const;
  /// Initialize underlying inet_addr_ to default values
  void reset (void);

  sockaddr_nl  _nl; 
};

#if defined (__ACE_INLINE__)
#include "ace/Netlink_Addr.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* ACE_NETLINK_ADDR_H */



