/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//     IPP
// 
// = FILENAME
//     CLASSIX/Dgram_Mcast.cpp
//
// = AUTHOR(S)
//     Wei Chiang
// 
// = COPYRIGHT
//     Copyright 1998 Nokia Telecommunications
//
// ============================================================================

#include "ace/CLASSIX/CLASSIX_Dgram_Mcast.h"

#if !defined (__ACE_INLINE__)
#include "ace/CLASSIX/CLASSIX_Dgram_Mcast.i"
#endif /* __ACE_INLINE__ */
/* ------------------------------------------------------------------------- */
int
ACE_CLASSIX_Dgram_Mcast::set_saps(int theStamp, 
				    const ACE_CLASSIX_Port& thePort)
{
    this->local_sap_.set_addr(thePort);
    if (this->peer_sap_.set(theStamp) != 0)
    {
	errno = EADDRNOTAVAIL;
	ACE_ERROR_RETURN((LM_ERROR, "ACE_CLASSIX_Dgram_Mcast- "
		   "failed to set up peer sap address \n"), -1);
    }
    else
    {
	this->dest_.target = this->peer_sap_.get_addr()->get_id();
	return 0;
    }
}


int
ACE_CLASSIX_Dgram_Mcast::set_mode_(u_int theMode, int theSite)
{
    int result = K_EFAULT;
    if (theMode <= K_FUNCXMODE  &&
	(result =::uiSite(&this->dest_.coTarget, theSite)) == 0)
	result = ::ipcTarget(&this->dest_.target, theMode);
    
    if (result != 0)
    {
	errno = ACE_CLASSIX_OS::convert_io_error(result);
	result = -1;
    }
    return result;
}

int
ACE_CLASSIX_Dgram_Mcast::set_mode_(u_int theMode)
{
    int result = K_EFAULT;
    if (theMode <= K_FUNCXMODE )
	result = ::ipcTarget(&this->dest_.target, theMode);

    if (result < 0)
    {
	errno = ACE_CLASSIX_OS::convert_io_error(result);
	result = -1;
    }
    return result;
}


void
ACE_CLASSIX_Dgram_Mcast::dump (void) const
{
    ACE_DEBUG ((LM_DEBUG, "ACE_CLASSIX_Dgram_Mcast...\n"));
    this->ACE_CLASSIX_Stream::dump();
    ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}
