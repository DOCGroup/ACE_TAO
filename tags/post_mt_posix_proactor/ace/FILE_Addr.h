/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    FILE_Addr.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_FILE_ADDR_H
#define ACE_FILE_ADDR_H

#include "ace/Addr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/ACE.h"

class ACE_Export ACE_FILE_Addr : public ACE_Addr
{
  // = TITLE
  //    Defines the FILE address family address format.
public:
  // = Initialization methods.
  ACE_FILE_Addr (void);
  // Default constructor.

  ACE_FILE_Addr (const ACE_FILE_Addr &sa);
  // Copy constructor.

  int set (const ACE_FILE_Addr &sa);
  // Acts like a copy constructor.  If <sa> == ACE_Addr::sap_any then
  // create a temporary filename using <ACE_OS::mktemp>.

  ACE_FILE_Addr (LPCTSTR filename);
  // Create a ACE_FILE_Addr from a pathname.

  int set (LPCTSTR filename);
  // Create a ACE_FILE_Addr from a pathname.

  ACE_FILE_Addr &operator= (const ACE_FILE_Addr &);
  // Assignment operator.

  virtual void *get_addr (void) const;
  // Return a pointer to the address.

#if defined (UNICODE)
  virtual int addr_to_string (wchar_t *addr, size_t) const;
  // Transform the current address into string format.
#endif /* UNICODE */

  virtual int addr_to_string (char *addr, size_t) const;
  // Transform the current address into string format.

  int operator == (const ACE_FILE_Addr &SAP) const;
  // Compare two addresses for equality.

  int operator != (const ACE_FILE_Addr &SAP) const;
  // Compare two addresses for inequality.

  LPCTSTR get_path_name (void) const;
  // Return the path name used for the rendezvous point.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  TCHAR filename_[MAXNAMLEN + 1];
  // Name of the file.
};

#if defined (__ACE_INLINE__)
#include "ace/FILE_Addr.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_FILE_ADDR_H */
