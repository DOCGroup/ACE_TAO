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

  ACE_DEV_Addr (LPCTSTR devname);
  // Create a ACE_DEV_Addr from a device name.

  void set (LPCTSTR devname);
  // Create a ACE_Addr from a ACE_DEV pathname.

  ACE_DEV_Addr &operator= (const ACE_DEV_Addr &);
  // Assignment operator.

  virtual void *get_addr (void) const;
  // Return a pointer to the address.

#if defined (UNICODE)
  virtual int addr_to_string (wchar_t *addr, size_t) const;
  // Transform the current address into string format.
#endif /* UNICODE */

  virtual int addr_to_string (char *addr, size_t) const;
  // Transform the current address into string format.

  int operator == (const ACE_DEV_Addr &SAP) const;
  // Compare two addresses for equality.

  int operator != (const ACE_DEV_Addr &SAP) const;
  // Compare two addresses for inequality.

  LPCTSTR get_path_name (void) const;
  // Return the path name used for the rendezvous point.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  TCHAR devname_[MAXNAMLEN + 1];
  // Name of the device.
};

#if defined (__ACE_INLINE__)
#include "ace/DEV_Addr.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_DEV_ADDR_H */
