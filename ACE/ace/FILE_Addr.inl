#include "ace/SString.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/// Compare two addresses for equality.

ACE_INLINE bool
ACE_FILE_Addr::operator == (ACE_FILE_Addr const &sap) const
{
  return ACE_OS::strcmp (this->filename_, sap.filename_) == 0;
}

/// Compare two addresses for inequality.

ACE_INLINE bool
ACE_FILE_Addr::operator != (ACE_FILE_Addr const &sap) const
{
  return !((*this) == sap);     // This is lazy, of course... ;-)
}

/// Return the path name used for the rendezvous point.

ACE_INLINE ACE_TCHAR const *
ACE_FILE_Addr::get_path_name () const
{
  return this->filename_;
}

ACE_END_VERSIONED_NAMESPACE_DECL
