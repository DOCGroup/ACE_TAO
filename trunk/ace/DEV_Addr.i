/* -*- C++ -*- */
// $Id$

#include "ace/OS_NS_string.h"
#include "ace/Global_Macros.h"
#include "ace/os_include/sys/os_socket.h"

ACE_INLINE void
ACE_DEV_Addr::set (const ACE_TCHAR *devname)
{
  ACE_TRACE ("ACE_DEV_Addr::set");

  this->ACE_Addr::base_set
    (AF_DEV, ACE_static_cast (int, ACE_OS::strlen (devname)));
  ACE_OS::strsncpy (this->devname_, devname, MAXNAMLEN);
}

// Transform the current address into string format.

ACE_INLINE int
ACE_DEV_Addr::addr_to_string (ACE_TCHAR *s, size_t len) const
{
  ACE_TRACE ("ACE_DEV_Addr::addr_to_string");

  ACE_OS::strsncpy (s, this->devname_, len);
  return 0;
}

// Return a pointer to the address.

ACE_INLINE void *
ACE_DEV_Addr::get_addr (void) const
{
  ACE_TRACE ("ACE_DEV_Addr::get_addr");

  return (void *) &this->devname_;
}

// Compare two addresses for equality.

ACE_INLINE int
ACE_DEV_Addr::operator == (const ACE_DEV_Addr &sap) const
{
  ACE_TRACE ("ACE_DEV_Addr::operator ==");

  return ACE_OS::strcmp (this->devname_, sap.devname_) == 0;
}

// Compare two addresses for inequality.

ACE_INLINE int
ACE_DEV_Addr::operator != (const ACE_DEV_Addr &sap) const
{
  ACE_TRACE ("ACE_DEV_Addr::operator !=");

  return !((*this) == sap);	// This is lazy, of course... ;-).
}

// Return the path name used for the rendezvous point.

ACE_INLINE const ACE_TCHAR *
ACE_DEV_Addr::get_path_name (void) const
{
  ACE_TRACE ("ACE_DEV_Addr::get_path_name");

  return this->devname_;
}
