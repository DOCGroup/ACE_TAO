/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//     ace
// 
// = FILENAME
//     CLASSIX_Peer_SAP.cpp
//
// = AUTHOR(S)
//     Nokia Telecommunications
// 
// ============================================================================

#include "ace/CLASSIX/CLASSIX_Peer_SAP.h"

#if !defined (__ACE_INLINE__)
#include "ace/CLASSIX/CLASSIX_Peer_SAP.i"
#endif /* __ACE_INLINE__ */

#include "ace/CLASSIX/CLASSIX_Group_Stamp.h"
/* ------------------------------------------------------------------------- */
int
ACE_CLASSIX_Peer_SAP::set(const ACE_CLASSIX_Port_Core& thePeer)
{
    delete this->peer_addr_;
    this->peer_addr_ = new ACE_CLASSIX_Port (thePeer);
    ACE_ASSERT(this->peer_addr_);
    return 0;
}

int
ACE_CLASSIX_Peer_SAP::set(const ACE_Addr& thePeer) 
{
    delete this->peer_addr_;
    this->peer_addr_ = new ACE_CLASSIX_Port (thePeer);
    ACE_ASSERT(this->peer_addr_);
    return 0;
}

int
ACE_CLASSIX_Peer_SAP::set(const KnUniqueId& thePeer)
{
    delete this->peer_addr_;
    this->peer_addr_ = new ACE_CLASSIX_Port (thePeer);
    ACE_ASSERT(this->peer_addr_);
    return 0;
}

int
ACE_CLASSIX_Peer_SAP::set(const KnCap& thePeer)
{
    delete this->peer_addr_;
    this->peer_addr_ = new ACE_CLASSIX_Group (thePeer);
    ACE_ASSERT(this->peer_addr_);
    return 0;
}

int
ACE_CLASSIX_Peer_SAP::set(int theStamp)
{
    delete this->peer_addr_;
    this->peer_addr_ = new ACE_CLASSIX_Group_Stamp (theStamp);
    ACE_ASSERT(this->peer_addr_);
    return 0;
}

void
ACE_CLASSIX_Peer_SAP::dump (void) const
{
    ACE_DEBUG ((LM_DEBUG, "ACE_CLASSIX_Peer_SAP...\n"));
    if (this->peer_addr_)
	this->peer_addr_->dump();
    ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}
