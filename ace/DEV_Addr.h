/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    DEV_Addr.h
//
// = AUTHOR
//    Gerhard Lenzer and Douglas C. Schmidt
//
// ============================================================================

#ifndef ACE_DEV_ADDR_H
#define ACE_DEV_ADDR_H
#include "ace/pre.h"

#include "ace/Addr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/ACE.h"

class ACE_Export ACE_DEV_Addr : public ACE_Addr
{
  // = TITLE
  //    Defines device address family address format.
public:
  // = Initialization methods.
  ACE_DEV_Addr (void);
  // Default constructor.

  ACE_DEV_Addr (const ACE_DEV_Addr &sa);
  // Copy constructor.

  int set (const ACE_DEV_Addr &sa);
  // Acts like a copy constructor.

  ACE_EXPLICIT ACE_DEV_Addr (const ACE_TCHAR *devname);
  // Create a ACE_DEV_Addr from a device name.

  void set (const ACE_TCHAR *devname);
  // Create a ACE_Addr from a ACE_DEV pathname.

  ACE_DEV_Addr &operator= (const ACE_DEV_Addr &);
  // Assignment operator.

  virtual void *get_addr (void) const;
  // Return a pointer to the address.

  virtual int addr_to_string (ACE_TCHAR *addr, size_t) const;
  // Transform the current address into string format.

  int operator == (const ACE_DEV_Addr &SAP) const;
  // Compare two addresses for equality.

  int operator != (const ACE_DEV_Addr &SAP) const;
  // Compare two addresses for inequality.

  const ACE_TCHAR *get_path_name (void) const;
  // Return the path name used for the rendezvous point.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_TCHAR devname_[MAXNAMLEN + 1];
  // Name of the device.
};

#if defined (__ACE_INLINE__)
#include "ace/DEV_Addr.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_DEV_ADDR_H */
