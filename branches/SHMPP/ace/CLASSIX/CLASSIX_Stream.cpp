/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//     ace
// 
// = FILENAME
//     CLASSIX_Stream.cpp
//
// = AUTHOR(S)
//     Nokia Telecommunications
// 
// ============================================================================

#include "ace/CLASSIX/CLASSIX_Stream.h"

#if !defined (__ACE_INLINE__)
#include "ace/CLASSIX/CLASSIX_Stream.i"
#endif /* __ACE_INLINE__ */
/* ------------------------------------------------------------------------- */
ACE_CLASSIX_Stream::ACE_CLASSIX_Stream(void)
    : local_sap_        (),
      local_sap_closed_ (0),
      local_port_       (0),
      peer_sap_         (),
      peer_sap_closed_  (1)
//      dest_site_        (-1)
{
}

ACE_CLASSIX_Stream::ACE_CLASSIX_Stream
         (const ACE_CLASSIX_Port_Core& thePeer, 
	  const ACE_CLASSIX_Port& theLocal)
    : local_sap_        (theLocal),
      local_sap_closed_ (0),
      local_port_       (0),
      peer_sap_         (thePeer),
      peer_sap_closed_  (1)
//      dest_site_        (-1)
{
    this->dest_.target = this->peer_sap_.get_addr()->get_id();
}

ACE_CLASSIX_Stream::ACE_CLASSIX_Stream(const ACE_CLASSIX_Port& thePeer, 
				       const ACE_CLASSIX_Port& theLocal)
    : local_sap_       (theLocal),
      local_sap_closed_(0),
      local_port_      (0),
      peer_sap_        (thePeer),
      peer_sap_closed_ (1)
//      dest_site_       (-1)
{
    this->dest_.target = this->peer_sap_.get_addr()->get_id();
}


ACE_CLASSIX_Stream::ACE_CLASSIX_Stream(const KnUniqueId& thePeer,
			       const ACE_CLASSIX_Port& theLocal)
    : local_sap_       (theLocal),
      local_sap_closed_(0),
      local_port_      (0),
      peer_sap_        (thePeer),
      peer_sap_closed_ (1)
//      dest_site_       (-1)
{
    this->dest_.target = this->peer_sap_.get_addr()->get_id();
}

int
ACE_CLASSIX_Stream::set_local_port(ACE_CLASSIX_Port_Core* thePort)
{
    delete this->local_port_;
    if (this->local_sap_.open(thePort) == 0)
    {
	this->local_port_ = thePort;
	return 0;
    }
    else
    {
	this->local_port_ = 0;
	this->local_sap_.clear();
	return -1;
    }
}

int 
ACE_CLASSIX_Stream::open(void)
{
    this->dest_.target = this->peer_sap_.get_addr()->get_id();
    this->open_writer();

    if (!this->local_sap().is_configured() ||
	this->local_sap().get_handle() == 
	ACE_CLASSIX_DEFAULT_PORT::instance()->get_handle())
    {
	delete this->local_port_;
	ACE_NEW_RETURN(this->local_port_, ACE_CLASSIX_Port_Core(), -1);
	return this->local_sap_.open(this->local_port_);
    }

    return 0;
}

int 
ACE_CLASSIX_Stream::open(const ACE_Addr& theAddr)
{
    this->dest_.target = this->peer_sap_.get_addr()->get_id();
    this->open_writer();

    if (!this->local_sap().is_configured() ||
	this->local_sap().get_handle() == 
	ACE_CLASSIX_DEFAULT_PORT::instance()->get_handle())
    {
	delete this->local_port_;
	if (theAddr.get_size() != this->local_sap().get_addr().get_size())
	    return -1;
	return this->local_sap_.open(theAddr);
    }
    return 0;	    
}

int
ACE_CLASSIX_Stream::close_writer(void)
{
    if (this->peer_sap_closed_ == 0)
    {
	if (this->send_n(0, 0) < 0)
	    ACE_DEBUG((LM_DEBUG, "failed to send a zero byte msg\n"));
	this->peer_sap_closed_ = 1;
    }
    return 0;
}

void
ACE_CLASSIX_Stream::open_writer(void)
{
    this->peer_sap_closed_ = 0;
}

int
ACE_CLASSIX_Stream::close(void)
{
    this->close_writer();

    this->local_sap_.close();
    if (this->local_port_)
    {
	delete this->local_port_;
	this->local_port_ = 0;
    }
    this->dest_.target = ACE_CLASSIX_OS::null_KnUniqueId();
    return 0;
}

void
ACE_CLASSIX_Stream::set_target_()
{
    this->dest_.target = this->peer_sap_.get_addr()->get_id();
}

int
ACE_CLASSIX_Stream::set_peer_sap(const ACE_Addr& theAddr)
{
    if (this->peer_sap_.set(theAddr) == 0)
    {
	this->set_target_();
	return 0;
    }
    return -1;
}

int
ACE_CLASSIX_Stream::set_peer_sap(const ACE_CLASSIX_Port_Core& theAddr)
{
    if (this->peer_sap_.set(theAddr) == 0)
    {
	this->set_target_();
	return 0;
    }
    return -1;
}

