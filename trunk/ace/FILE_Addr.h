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
//    Douglas C. Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_FILE_ADDR_H
#define ACE_FILE_ADDR_H
#include "ace/pre.h"

#include "ace/Addr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Flag_Manip.h"

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

  ACE_EXPLICIT ACE_FILE_Addr (const ACE_TCHAR *filename);
  // Create a ACE_FILE_Addr from a pathname.

  int set (const ACE_TCHAR *filename);
  // Create a ACE_FILE_Addr from a pathname.

  ACE_FILE_Addr &operator= (const ACE_FILE_Addr &);
  // Assignment operator.

  virtual void *get_addr (void) const;
  // Return a pointer to the address.

  virtual int addr_to_string (ACE_TCHAR *addr, size_t) const;
  // Transform the current address into string format.

  int operator == (const ACE_FILE_Addr &SAP) const;
  // Compare two addresses for equality.

  int operator != (const ACE_FILE_Addr &SAP) const;
  // Compare two addresses for inequality.

  const ACE_TCHAR *get_path_name (void) const;
  // Return the path name used for the rendezvous point.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_TCHAR filename_[MAXNAMLEN + 1];
  // Name of the file.
};

#if defined (__ACE_INLINE__)
#include "ace/FILE_Addr.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_FILE_ADDR_H */
