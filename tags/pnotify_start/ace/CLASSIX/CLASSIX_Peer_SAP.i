/* -*- C++ -*- */
// $Id$

ACE_INLINE
ACE_CLASSIX_Peer_SAP::ACE_CLASSIX_Peer_SAP(void)
  :    peer_addr_   (0)
{
}

ACE_INLINE
ACE_CLASSIX_Peer_SAP::ACE_CLASSIX_Peer_SAP(const ACE_CLASSIX_Port_Core& 
					   thePeer)
  :    peer_addr_   (0)
{
    this->set(thePeer);
}

ACE_INLINE
ACE_CLASSIX_Peer_SAP::ACE_CLASSIX_Peer_SAP(const ACE_CLASSIX_Port& thePeer) 
    : peer_addr_   (0)
{
    this->set(thePeer);
}

ACE_INLINE
ACE_CLASSIX_Peer_SAP::ACE_CLASSIX_Peer_SAP(const ACE_Addr& thePeer) 
    : peer_addr_   (0)
{
    this->set(thePeer);
}

ACE_INLINE
ACE_CLASSIX_Peer_SAP::ACE_CLASSIX_Peer_SAP(const KnUniqueId& thePeer)
    : peer_addr_   (0)
{
    this->set(thePeer);
}

ACE_INLINE
ACE_CLASSIX_Peer_SAP::ACE_CLASSIX_Peer_SAP(int theStamp)
    : peer_addr_   (0)
{
    this->set(theStamp);
}

ACE_INLINE
ACE_CLASSIX_Peer_SAP::~ACE_CLASSIX_Peer_SAP(void)
{
    delete this->peer_addr_;
}


ACE_INLINE
const ACE_CLASSIX_Addr*
ACE_CLASSIX_Peer_SAP::get_addr(void) const
{
    return this->peer_addr_;
}

ACE_INLINE
int
ACE_CLASSIX_Peer_SAP::is_configured(void) const
{
    if (this->peer_addr_)
	return this->peer_addr_->is_configured();
    else
	return 0;
}

#if 0
ACE_INLINE
int
ACE_CLASSIX_Peer_SAP::control(u_int theMode)
{
    return this->peer_addr_->set_mode_(theMode); 
}

ACE_INLINE
int
ACE_CLASSIX_Peer_SAP::control(u_int theMode, int theSite)
{
    return this->set_mode_(theMode, theSite); 
}


ACE_INLINE
int
ACE_CLASSIX_Peer_SAP::set_mode_(u_int)
{
    return K_EFAULT;
}

ACE_INLINE
int
ACE_CLASSIX_Peer_SAP::set_mode_(u_int, int)
{
    return K_EFAULT;
}
#endif
