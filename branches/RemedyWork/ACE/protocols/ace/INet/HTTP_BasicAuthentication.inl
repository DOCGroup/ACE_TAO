// -*- C++ -*-
//
// $Id$

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTTP
  {

    ACE_INLINE
    void BasicAuthentication::user (const ACE_CString& usr)
      {
        this->user_ = usr;
      }

    ACE_INLINE
    const ACE_CString& BasicAuthentication::user () const
      {
        return this->user_;
      }

    ACE_INLINE
    void BasicAuthentication::password (const ACE_CString& passwd)
      {
        this->passwd_ = passwd;
      }

    ACE_INLINE
    const ACE_CString& BasicAuthentication::password () const
      {
        return this->passwd_;
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
