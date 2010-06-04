// -*- C++ -*-
//
// $Id$

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace INet
  {
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

  };
};

ACE_END_VERSIONED_NAMESPACE_DECL
