// SOCK_Stream.cpp
// $Id$

#include "ace/Log_Msg.h"

#include "HTBP_Session.h"
#include "ace/SOCK_Connector.h"
#include "ace/Event_Handler.h"
#include "HTBP_Filter.h"
#include "HTBP_ID_Requestor.h"

#if !defined (__ACE_INLINE__)
#include "HTBP_Session.inl"
#endif

ACE_RCSID(HTBP,HTBP_Session,"$Id$")

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE::HTBP::Session::Session_Map ACE::HTBP::Session::session_map_;
ACE_UINT32 ACE::HTBP::Session::last_session_id_ = 0;
ACE_SYNCH_MUTEX ACE::HTBP::Session::session_id_lock_;

/// Static method definitions
ACE_UINT32
ACE::HTBP::Session::next_session_id ()
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, g, ACE::HTBP::Session::session_id_lock_, 0);
  return ++last_session_id_;
}

int
ACE::HTBP::Session::add_session (ACE::HTBP::Session *s)
{
  return session_map_.bind (s->session_id(),s);
}

int
ACE::HTBP::Session::remove_session (ACE::HTBP::Session *s)
{
  if (session_map_.current_size() > 0)
    return session_map_.unbind(s->session_id());
  return 0;
}

int
ACE::HTBP::Session::find_session (const ACE::HTBP::Session_Id_t &sid, ACE::HTBP::Session *&out)
{
  ACE::HTBP::Session::Map_Entry *e = 0;
  if (session_map_.find (sid,e) == -1)
    {
      out = 0;
      return -1;
    }
  out = e->int_id_;
  return 0;
}

//----------------------------------------------------------------------------
ACE::HTBP::Session::Session (void)
  : proxy_addr_ (0),
    destroy_proxy_addr_ (0),
    inbound_ (0),
    outbound_ (0),
    closed_ (false),
    handler_ (0),
    reactor_(0),
    stream_ (0),
    sock_flags_(0)
{
  ACE::HTBP::ID_Requestor req;
  ACE_TCHAR * htid = req.get_HTID();
  ACE_Auto_Array_Ptr<ACE_TCHAR> guard (htid);
  session_id_.local_ = ACE_TEXT_ALWAYS_CHAR(htid);
  session_id_.id_ = ACE::HTBP::Session::next_session_id();
  ACE_NEW (inbound_, ACE::HTBP::Channel (this));
  ACE_NEW (outbound_, ACE::HTBP::Channel (this));
}

ACE::HTBP::Session::Session (const ACE::HTBP::Addr &peer,
                             const ACE::HTBP::Addr &local,
                             ACE_UINT32 sid,
                             ACE_INET_Addr *proxy,
                             bool take_proxy)
  : proxy_addr_ (proxy),
    destroy_proxy_addr_ (take_proxy),
    inbound_ (0),
    outbound_ (0),
    closed_ (false),
    handler_ (0),
    reactor_(0),
    stream_ (0),
    sock_flags_(0)
{
  session_id_.peer_ = peer;
  session_id_.local_ = local;
  session_id_.id_ = (sid == 0) ?
    ACE::HTBP::Session::next_session_id() : sid;

  ACE_NEW (inbound_,ACE::HTBP::Channel (this));
  ACE_NEW (outbound_,ACE::HTBP::Channel (this));
}

ACE::HTBP::Session::Session (const ACE::HTBP::Session_Id_t &id,
                             ACE_INET_Addr *proxy,
                             bool take_proxy)
  : proxy_addr_ (proxy),
    destroy_proxy_addr_ (take_proxy),
    session_id_(id),
    inbound_ (0),
    outbound_ (0),
    closed_ (false),
    handler_ (0),
    reactor_ (0),
    stream_ (0),
    sock_flags_(0)
{
  ACE_NEW (inbound_, ACE::HTBP::Channel (this));
  ACE_NEW (outbound_, ACE::HTBP::Channel (this));
}

ACE::HTBP::Session::Session (const ACE::HTBP::Session &other)
{
  this->operator=(other);
}

ACE::HTBP::Session&
ACE::HTBP::Session::operator= (const ACE::HTBP::Session &)
{
  // @TODO: figure out why the assignment operator is here if it is
  // unimplemented? Previously there was an ACE_ASSERT(this == 0)
  // so apparently something bad had been happening long ago, but I
  // have no idea what.
  return *this;
}

ACE::HTBP::Session::~Session (void)
{
  if (destroy_proxy_addr_)
    delete proxy_addr_;

  delete this->inbound_;
  delete this->outbound_;
}

int
ACE::HTBP::Session::close (void)
{
  if (this->inbound_)
    this->inbound_->close();
  if (this->outbound_)
    this->outbound_->close();
  this->closed_ = true;
  return ACE::HTBP::Session::remove_session (this);
}


