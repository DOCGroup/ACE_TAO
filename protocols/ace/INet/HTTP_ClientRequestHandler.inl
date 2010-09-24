// -*- C++ -*-
//
// $Id$

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTTP
  {
    ACE_INLINE
    ClientRequestHandler::SessionHolder::session_type& ClientRequestHandler::SessionHolder::operator *()
      {
        return this->session_;
      }

    ACE_INLINE
    ClientRequestHandler::SessionHolder::session_type* ClientRequestHandler::SessionHolder::operator -> ()
      {
        return &this->session_;
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
    ClientRequestHandler::SessionHolder& ClientRequestHandler::session ()
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
