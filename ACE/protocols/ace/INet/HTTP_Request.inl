ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTTP
  {

    ACE_INLINE
    void Request::reset ()
      {
        this->clear ();
        this->set_version (HTTP_1_0);
        this->method_ = HTTP_GET;
        this->uri_ = "/";
      }

    ACE_INLINE
    void Request::reset (ACE_CString const& version)
      {
        this->clear ();
        this->set_version (version);
        this->method_ = HTTP_GET;
        this->uri_ = "/";
      }

    ACE_INLINE
    void Request::reset (ACE_CString const& method, ACE_CString const& uri)
      {
        this->clear ();
        this->set_version (HTTP_1_0);
        this->method_ = method;
        this->uri_ = uri;
      }

    ACE_INLINE
    void Request::reset (ACE_CString const& method, ACE_CString const& uri, ACE_CString const& version)
      {
        this->clear ();
        this->set_version (version);
        this->method_ = method;
        this->uri_ = uri;
      }

    ACE_INLINE
    void Request::set_method(ACE_CString const& method)
      {
        this->method_ = method;
      }

    ACE_INLINE
    ACE_CString const& Request::get_method() const
      {
        return this->method_;
      }

    ACE_INLINE
    void Request::set_URI(ACE_CString const& uri)
      {
        this->uri_ = uri;
      }

    ACE_INLINE
    ACE_CString const& Request::get_URI() const
      {
        return this->uri_;
      }

    ACE_INLINE
    void Request::set_host(ACE_CString const& host)
      {
        this->set (HOST, host);
      }

    ACE_INLINE
    bool Request::has_host () const
      {
        return this->has (HOST);
      }

    ACE_INLINE
    ACE_CString Request::get_host() const
      {
        ACE_CString val;
        this->get (HOST, val);
        return val;
      }

    ACE_INLINE
    bool Request::has_credentials() const
      {
        return this->has (AUTHORIZATION);
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