ACE::HTBP::Channel *
ACE::HTBP::Session::outbound (void) const
{
  if (!this->closed_ && this->proxy_addr_)
    this->reconnect();
  if ( this->outbound_ == 0)
    return 0;
  ACE::HTBP::Channel::State s =this->outbound_->state();
  return s == ACE::HTBP::Channel::Init || s == ACE::HTBP::Channel::Ready ? this->outbound_ : 0;
}

void
ACE::HTBP::Session::reconnect_i (ACE::HTBP::Channel *s) const
{
  ACE_SOCK_Connector conn;
  if (conn.connect (s->ace_stream(),*this->proxy_addr_) == -1)
    {
      ACE_TCHAR buffer[128];
      this->proxy_addr_->addr_to_string(buffer, 128, 0);
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT("(%P|%t) ACE::HTBP::Session::reconnect")
                  ACE_TEXT(" failed to %s, %p\n"),
                  buffer, s == this->inbound_ ?
                  ACE_TEXT("inbound") : ACE_TEXT ("outbound")));
    }
  else
    {
#if !defined (ACE_LACKS_TCP_NODELAY)
      int no_delay = 1;
      int result = s->ace_stream().set_option (ACE_IPPROTO_TCP,
                                               TCP_NODELAY,
                                               (void *) &no_delay,
                                               sizeof (no_delay));
      if (result == -1)
        ACE_DEBUG ((LM_DEBUG, "HTBP::Session::reconnect_i, %p\n", "set_option" ));
#endif /* ! ACE_LACKS_TCP_NODELAY */
      
    }
  s->register_notifier(this->reactor_);
  if (s == this->inbound_)
    s->send_ack();
}

ACE_Event_Handler *
ACE::HTBP::Session::handler (void)
{
  return this->handler_;
}

void
ACE::HTBP::Session::handler (ACE_Event_Handler * h)
{
  this->handler_ = h;
}

void
ACE::HTBP::Session::detach (ACE::HTBP::Channel *ch)
{
  if (this->inbound_ == ch)
    this->inbound_ = 0;
  else if (this->outbound_ == ch)
    this->outbound_ = 0;
  else
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT("ACE::HTBP::Session::detach ")
                ACE_TEXT("called with unknown channel\n")));
}

void
ACE::HTBP::Session::reactor (ACE_Reactor *r)
{
  this->reactor_ = r;
  this->inbound_->register_notifier(r);
  this->outbound_->register_notifier(r);
}

int
ACE::HTBP::Session::enqueue (ACE_Message_Block *msg)
{
  this->outbound_queue_.enqueue_tail(msg);
  return msg->length();
}

int
ACE::HTBP::Session::flush_outbound_queue (void)
{
  int result = 0;
  if (this->outbound_queue_.message_count() > 0)
    {
      ACE_Message_Block *msg = 0;
      iovec *iov = 0;
      ACE_NEW_RETURN (iov,
                      iovec[this->outbound_queue_.message_count()],
                      -1);
      ACE_Auto_Array_Ptr<iovec> guard (iov);
      this->outbound_queue_.peek_dequeue_head (msg);
      for (size_t i = 0; i < this->outbound_queue_.message_count(); i++)
        {
          iov[i].iov_base = msg->rd_ptr();
          iov[i].iov_len = msg->length();
          msg = msg->next();
        }
      if (this->outbound_->state() ==  ACE::HTBP::Channel::Wait_For_Ack)
        this->outbound_->recv_ack();
      result = this->outbound_->sendv (iov,this->outbound_queue_.message_count(),0);
      while (this->outbound_queue_.message_count ())
        {
          this->outbound_queue_.dequeue_head (msg);
          msg->release ();
        }
    }
  return result;
}

int
ACE::HTBP::Session::close_inbound (void) const
{
  return this->inbound_ ? this->inbound_->close() : 0;
}

int
ACE::HTBP::Session::close_outbound (void) const
{
  return this->outbound_ ? this->outbound_->close() : 0;
}

int
ACE::HTBP::Session::enable (int flags)
{
  this->sock_flags_ |= flags;
  int result = this->inbound_ ? this->inbound_->enable(flags) : 0;
  result |= this->outbound_ ? this->outbound_->enable (flags) : 0;
  return result;
}

int
ACE::HTBP::Session::disable (int flags)
{
  this->sock_flags_ &= ~flags;
  int result = this->inbound_ ? this->inbound_->disable(flags) : 0;
  result |= this->outbound_ ? this->outbound_->disable (flags) : 0;
  return result;
}

ACE::HTBP::Stream *
ACE::HTBP::Session::stream (void)const
{
  return this->stream_;
}

void
ACE::HTBP::Session::stream (ACE::HTBP::Stream *s)
{
  this->stream_ = s;
}

ACE_END_VERSIONED_NAMESPACE_DECL
