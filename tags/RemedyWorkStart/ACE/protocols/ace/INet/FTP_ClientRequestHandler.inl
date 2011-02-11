// -*- C++ -*-
//
// $Id$

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace FTP
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
    bool ClientRequestHandler::uses_passive_mode () const
      {
        return this->use_passive_mode_;
      }

    ACE_INLINE
    void ClientRequestHandler::use_passive_mode ()
      {
        this->use_passive_mode_ = true;
      }

    ACE_INLINE
    void ClientRequestHandler::use_active_mode (u_short port)
      {
        this->use_passive_mode_ = false;
        this->active_port_ = port;
      }

    ACE_INLINE
    u_short ClientRequestHandler::uses_active_port () const
      {
        return this->active_port_;
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
        if (this->session_)
          return this->out_data_stream_;
        else
          return ACE::IOS::Null::out_stream_;
      }

    ACE_INLINE
    std::istream& ClientRequestHandler::response_stream ()
      {
        if (this->session_)
          return this->in_data_stream_;
        else
          return ACE::IOS::Null::in_stream_;
      }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
