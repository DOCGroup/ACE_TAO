// -*- C++ -*-
//
// $Id$

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTTPS
  {

    ACE_INLINE
    const ACE_CString& URL::get_scheme () const
      {
        return protocol ();
      }

    ACE_INLINE
    u_short URL::default_port () const
      {
        return HTTPS_PORT;
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
