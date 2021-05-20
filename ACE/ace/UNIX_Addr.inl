// -*- C++ -*-

#include <cstring>

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

#if defined (ACE_HAS_WCHAR)
/// Creates an ACE_UNIX_Addr from a string.
ACE_INLINE
ACE_UNIX_Addr::ACE_UNIX_Addr (const wchar_t rendezvous_point[])
{
  this->set (ACE_Wide_To_Ascii (rendezvous_point).char_rep ());
}

/// Creates an ACE_UNIX_Addr from a string.
ACE_INLINE int
ACE_UNIX_Addr::set (const wchar_t rendezvous_point[])
{
  return this->set (ACE_Wide_To_Ascii (rendezvous_point).char_rep ());
}
#endif /* ACE_HAS_WCHAR */

/// Compare two addresses for equality.
ACE_INLINE bool
ACE_UNIX_Addr::operator == (const ACE_UNIX_Addr &sap) const
{
  return std::strncmp (this->unix_addr_.sun_path,
                       sap.unix_addr_.sun_path,
                       sizeof this->unix_addr_.sun_path) == 0;
}

/// Compare two addresses for inequality.
ACE_INLINE bool
ACE_UNIX_Addr::operator != (const ACE_UNIX_Addr &sap) const
{
  // This is lazy, of course... ;-)
  return !((*this) == sap);
}

/// Return the path name used for the rendezvous point.
ACE_INLINE const char *
ACE_UNIX_Addr::get_path_name () const
{
  return this->unix_addr_.sun_path;
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */
