/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    SPIPE_Addr.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt
 */
//=============================================================================


#ifndef ACE_SPIPE_ADDR_H
#define ACE_SPIPE_ADDR_H
#include "ace/pre.h"

#include "ace/IPC/Addr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/ACE.h"

/**
 * @class ACE_SPIPE_Addr
 *
 * @brief Defines the SVR4 STREAM pipe address family address format.
 */
class ACE_Export ACE_SPIPE_Addr : public ACE_Addr
{
public:
  // = Initialization methods.
  /// Default constructor.
  ACE_SPIPE_Addr (void);

  /// Copy constructor.
  ACE_SPIPE_Addr (const ACE_SPIPE_Addr &sa);

  /// Create a ACE_SPIPE_Addr from a rendezvous point in the file
  /// system.
  ACE_SPIPE_Addr (const ACE_TCHAR *rendezvous_point, gid_t = 0, uid_t = 0);

  /// Acts like a copy constructor...
  int set (const ACE_SPIPE_Addr &sa);

  /// Create a ACE_SPIPE_Addr from a rendezvous point in the file
  /// system.
  int set (const ACE_TCHAR *rendezvous_point, gid_t = 0, uid_t = 0);

  /// Return a pointer to the address.
  virtual void *get_addr (void) const;

  /// Set a pointer to the underlying network address.
  virtual void set_addr (void *addr, int len);

  /// Transform the current address into string format.
  virtual int addr_to_string (ACE_TCHAR *addr, size_t) const;

  /// Transform the string into the current addressing format.
  virtual int string_to_addr (const ACE_TCHAR *addr);

  // = Equality/inequality tests
  /// Check for equality.
  int operator == (const ACE_SPIPE_Addr &SAP) const;

  /// Check for inequality
  int operator != (const ACE_SPIPE_Addr &SAP) const;

  // = SPIPE-specific address operations
  /// Pathname of rendezvous point in file system.
  const ACE_TCHAR *get_path_name (void) const;

  /// Get user id.
  /// Set user id.
  uid_t user_id (void) const;
  void user_id (uid_t uid);

  /// Set group ids.
  /// Get group ids.
  void group_id (gid_t gid);
  gid_t group_id (void) const;

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Contains security attributes.
  struct SPIPE_Addr
  {
    gid_t gid_;
    // Group id.

    uid_t uid_;
    // User id.

    ACE_TCHAR rendezvous_[MAXNAMLEN + 1];
    // Pathname in the file system.

  } SPIPE_addr_;
  // Contents of an SPIPE address.
};

#if defined (__ACE_INLINE__)
#include "ace/IPC/SPIPE_Addr.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_SPIPE_ADDR_H */
