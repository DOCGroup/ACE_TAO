// -*- C++ -*-
//
// $Id$

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTTP
  {

#if defined (ACE_HAS_WCHAR)
    ACE_INLINE
    bool URL::parse (const ACE_WString& url_string)
      {
        return URL_Base::parse (url_string);
      }
#endif

    ACE_INLINE
    const ACE_CString& URL::get_protocol () const
      {
        return PROTOCOL;
      }

    ACE_INLINE
    const ACE_CString& URL::get_user_info () const
      {
        return this->userinfo_;
      }

    ACE_INLINE
    const ACE_CString& URL::get_path () const
      {
        return this->path_;
      }

    ACE_INLINE
    const ACE_CString& URL::get_query () const
      {
        return this->query_;
      }

    ACE_INLINE
    void URL::set_user_info (const ACE_CString& userinfo)
      {
        this->userinfo_ = userinfo;
      }

    ACE_INLINE
    void URL::set_path (const ACE_CString& path)
      {
        this->path_ = path;
      }

    ACE_INLINE
    void URL::set_query (const ACE_CString& query)
      {
        this->query_ = query;
      }

    ACE_INLINE
    void URL::set_proxy (const ACE_CString& proxy_host, u_short proxy_port)
      {
        this->proxy_host_ = proxy_host;
        this->proxy_port_ = proxy_port;
      }

    ACE_INLINE
    const ACE_CString& URL::get_proxy_host () const
      {
        return this->proxy_host_;
      }

    ACE_INLINE
    u_short URL::get_proxy_port () const
      {
        return this->proxy_port_;
      }

    ACE_INLINE
    bool URL::has_proxy () const
      {
        return !this->proxy_host_.empty ();
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
