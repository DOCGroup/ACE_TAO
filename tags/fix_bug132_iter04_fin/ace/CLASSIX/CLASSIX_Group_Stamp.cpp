/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//     ace
// 
// = FILENAME
//     ace/CLASSIX_Group_Stamp.cpp
//
// = AUTHOR(S)
//     Nokia Telecommunications
// 
// ============================================================================

#include "ace/CLASSIX/CLASSIX_Group_Stamp.h"

#if !defined (__ACE_INLINE__)
#include "ace/CLASSIX/CLASSIX_Group_Stamp.i"
#endif /* __ACE_INLINE__ */
/* ------------------------------------------------------------------------- */
void
ACE_CLASSIX_Group_Stamp::set_addr(void* theStamp, int)
{
    // Get the group capability
    this->stamp_ = *((u_int*)theStamp);
    KnActorPrivilege     actor;
    actorPrivilege(K_MYACTOR, &actor, NULL);
    if (::grpAllocate((actor == K_SUPACTOR) ?  K_STATSYS : K_STATUSER,
		      &this->addr_.group_, 
		      this->stamp_) < 0 )
    {
	ACE_DEBUG((LM_DEBUG, "ACE_CLASSIX_Group_Stamp()::"
		   "group allocation failed\n"));
	this->init_();
	this->stamp_ = ACE_CLASSIX_Group_Stamp::ILLEGAL_STAMP;
    }
    else
    {
	this->set_config_();
    }
}

void
ACE_CLASSIX_Group_Stamp::dump(void) const
{
  ACE_DEBUG ((LM_DEBUG, "ACE_CLASSIX_Group_Stamp...\n"));
  ACE_DEBUG ((LM_DEBUG, "\nStamp = %d\n", this->get_stamp()));
  this->ACE_CLASSIX_Group::dump();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));    
}
