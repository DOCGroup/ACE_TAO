/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//     ace
// 
// = FILENAME
//     CLASSIX_SAP.cpp
//
// = AUTHOR(S)
//     Nokia Telecommunications
// 
// ============================================================================

#include "ace/CLASSIX/CLASSIX_SAP.h"

#if !defined (__ACE_INLINE__)
#include "ace/CLASSIX/CLASSIX_SAP.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_CLASSIX_SAP)
/* ------------------------------------------------------------------------- */
int
ACE_CLASSIX_SAP::get_addr(ACE_Addr& theAddr) const
{
    if (theAddr.get_size() < this->local_addr_.get_size() || 
	theAddr.get_type() != this->local_addr_.get_type())
    {
	return -1;
    }
    else
    {
	theAddr.set_addr(this->local_addr_.get_addr(), 
			 this->local_addr_.get_size());
	return 0;
    }
}

void
ACE_CLASSIX_SAP::set_addr(const ACE_Addr& theAddr)
{
    this->local_addr_.set_addr(theAddr.get_addr(), theAddr.get_size());
}


void
ACE_CLASSIX_SAP::set_addr(const ACE_CLASSIX_Port_Core& thePort)
{
    this->local_addr_.set_addr(thePort.get_addr(), 
			       sizeof (ACE_CLASSIX_Port_Core::Addr));
}

int
ACE_CLASSIX_SAP::set(const KnUniqueId& theId)
{
    return this->local_addr_.set(theId);
}

void
ACE_CLASSIX_SAP::set_handle(ACE_HANDLE theHandle)
{
    if (this->local_addr_.set_handle(theHandle) == -1)
	ACE_DEBUG((LM_DEBUG, "ACE_CLASSIX_SAP::set_handle()::"
		   "Handle invalid\n"));
}

int
ACE_CLASSIX_SAP::set(const ACE_Addr& theAddr)
{
    this->set_addr(theAddr);
    return 0;
}

int
ACE_CLASSIX_SAP::open(const ACE_Addr& theAddr)
{
    this->set_addr(theAddr);
    return 0;
}


int
ACE_CLASSIX_SAP::set(const ACE_CLASSIX_Port_Core& thePort)
{
    this->set_addr(thePort);
    return 0;
}


int
ACE_CLASSIX_SAP::open(const ACE_CLASSIX_Port_Core* thePort)
{
    if (thePort)
    {
	this->set_addr(ACE_CLASSIX_Port(*thePort));
	return 0;
    }
    else
	return -1;
}

int
ACE_CLASSIX_SAP::close(void)
{    
    // Disable receiving
    this->unselectable();
    //*** Do something to flush out potential messages on the local port
    // e.g. have a separte thread call ipcReceive on each disabled SAP.

    this->local_addr_.clear();
    return 0;
}

int
ACE_CLASSIX_SAP::selectable(int thePriority)
{
    if (!this->enabled_)
    {
	int result = this->local_addr_.enable(thePriority);
	if (result == 0) 
	    this->enabled_ = 1;
	return result;
    }
    else
	return 0;
}

int
ACE_CLASSIX_SAP::unselectable(int)
{
    if (this->enabled_)
    {
	int result = this->local_addr_.disable();
	if (result == 0)
	    this->enabled_ = 0;
	return result;
    }
    else
	return 0;
}


void
ACE_CLASSIX_SAP::dump(void) const
{
    ACE_DEBUG ((LM_INFO, "ACE_CLASSIX_SAP...\n"));

    this->local_addr_.dump();

    ACE_DEBUG ((LM_INFO, ACE_END_DUMP));
}