int
ACE_CLASSIX_Stream::set_peer_sap(const KnUniqueId& theAddr)
{
    if (this->peer_sap_.set(theAddr) == 0)
    {
	this->set_target_();
	return 0;
    }
    return -1;
}

int
ACE_CLASSIX_Stream::set_peer_sap(const KnCap& theAddr)
{
    if (this->peer_sap_.set(theAddr) == 0)
    {
	this->set_target_();
	return 0;
    }
    return -1;
}

ssize_t
ACE_CLASSIX_Stream::send_n(const void* theBuf, size_t theLen, int theFlags)
{
    if (!this->is_writer_closed())
    {
	this->set_mode_(theFlags);
	return this->send_n(theBuf, theLen);
    }
    else
	return 0;
}

ssize_t
ACE_CLASSIX_Stream::send_n(const void* theBuf, size_t theLen) const
{
    if (!this->is_writer_closed())
    {
	ACE_CLASSIX_Msg msg(theBuf, theLen);
	int result = 0;
	if ((result = ::ipcSend((KnMsgDesc*) msg.get(), 
				this->local_sap().get_handle(), 
				(KnIpcDest*) &this->dest_)) == 0)
	    return theLen;
	else
	{
	    errno = ACE_CLASSIX_OS::convert_io_error(result);
	    return -1;
	}
    }
    else
    {
	return 0;
    }
}

ssize_t
ACE_CLASSIX_Stream::reply(const void* theBuf, size_t theLen) const
{
    if (!this->is_writer_closed())
    {
	ACE_CLASSIX_Msg msg(theBuf, theLen);
	if (int result = ::ipcReply((KnMsgDesc*) msg.get()) == 0)
	    return theLen;
	else
	{
	    errno = ACE_CLASSIX_OS::convert_io_error(result);
	    return -1;
	}
    }
    else
	return 0;
}

ssize_t
ACE_CLASSIX_Stream::ipcRecv(void* theBuf, size_t theLen, 
			 int theFlag, ACE_Time_Value* theTimeout) const
{
    if (!this->is_reader_closed())
    {
	// In chorus environment, a negative value means block
	// until recv is possible 
	int timeout = theTimeout == 0 ? -1 : theTimeout->msec();
	if (timeout == 0)
	    timeout = -1;

	if (theFlag & MSG_PEEK)
	{
	    theBuf = 0;
	    theLen = 0;
	}

	ACE_CLASSIX_Msg rmsg(theBuf, theLen);
	int handle = this->local_sap().get_handle();
	int result = ::ipcReceive((KnMsgDesc*) rmsg.get(), &handle,  
				  timeout);
	if (result < 0)
	{
	    errno = ACE_CLASSIX_OS::convert_io_error(result);
	    return -1;
	}
	else
	    return result;
    }
    else
    {
	errno = ENOTCONN;
	return -1;
    }
}

ssize_t
ACE_CLASSIX_Stream::ipcRecv_n(void* theBuf, size_t theLen, 
			   int theFlag, ACE_Time_Value* theTimeout) const
{
    // Timeout value applies to each around of receive.
    // ....Should be adjusted after each round!
    int len = 0;
    void *next = theBuf;
    size_t more = theLen; 
    for (int n = 0; 
	 more > 0; 
	 len += n, next += n, more -=n)
    {
	if ((n = this->ipcRecv(next, more, theFlag, theTimeout)) == -1)
	    break;
    }
    return len;
}

ssize_t
ACE_CLASSIX_Stream::recv(void* theBuf, int theLen) const
{
    if (!this->is_reader_closed())
    {
	ACE_CLASSIX_Msg rmsg(theBuf, theLen);
	int result =  ::ipcGetData(rmsg.get());
	if (result >= 0)
	    return result;
	else if (result == K_EINVAL)
	    errno = EWOULDBLOCK;
	else 
	    errno = EFAULT;

	return -1;
    }
    else
	return 0;
}



void
ACE_CLASSIX_Stream::dump (void) const
{
    ACE_DEBUG ((LM_DEBUG, "ACE_CLASSIX_Stream...\n"));
    ACE_DEBUG ((LM_DEBUG, "...local sap(%s)\n", 
	       this->is_writer_closed() == 1 ? "closed" : "opened"));
    if (this->local_port_)
	ACE_DEBUG ((LM_DEBUG, "\towns the local port(%x)\n", 
		    this->local_port_));
    this->local_sap().dump();

    ACE_DEBUG ((LM_DEBUG, "...Peer sap(%s & %s)\n",
	       this->is_reader_closed() == 1 ? "closed" : "opened",
	       this->is_selectable() ? "enabled" : "disabled"));
    this->peer_sap().dump();
    ACE_DEBUG((LM_DEBUG, "Dest- Target\n head = %x tail = %x\n",
	       this->dest_.target.uiHead,
	       this->dest_.target.uiTail
	       /* this->dest_site_ */));
    ACE_DEBUG((LM_DEBUG, "Dest- coTarget\n head = %x tail = %x\n",
	       this->dest_.coTarget.uiHead,
	       this->dest_.coTarget.uiTail
	       /* this->dest_site_ */));
    ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}
