/* -*- C++ -*- */
// $Id$

// FILE_Addr.i

// Do nothing constructor.

// Transform the current address into string format.

#include "ace/SString.h"

ACE_INLINE
ACE_FILE_Addr::ACE_FILE_Addr (void)
  : ACE_Addr (AF_FILE, sizeof this->filename_)
{
  (void) ACE_OS::memset ((void *) &this->filename_, 
			 0, sizeof this->filename_);
}

// Copy constructor. 

ACE_INLINE
ACE_FILE_Addr::ACE_FILE_Addr (const ACE_FILE_Addr &sa)
  : ACE_Addr (AF_FILE, ACE_OS::strlen (sa.filename_))
{
  (void) ACE_OS::memcpy ((void *) &this->filename_, 
			 (void *) &sa.filename_, sa.get_size ());
}

ACE_INLINE ACE_FILE_Addr &
ACE_FILE_Addr::operator= (const ACE_FILE_Addr &sa)
{
  if (this != &sa)
    (void) ACE_OS::memcpy ((void *) &this->filename_, 
			   (void *) &sa.filename_, 
			   sa.get_size ());
  return *this;
}

ACE_INLINE void 
ACE_FILE_Addr::set (LPCTSTR filename)
{

  this->ACE_Addr::base_set (AF_FILE, ACE_OS::strlen (filename) );
  (void) ACE_OS::strcpy (this->filename_, filename);
}

// Create a ACE_Addr from a ACE_FILE pathname.

ACE_INLINE
ACE_FILE_Addr::ACE_FILE_Addr (LPCTSTR filename)
{
  this->set (filename);
}

// Transform the current address into string format. 

ACE_INLINE int
ACE_FILE_Addr::addr_to_string (LPTSTR s, size_t len) const
{
  ACE_OS::strncpy (s, this->filename_, len);
  return 0;
}

// Return the address. 

ACE_INLINE void *
ACE_FILE_Addr::get_addr (void) const
{
  return (void *) &this->filename_;
}

// Compare two addresses for equality.

ACE_INLINE int
ACE_FILE_Addr::operator == (const ACE_Addr &sap) const
{
  return ACE_OS::strcmp (this->filename_,
			 ((ACE_FILE_Addr &) sap).filename_) == 0;
}

// Compare two addresses for inequality.

ACE_INLINE int
ACE_FILE_Addr::operator != (const ACE_Addr &sap) const
{
  return !((*this) == sap);	// This is lazy, of course... ;-) 
}

// Return the path name used for the rendezvous point.

ACE_INLINE LPCTSTR
ACE_FILE_Addr::get_path_name (void) const
{
  return this->filename_;
}

