ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace INet
  {
    ACE_INLINE
    ACE_CString const& URL_Base::get_protocol () const
      {
        return this->get_scheme ();
      }

    ACE_INLINE
    ACE_CString const& URL_Base::get_path () const
      {
        return this->path_;
      }

    ACE_INLINE
    void URL_Base::set_path (ACE_CString const& path)
      {
        this->path_ = path;
      }

    ACE_INLINE
    void URL_Base::set_query (ACE_CString const& /*query*/)
      {
      }

    ACE_INLINE
    void URL_Base::set_fragment (ACE_CString const& /*fragment*/)
      {
      }

    ACE_INLINE
    ACE_CString const& URL_Base::get_query () const
      {
        return empty_;
      }

    ACE_INLINE
    ACE_CString const& URL_Base::get_fragment () const
      {
        return empty_;
      }

    ACE_INLINE
    ACE_CString URL_Base::get_authority () const
      {
        return empty_;
      }

    ACE_INLINE
    ACE_CString const& URL_INetBase::get_host () const
      {
        return this->host_;
      }

    ACE_INLINE
    u_short URL_INetBase::get_port () const
      {
        return this->port_;
      }

    ACE_INLINE
    void URL_INetBase::set_host (ACE_CString const& host)
      {
        this->host_ = host;
      }

    ACE_INLINE
    void URL_INetBase::set_port (u_short port)
      {
        this->port_ = port;
      }

    ACE_INLINE
    ACE_CString const& URL_INetAuthBase::get_user_info () const
      {
        return this->userinfo_;
      }

    ACE_INLINE
    void URL_INetAuthBase::set_user_info (ACE_CString const& userinfo)
      {
        this->userinfo_ = userinfo;
      }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
