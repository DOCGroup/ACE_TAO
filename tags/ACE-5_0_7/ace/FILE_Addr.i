/* -*- C++ -*- */
// $Id$

// FILE_Addr.i

// Do nothing constructor.

// Transform the current address into string format.

#include "ace/SString.h"

// Return the address.

ACE_INLINE void *
ACE_FILE_Addr::get_addr (void) const
{
  return (void *) &this->filename_;
}

// Compare two addresses for equality.

ACE_INLINE int
ACE_FILE_Addr::operator == (const ACE_FILE_Addr &sap) const
{
  return ACE_OS::strcmp (this->filename_, sap.filename_) == 0;
}

// Compare two addresses for inequality.

ACE_INLINE int
ACE_FILE_Addr::operator != (const ACE_FILE_Addr &sap) const
{
  return !((*this) == sap);	// This is lazy, of course... ;-)
}

// Return the path name used for the rendezvous point.

ACE_INLINE LPCTSTR
ACE_FILE_Addr::get_path_name (void) const
{
  return this->filename_;
}

