ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTTP
  {

    ACE_INLINE
    void BasicAuthentication::user (ACE_CString const& usr)
      {
        this->user_ = usr;
      }

    ACE_INLINE
    ACE_CString const& BasicAuthentication::user () const
      {
        return this->user_;
      }

    ACE_INLINE
    void BasicAuthentication::password (ACE_CString const& passwd)
      {
        this->passwd_ = passwd;
      }

    ACE_INLINE
    ACE_CString const& BasicAuthentication::password () const
      {
        return this->passwd_;
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
