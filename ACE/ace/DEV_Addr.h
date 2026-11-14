// -*- C++ -*-

//==========================================================================
/**
 *  @file    DEV_Addr.h
 *
 *  @author Gerhard Lenzer and Douglas C. Schmidt
 */
//==========================================================================

#ifndef ACE_DEV_ADDR_H
#define ACE_DEV_ADDR_H

#include /**/ "ace/pre.h"

#include "ace/Addr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/os_dirent.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class ACE_DEV_Addr
 *
 * @brief Defines device address family address format.
 */
class ACE_Export ACE_DEV_Addr : public ACE_Addr
{
public:
  /// Default constructor.
  ACE_DEV_Addr ();

  ACE_DEV_Addr (ACE_DEV_Addr const &sa);

  /// Acts like a copy constructor.
  int set (ACE_DEV_Addr const &sa);

  /// Create a ACE_DEV_Addr from a device name.
  explicit ACE_DEV_Addr (ACE_TCHAR const *devname);

  /// Create a ACE_Addr from a ACE_DEV pathname.
  void set (ACE_TCHAR const *devname);

  ACE_DEV_Addr &operator= (ACE_DEV_Addr const &);

  /// Return a pointer to the address.
  void *get_addr () const override;

  /// Transform the current address into string format.
  virtual int addr_to_string (ACE_TCHAR *addr, size_t) const;

  /// Compare two addresses for equality.
  bool operator == (ACE_DEV_Addr const &sap) const;

  /// Compare two addresses for inequality.
  bool operator != (ACE_DEV_Addr const &sap) const;

  /// Return the path name used for the rendezvous point.
  ACE_TCHAR const *get_path_name () const;

  /// Dump the state of an object.
  void dump () const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  static constexpr int DEVNAME_LENGTH = MAXPATHLEN + 1;
  /// Name of the device.
  ACE_TCHAR devname_[DEVNAME_LENGTH] {};
};

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/DEV_Addr.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* ACE_DEV_ADDR_H */
