// -*- C++ -*-
//
// $Id$

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace INet
  {
    ACE_INLINE
    const ACE_CString& URL_Base::get_protocol () const
      {
        return this->get_scheme ();
      }

    ACE_INLINE
    const ACE_CString& URL_Base::get_path () const
      {
        return this->path_;
      }

    ACE_INLINE
    void URL_Base::set_path (const ACE_CString& path)
      {
        this->path_ = path;
      }

    ACE_INLINE
    void URL_Base::set_query (const ACE_CString& /*query*/)
      {
      }

    ACE_INLINE
    void URL_Base::set_fragment (const ACE_CString& /*fragment*/)
      {
      }

    ACE_INLINE
    const ACE_CString& URL_Base::get_query () const
      {
        return empty_;
      }

    ACE_INLINE
    const ACE_CString& URL_Base::get_fragment () const
      {
        return empty_;
      }

    ACE_INLINE
    ACE_CString URL_Base::get_authority () const
      {
        return empty_;
      }

    ACE_INLINE
    const ACE_CString& URL_INetBase::get_host () const
      {
        return this->host_;
      }

    ACE_INLINE
    u_short URL_INetBase::get_port () const
      {
        return this->port_;
      }

    ACE_INLINE
    void URL_INetBase::set_host (const ACE_CString& host)
      {
        this->host_ = host;
      }

    ACE_INLINE
    void URL_INetBase::set_port (u_short port)
      {
        this->port_ = port;
      }

    ACE_INLINE
    const ACE_CString& URL_INetAuthBase::get_user_info () const
      {
        return this->userinfo_;
      }

    ACE_INLINE
    void URL_INetAuthBase::set_user_info (const ACE_CString& userinfo)
      {
        this->userinfo_ = userinfo;
      }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
