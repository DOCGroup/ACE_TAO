/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//     ace
// 
// = FILENAME
//     CLASSIX_Group.cpp
//
// = AUTHOR(S)
//     Nokia Telecommunications
// 
// ============================================================================

#include "ace/CLASSIX/CLASSIX_Group.h"

#if !defined (__ACE_INLINE__)
#include "ace/CLASSIX/CLASSIX_Group.i"
#endif /* __ACE_INLINE__ */
/* ------------------------------------------------------------------------- */

void
ACE_CLASSIX_Group::init_(void)
{
    this->clear_config_();
    this->addr_.group_.ui.uiHead = this->addr_.group_.ui.uiTail = 0;
    this->addr_.group_.key.keyHead = this->addr_.group_.key.keyTail = 0;
}

void
ACE_CLASSIX_Group::set_addr(void* theSrc, int theLen)
{
    if (theLen > this->get_size())
        return;

    ACE_OS::memcpy ((void *) &this->addr_,
                    (void *) theSrc, theLen);
}

void
ACE_CLASSIX_Group::set_addr(const KnCap& thePeer)
{
    this->addr_.group_ = thePeer;
    this->set_config_();
}


int
ACE_CLASSIX_Group::insert(const ACE_CLASSIX_Port* thePort)
{
    if (this->is_configured())
	return ::grpPortInsert((KnCap*) &this->get_cap(), 
			       (KnUniqueId*) &thePort->get_id());
    else
	return K_EINVAL;
}

int
ACE_CLASSIX_Group::remove(const ACE_CLASSIX_Port* thePort)
{
    if (this->is_configured())
	return ::grpPortRemove((KnCap*) &this->get_cap(), 
			       (KnUniqueId*) &thePort->get_id());
    else
	return K_EINVAL;
}

void
ACE_CLASSIX_Group::dump(void) const
{
  ACE_DEBUG ((LM_DEBUG, "ACE_CLASSIX_Group...\n"));
  char s[200];
  if (!this->is_configured())
      ACE_OS::sprintf(s, "Group has not been setup\n");
  else
  {
      const KnCap &group = this->get_cap();
      ACE_OS::sprintf (s, "id head = %d(%x), id tail = %d(%x)\n"
		       "key head = %d(%x) key tail = %d(%x)\n",
		       group.ui.uiHead, group.ui.uiHead, 
		       group.ui.uiTail, group.ui.uiTail, 
		       group.key.keyHead, group.key.keyHead, 
		       group.key.keyTail, group.key.keyTail);
  }
  ACE_DEBUG ((LM_DEBUG, "\n%s", s));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));    
}
