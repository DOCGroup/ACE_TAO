/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    FILE_Addr.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================


#ifndef ACE_FILE_ADDR_H
#define ACE_FILE_ADDR_H
#include "ace/pre.h"

#include "ace/Addr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Flag_Manip.h"

/**
 * @class ACE_FILE_Addr
 *
 * @brief Defines the FILE address family address format.
 */
class ACE_Export ACE_FILE_Addr : public ACE_Addr
{
public:
  // = Initialization methods.
  /// Default constructor.
  ACE_FILE_Addr (void);

  /// Copy constructor.
  ACE_FILE_Addr (const ACE_FILE_Addr &sa);

  /// Acts like a copy constructor.  If <sa> == ACE_Addr::sap_any then
  /// create a temporary filename using <ACE_OS::mktemp>.
  int set (const ACE_FILE_Addr &sa);

  /// Create a ACE_FILE_Addr from a pathname.
  ACE_EXPLICIT ACE_FILE_Addr (const ACE_TCHAR *filename);

  /// Create a ACE_FILE_Addr from a pathname.
  int set (const ACE_TCHAR *filename);

  /// Assignment operator.
  ACE_FILE_Addr &operator= (const ACE_FILE_Addr &);

  /// Return a pointer to the address.
  virtual void *get_addr (void) const;

  /// Transform the current address into string format.
  virtual int addr_to_string (ACE_TCHAR *addr, size_t) const;

  /// Compare two addresses for equality.
  int operator == (const ACE_FILE_Addr &SAP) const;

  /// Compare two addresses for inequality.
  int operator != (const ACE_FILE_Addr &SAP) const;

  /// Return the path name used for the rendezvous point.
  const ACE_TCHAR *get_path_name (void) const;

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Name of the file.
  ACE_TCHAR filename_[MAXNAMLEN + 1];
};

#if defined (__ACE_INLINE__)
#include "ace/FILE_Addr.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_FILE_ADDR_H */
