// -*- C++ -*-
//
// $Id$

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace INet
  {
    ACE_INLINE
    AuthenticationBase::AuthenticationBase (const ACE_CString& realm,
                                            const ACE_CString& user,
                                            const ACE_CString& pw)
      : realm_ (realm),
        user_ (user),
        password_ (pw)
    {}

    ACE_INLINE
    const ACE_CString& AuthenticationBase::realm () const
      {
        return this->realm_;
      }

    ACE_INLINE
    const ACE_CString& AuthenticationBase::user () const
      {
        return this->user_;
      }

    ACE_INLINE
    void AuthenticationBase::user (const ACE_CString& usr)
      {
        this->user_ = usr;
      }

    ACE_INLINE
    const ACE_CString& AuthenticationBase::password () const
      {
        return this->password_;
      }

    ACE_INLINE
    void AuthenticationBase::password (const ACE_CString& pw)
      {
        this->password_ = pw;
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
