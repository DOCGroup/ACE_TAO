ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTTP
  {

    ACE_INLINE
    ACE_CString const& URL::get_scheme () const
      {
        return protocol ();
      }

    ACE_INLINE
    ACE_CString const& URL::get_query () const
      {
        return this->query_;
      }

    ACE_INLINE
    ACE_CString const& URL::get_fragment () const
      {
        return this->fragment_;
      }

    ACE_INLINE
    void URL::set_query (ACE_CString const& query)
      {
        this->query_ = query;
      }

    ACE_INLINE
    void URL::set_fragment (ACE_CString const& fragment)
      {
        this->fragment_ = fragment;
      }

    ACE_INLINE
    void URL::set_proxy (ACE_CString const& proxy_host, u_short proxy_port)
      {
        this->proxy_host_ = proxy_host;
        this->proxy_port_ = proxy_port;
      }

    ACE_INLINE
    ACE_CString const& URL::get_proxy_host () const
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

    ACE_INLINE
    u_short URL::default_port () const
      {
        return HTTP_PORT;
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
