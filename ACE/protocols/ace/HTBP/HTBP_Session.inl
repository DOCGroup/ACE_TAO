// -*- C++ -*-
//
// $Id$

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
void
ACE::HTBP::Session::reconnect () const
{
  if (!this->closed_ && this->proxy_addr_)
    {
      if (inbound_ &&
          inbound_->ace_stream().get_handle() == ACE_INVALID_HANDLE)
        reconnect_i (inbound_);
      if (outbound_ &&
          outbound_->ace_stream().get_handle() == ACE_INVALID_HANDLE)
        reconnect_i (outbound_);
    }
}

ACE_INLINE
void
ACE::HTBP::Session::inbound (ACE::HTBP::Channel *s)
{
  this->inbound_ = s;
}


ACE_INLINE
void
ACE::HTBP::Session::outbound (ACE::HTBP::Channel *s)
{
  this->outbound_ = s;
}

ACE_INLINE
ACE::HTBP::Channel *
ACE::HTBP::Session::inbound (void) const
{
  return this->inbound_;
}

ACE_INLINE
const ACE::HTBP::Session_Id_t&
ACE::HTBP::Session::session_id(void) const
{
  return this->session_id_;
}

// I'm wondering if this should be coupled with a map update...?
ACE_INLINE
void
ACE::HTBP::Session::session_id (ACE_UINT32 id)
{
  this->session_id_.id_ = id;
}

ACE_INLINE
const ACE_INET_Addr *
ACE::HTBP::Session::proxy_addr (void) const
{
  return this->proxy_addr_;
}

ACE_INLINE
const ACE::HTBP::Addr &
ACE::HTBP::Session::peer_addr (void) const
{
  return this->session_id_.peer_;
}

ACE_INLINE
const ACE::HTBP::Addr &
ACE::HTBP::Session::local_addr (void) const
{
  return this->session_id_.local_;
}

ACE_INLINE
void
ACE::HTBP::Session::proxy_addr (ACE_INET_Addr *pa, int destroy)
{
  if (this->destroy_proxy_addr_)
    delete this->proxy_addr_;
  this->proxy_addr_ = pa;
  this->destroy_proxy_addr_ = destroy;
}

ACE_INLINE
int
ACE::HTBP::Session::sock_flags (void) const
{
  return this->sock_flags_;
}

//---------------------------------------------------------------------------
ACE_INLINE
u_long
ACE::HTBP::Session_Id_t::hash () const
{
  return id_; // + local.hash() + peer.hash();
}

ACE_INLINE
bool
ACE::HTBP::Session_Id_t::operator ==(const ACE::HTBP::Session_Id_t &other) const
{
  return ((this->id_ == other.id_) &&
          (this->local_ == other.local_) &&
          (this->peer_ == other.peer_));
}

ACE_END_VERSIONED_NAMESPACE_DECL
