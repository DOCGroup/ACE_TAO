/* -*- C++ -*- */
// $Id$

// Defines the ClassiX IPC address family address format.

#define ACE_BUILD_DLL
#include "ace/CLASSIX/CLASSIX_Port.h"

#if !defined (__ACE_INLINE__)
#include "ace/CLASSIX/CLASSIX_Port.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_CLASSIX_Port)


ACE_CLASSIX_Port::ACE_CLASSIX_Port(void* thePort, int theLen)
    : ACE_CLASSIX_Addr (sizeof (ACE_CLASSIX_Port_Core::Addr))
{
    if (thePort &&  theLen == this->get_size())
    {
	ACE_CLASSIX_Port_Core::Addr *port = 
	    (ACE_CLASSIX_Port_Core::Addr*) thePort;
	this->addr_.handle  = port->handle;
	this->addr_.id      = port->id;
    }
    else
    {
	this->clear();
    }
}

ACE_CLASSIX_Port::ACE_CLASSIX_Port(ACE_Addr const& theAddr)
    : ACE_CLASSIX_Addr (sizeof (ACE_CLASSIX_Port_Core::Addr))
{
    if (theAddr.get_size() == this->get_size() &&
	theAddr.get_type() == this->get_type())
	this->set_(theAddr);
    else
	this->clear();
}

void
ACE_CLASSIX_Port::clear(void)
{
    this->addr_.handle = ACE_CLASSIX_ADDR_UNKNOWN;
    this->addr_.id.uiHead = this->addr_.id.uiTail = 0;
}

int
ACE_CLASSIX_Port::set_(const ACE_Addr& theAddr)
{
    ACE_CLASSIX_Port_Core::Addr *src = 
	((ACE_CLASSIX_Port_Core::Addr*) theAddr.get_addr());
    this->addr_.id = src->id;
    this->addr_.handle = src->handle;
	
    return 0;
}

int
ACE_CLASSIX_Port::set(const KnUniqueId& thePort)
{
    int no = ::portLi((KnUniqueId*) &thePort);

    // update the port no only if the no is valid
    if (no < 0)
	// Assume this is a remote port
	no = ACE_INVALID_HANDLE;

    this->addr_.handle = no;
    this->addr_.id     = thePort;
    return 0;
}

int
ACE_CLASSIX_Port::set_handle(ACE_HANDLE thePort)
{
    if (thePort == ACE_INVALID_HANDLE && 
	::portUi(&this->addr_.id, thePort) < 0)
	return -1;

    this->addr_.handle = thePort;
    return 0;
}

void
ACE_CLASSIX_Port::set_addr(void* theSrc, int theLen)
{
    // Remove self owned first and Make sure no memory overflow 
    if (theSrc && theLen > this->get_size())
	return;

    ACE_OS::memcpy ((void *) &this->addr_,
		    (void *) theSrc, theLen);

}

ACE_CLASSIX_Port const&  
ACE_CLASSIX_Port::operator =(ACE_Addr const& theAddr)
{
    if (theAddr.get_size() == this->get_size() &&
	theAddr.get_type() == this->get_type())
    {
	
	this->set_(theAddr);
    }
    return *this;
}

int 
ACE_CLASSIX_Port::addr_to_string (ACE_TCHAR s[], size_t) const
{
  ACE_OS::sprintf (s, ACE_TEXT ("%d:%d"),
                   ACE_TEXT_CHAR_TO_TCHAR (::agetId()),
                   this->get_port_number ());
}

void
ACE_CLASSIX_Port::dump (void) const
{
  ACE_DEBUG ((LM_DEBUG, "ACE_CLASSIX_Port...\n"));
  ACE_Addr::dump();
  char s[100];
  ACE_OS::sprintf (s, "handle = %d;\tid head = %x, id tail = %x\n",
		   this->get_handle(),
		   this->get_id().uiHead, this->get_id().uiTail);
  ACE_DEBUG ((LM_DEBUG, "%s", s));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}
