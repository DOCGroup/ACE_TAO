/* -*- C++ -*- */
// $Id$

ACE_INLINE
ACE_CLASSIX_Stream::~ACE_CLASSIX_Stream(void)
{
  this->close();
}

ACE_INLINE
int
ACE_CLASSIX_Stream::control(u_int theMode)
{
  return this->set_mode_(theMode); 
}

ACE_INLINE
int
ACE_CLASSIX_Stream::control(u_int theMode, int theSite)
{
  return this->set_mode_(theMode, theSite); 
}

ACE_INLINE
int
ACE_CLASSIX_Stream::set_mode_(u_int theMode)
{
  return 0;
}

ACE_INLINE
int
ACE_CLASSIX_Stream::set_mode_(u_int theMode, int theSite)
{
  return 0;
}

ACE_INLINE
ssize_t
ACE_CLASSIX_Stream::peek(ACE_Time_Value* theTimeout) const
{
  return this->ipcRecv(0, 0, MSG_PEEK, theTimeout);
}

ACE_INLINE
ssize_t
ACE_CLASSIX_Stream::send(const void *theBuf,  size_t theLen) const
{
  return this->send_n(theBuf, theLen);
}

ACE_INLINE
ssize_t
ACE_CLASSIX_Stream::send(const void *theBuf,size_t theLen, int theFlag)
{
  return this->send_n(theBuf, theLen, theFlag);
}

ACE_INLINE
ssize_t
ACE_CLASSIX_Stream::recv_n(void* theBuf, int theLen) const
{
  return this->recv_n(theBuf, theLen);
}

ACE_INLINE
ACE_HANDLE
ACE_CLASSIX_Stream::get_handle(void) const
{
  return this->local_sap_.get_handle();
}

ACE_INLINE
void
ACE_CLASSIX_Stream::set_handle(ACE_HANDLE theHandle)
{
  this->local_sap_.set_handle(theHandle);
}

ACE_INLINE
const ACE_CLASSIX_SAP& 
ACE_CLASSIX_Stream::local_sap(void) const
{
    return this->local_sap_;
}

ACE_INLINE
const ACE_CLASSIX_Peer_SAP& 
ACE_CLASSIX_Stream::peer_sap(void) const
{
    return this->peer_sap_;
}

ACE_INLINE
ACE_CLASSIX_SAP& 
ACE_CLASSIX_Stream::get_local_sap(void)
{
    return this->local_sap_;
}

ACE_INLINE
ACE_CLASSIX_Peer_SAP& 
ACE_CLASSIX_Stream::get_peer_sap(void)
{
    return this->peer_sap_;
}

ACE_INLINE
const ACE_CLASSIX_Port& 
ACE_CLASSIX_Stream::local_addr(void) const
{
    return this->local_sap_.get_addr();
}

ACE_INLINE
int
ACE_CLASSIX_Stream::selectable(int thePriority)
{
    return this->local_sap_.selectable(thePriority);
}

ACE_INLINE
int
ACE_CLASSIX_Stream::unselectable(void)
{
    return this->local_sap_.unselectable();
}

ACE_INLINE
int
ACE_CLASSIX_Stream::is_selectable(void) const
{
    return this->local_sap_.is_selectable();
}


ACE_INLINE
int
ACE_CLASSIX_Stream::is_writer_closed(void) const
{
    
    return (this->peer_sap_closed_ == 1);
}

ACE_INLINE
int
ACE_CLASSIX_Stream::close_reader(void)
{
    this->local_sap_closed_ = 1;
    return 0;
}


ACE_INLINE
int
ACE_CLASSIX_Stream::is_reader_closed(void) const
{
    
    return this->local_sap_closed_ == 1;
}


ACE_INLINE
int
ACE_CLASSIX_Stream::enable(int) const
{
    return 0;
}

ACE_INLINE
int
ACE_CLASSIX_Stream::disable(int) const
{
    return 0;
}

ACE_INLINE
int
ACE_CLASSIX_Stream::get_remote_addr(ACE_Addr&) const
{
    return this->peer_sap_closed_ == 0;
}
