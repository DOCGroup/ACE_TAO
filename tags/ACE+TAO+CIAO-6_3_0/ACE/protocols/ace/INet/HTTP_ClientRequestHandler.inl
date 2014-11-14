// -*- C++ -*-
//
// $Id$

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTTP
  {
    ACE_INLINE
    SessionBase& SessionHolder::operator *()
      {
        return this->session ();
      }

    ACE_INLINE
    SessionBase* SessionHolder::operator -> ()
      {
        return &this->session ();
      }

    ACE_INLINE
    bool ClientRequestHandler::HttpConnectionKey::is_proxy_connection () const
      {
        return this->proxy_connection_;
      }

    ACE_INLINE
    const ACE_CString& ClientRequestHandler::HttpConnectionKey::proxy_target_host () const
      {
        return this->proxy_target_host_;
      }

    ACE_INLINE
    u_short ClientRequestHandler::HttpConnectionKey::proxy_target_port () const
      {
        return this->proxy_target_port_;
      }

    ACE_INLINE
    Request& ClientRequestHandler::request ()
      {
        return this->request_;
      }

    ACE_INLINE
    const Request& ClientRequestHandler::request () const
      {
        return this->request_;
      }

    ACE_INLINE
    Response& ClientRequestHandler::response ()
      {
        return this->response_;
      }

    ACE_INLINE
    const Response& ClientRequestHandler::response () const
      {
        return this->response_;
      }

    ACE_INLINE
    SessionHolder& ClientRequestHandler::session ()
      {
        return *this->session_;
      }

    ACE_INLINE
    void ClientRequestHandler::session (SessionHolder* session)
      {
        this->session_ = session;
      }

    ACE_INLINE
    std::ostream& ClientRequestHandler::request_stream ()
      {
        return this->session_ ?
          this->session ()->request_stream (*this) :
          ACE::IOS::Null::out_stream_;
      }

    ACE_INLINE
    std::istream& ClientRequestHandler::response_stream ()
      {
        return this->session_ ?
          this->session ()->response_stream (*this) :
          ACE::IOS::Null::in_stream_;
      }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
