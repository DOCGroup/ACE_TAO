// -*- C++ -*-
//
// $Id$

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTTP
  {
    ACE_INLINE
    void SessionBase::set_keep_alive (bool f)
      {
        this->keep_alive_ = f;
      }

    ACE_INLINE
    bool SessionBase::keep_alive () const
      {
        return this->keep_alive_;
      }

    ACE_INLINE
    void SessionBase::set_host (const ACE_CString& host, u_short port)
      {
        if (!this->is_connected ())
          {
            this->host_ = host;
            this->port_ = port;
            this->proxy_connection_ = false;
          }
      }

    ACE_INLINE
    void SessionBase::set_host (const ACE_CString& host)
      {
        if (!this->is_connected ())
          {
            this->host_ = host;
            this->proxy_connection_ = false;
          }
      }

    ACE_INLINE
    void SessionBase::set_port (u_short port)
      {
        if (!this->is_connected ())
          {
            this->port_ = port;
          }
      }

    ACE_INLINE
    void SessionBase::set_proxy_target (const ACE_CString& host, u_short port)
      {
        if (!this->is_connected ())
          {
            this->proxy_target_host_ = host;
            this->proxy_target_port_ = port;
            this->proxy_connection_ = true;
          }
      }

    ACE_INLINE
    const ACE_CString& SessionBase::get_host () const
      {
        return this->host_;
      }

    ACE_INLINE
    u_short SessionBase::get_port () const
      {
        return this->port_;
      }

    ACE_INLINE
    bool SessionBase::is_proxy_connection () const
      {
        return this->proxy_connection_;
      }

    ACE_INLINE
    const ACE_CString& SessionBase::get_proxy_target_host () const
      {
        return this->proxy_target_host_;
      }

    ACE_INLINE
    u_short SessionBase::get_proxy_target_port () const
      {
        return this->proxy_target_port_;
      }

    ACE_INLINE
    bool SessionBase::reconnect_needed ()
      {
        if (this->cannot_reconnect_)
          return false;
        if (!this->needs_reconnect_)
          {
            this->reconnect_countdown_.update ();
            return this->reconnect_timer_ == ACE_Time_Value::zero;
          }
        return true;
      }

    ACE_INLINE
    void SessionBase::close_streams ()
      {
        if (this->in_stream_)
          {
            delete this->in_stream_;
            this->in_stream_ = 0;
          }
        if (this->out_stream_)
          {
            delete this->out_stream_;
            this->out_stream_ = 0;
          }
      }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
