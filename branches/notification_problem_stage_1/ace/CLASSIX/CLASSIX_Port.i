/* -*- C++ -*- */
// $Id$

ACE_INLINE
ACE_CLASSIX_Port::ACE_CLASSIX_Port(void)
    : ACE_CLASSIX_Addr (sizeof (ACE_CLASSIX_Port_Core::Addr))
{
    this->set_handle(K_DEFAULTPORT);
}

ACE_INLINE
ACE_CLASSIX_Port::ACE_CLASSIX_Port(const ACE_CLASSIX_Port& theSrc)
    : ACE_CLASSIX_Addr (sizeof (ACE_CLASSIX_Port_Core::Addr))
{
    this->set_(theSrc);
}

ACE_INLINE
ACE_CLASSIX_Port::ACE_CLASSIX_Port(const KnUniqueId& thePort)
    : ACE_CLASSIX_Addr (sizeof (ACE_CLASSIX_Port_Core::Addr))
{
    this->set(thePort);
}

ACE_INLINE
ACE_CLASSIX_Port::ACE_CLASSIX_Port(ACE_HANDLE thePort)
    : ACE_CLASSIX_Addr (sizeof (ACE_CLASSIX_Port_Core::Addr))
{
    this->set_handle(thePort);
}

ACE_INLINE
ACE_CLASSIX_Port::ACE_CLASSIX_Port(const ACE_CLASSIX_Port_Core& thePort)
    : ACE_CLASSIX_Addr (sizeof (ACE_CLASSIX_Port_Core::Addr))
{
	this->addr_.handle = thePort.get_handle();
	this->addr_.id     = thePort.get_id();
}

ACE_INLINE
ACE_HANDLE
ACE_CLASSIX_Port::get_handle(void) const
{
    return this->addr_.handle;
}

ACE_INLINE
const KnUniqueId&
ACE_CLASSIX_Port::get_id(void) const
{
    return this->addr_.id;
}

ACE_INLINE
void *
ACE_CLASSIX_Port::get_addr(void) const
{
    return (void*) &this->addr_;
}

ACE_INLINE
int
ACE_CLASSIX_Port::is_configured(void) const
{
    return (this->addr_.handle != ACE_CLASSIX_ADDR_UNKNOWN);
}

ACE_INLINE
ACE_CLASSIX_Addr::Addr_Type
ACE_CLASSIX_Port::is_type(void) const
{
    return ACE_CLASSIX_Addr::PORT;
}

ACE_INLINE
int
ACE_CLASSIX_Port::enable(int thePriority) const
{
    if (ACE_Reactor::instance()->notify() == 0)
	return ::portEnable(K_MYACTOR, this->get_handle(), thePriority);
    else
	return -1;
}

ACE_INLINE
int
ACE_CLASSIX_Port::disable(void) const
{
    if (ACE_Reactor::instance()->notify() == 0)
	return ::portDisable(K_MYACTOR, this->get_handle());
    else
	return -1;
}

// Compare two addresses for equality.
ACE_INLINE
int
ACE_CLASSIX_Port::operator==(const ACE_CLASSIX_Port &theSrc) const
{
  return 
      this->ACE_Addr::operator == (theSrc) &&
      this->addr_.id.uiHead == theSrc.addr_.id.uiHead &&
      this->addr_.id.uiTail == theSrc.addr_.id.uiTail &&
      this->addr_.handle == theSrc.addr_.handle;
}

ACE_INLINE
int
ACE_CLASSIX_Port::operator != (const ACE_CLASSIX_Port &sap) const
{
  return !((*this) == sap);
}
