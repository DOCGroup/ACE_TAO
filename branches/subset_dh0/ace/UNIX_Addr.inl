// -*- C++ -*-
//
// $Id$


#include "ace/OS_NS_string.h"


#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

// Return a pointer to the underlying address.

ACE_INLINE void *
ACE_UNIX_Addr::get_addr (void) const
{
  return (void *) &this->unix_addr_;
}

// Transform the string into the current addressing format.

ACE_INLINE int
ACE_UNIX_Addr::string_to_addr (const char addr[])
{
  ACE_OS::strsncpy (this->unix_addr_.sun_path, addr,
                    sizeof this->unix_addr_.sun_path);
  return 0;
}

// Transform the current address into string format.

ACE_INLINE int
ACE_UNIX_Addr::addr_to_string (ACE_TCHAR s[], size_t len) const
{
  ACE_OS::strsncpy (s,
                    ACE_TEXT_CHAR_TO_TCHAR (this->unix_addr_.sun_path),
                    len);
  return 0;
}

#if defined (ACE_HAS_WCHAR)
/// Creates an ACE_UNIX_Addr from a string.
ACE_INLINE
ACE_UNIX_Addr::ACE_UNIX_Addr (const wchar_t rendezvous_point[])
{
  this->set (ACE_TEXT_ALWAYS_CHAR (rendezvous_point));
}

/// Creates an ACE_UNIX_Addr from a string.
ACE_INLINE int
ACE_UNIX_Addr::set (const wchar_t rendezvous_point[])
{
  return this->set (ACE_TEXT_ALWAYS_CHAR (rendezvous_point));
}
#endif /* ACE_HAS_WCHAR */

// Compare two addresses for equality.

ACE_INLINE bool
ACE_UNIX_Addr::operator == (const ACE_UNIX_Addr &sap) const
{
  return ACE_OS::strncmp (this->unix_addr_.sun_path,
			  sap.unix_addr_.sun_path,
			  sizeof this->unix_addr_.sun_path) == 0;
}

// Compare two addresses for inequality.

ACE_INLINE bool
ACE_UNIX_Addr::operator != (const ACE_UNIX_Addr &sap) const
{
  return !((*this) == sap);	// This is lazy, of course... ;-)
}

// Return the path name used for the rendezvous point.

ACE_INLINE const char *
ACE_UNIX_Addr::get_path_name (void) const
{
  return this->unix_addr_.sun_path;
}

ACE_INLINE u_long
ACE_UNIX_Addr::hash (void) const
{
  return ACE::hash_pjw (this->unix_addr_.sun_path);
}

#endif /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */
