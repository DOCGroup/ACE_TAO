/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//     ace
// 
// = FILENAME
//     CLASSIX_Port_Core.cpp
//
// = AUTHOR(S)
//     Nokia Telecommunications
// 
// ============================================================================

#include "ace/CLASSIX/CLASSIX_Port_Core.h"

#if !defined (__ACE_INLINE__)
#include "ace/CLASSIX/CLASSIX_Port_Core.i"
#endif /* __ACE_INLINE__ */

#include "ace/Synch_T.h"
#include "ace/CLASSIX/CLASSIX_OS.h"
/* ------------------------------------------------------------------------- */

ACE_ALLOC_HOOK_DEFINE(ACE_CLASSIX_Port_Core)

ACE_CLASSIX_Port_Core::ACE_CLASSIX_Port_Core(void)
{
    if ((this->addr_.handle = ::portCreate(K_MYACTOR, &this->addr_.id)) < 0)
	this->clear_config_();
    else
    {
	this->set_config_();
    }
}

ACE_CLASSIX_Port_Core::~ACE_CLASSIX_Port_Core(void)
{
    if (this->is_configured())
	if (int result = ::portDelete(K_MYACTOR, this->get_handle()) < 0)
	    ACE_ERROR((LM_DEBUG, 
		       "ACE_CLASSIX_Port_Core::~ACE_CLASSIX_PORT_CORE()- "
		       "failed to delete port, error = %d", result));
}

void
ACE_CLASSIX_Port_Core::clear_config_(void)
{
    this->config_ = 0;
    this->addr_.id.uiHead = this->addr_.id.uiTail = 0;
    this->addr_.handle = ACE_INVALID_HANDLE;
}


void
ACE_CLASSIX_Port_Core::dump (void) const
{
  ACE_DEBUG ((LM_DEBUG, "ACE_CLASSIX_Port_Core...\n"));
  char s[100];
  if (!this->is_configured())
      ACE_OS::sprintf(s, "Port has not been allocated\n");
  else
      ACE_OS::sprintf (s, "handle = %d;\tid head = %x, id tail = %x\n",
		       this->get_handle(),
		       this->get_id().uiHead, this->get_id().uiTail);
  ACE_DEBUG ((LM_DEBUG, "%s", s));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}
