/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//     ace
// 
// = FILENAME
//     CLASSIX_OS.cpp
//
// = AUTHOR(S)
//     Nokia Telecommunications
// 
// ============================================================================

#include "ace/CLASSIX/CLASSIX_OS.h"

#if !defined (__ACE_INLINE__)
#include "ace/CLASSIX/CLASSIX_OS.i"
#endif /* __ACE_INLINE__ */

#include "ace/CLASSIX/CLASSIX_Select_Reactor.h"

/* ------------------------------------------------------------------------- */

ACE_ALLOC_HOOK_DEFINE(ACE_CLASSIX_OS)
ACE_ALLOC_HOOK_DEFINE(ACE_CLASSIX_Msg)

#if 0
ACE_ALLOC_HOOK_DEFINE(ACE_CLASSIX_Mgr)
ACE_CLASSIX_Mgr::ACE_CLASSIX_Mgr(void)
{
}

ACE_CLASSIX_Mgr::~ACE_CLASSIX_Mgr(void)
{
}

ACE_SYNCH_RECURSIVE_MUTEXT&
ACE_CLASSIX_Mgr::ref_lock(void) const;
{
    return port_lock_;
}
#endif
/* ------------------------------------------------------------------------- */
KnUniqueId ACE_CLASSIX_OS::null_KnUniqueId_;
ACE_Recursive_Thread_Mutex ACE_CLASSIX_OS::lock_;

ACE_CLASSIX_OS::ACE_CLASSIX_OS(void)
{
  // Create a actor-wide CLASSIX reactor
  ACE_Reactor *r = 
    ACE_Reactor::instance
    (new ACE_Reactor(new ACE_CLASSIX_Select_Reactor(), 1), 1);
  delete r;
  
  // Fills in a null KnUniqueId so that it can be referenced when required
  this->null_KnUniqueId_.uiHead = this->null_KnUniqueId_.uiTail = 0;
}

ssize_t
ACE_CLASSIX_OS::convert_io_error(ssize_t theOriginal)
{
    switch (theOriginal)
    {
    case K_EFULL:
	return ENOBUFS;

    case K_EINVAL:
	return EFTYPE;

    case K_EFAULT:
	return EFAULT;

    case K_ETOOMUCH:
	return EMSGSIZE;

    case K_EUNKNOWN:
	return EBADF;

    case K_ENOPORT:
	return EBADF;

    case K_ETIMEOUT:
	return ETIMEOUT;

    case K_EABORT:
	return EINTR;

    default:
	ACE_DEBUG((LM_DEBUG, "unknonw error: %d\n", theOriginal));
	return EFAULT;
    }
}



/* ------------------------------------------------------------------------- */

void
ACE_CLASSIX_Msg::dump(void) const
{
    ACE_DEBUG ((LM_INFO, ACE_BEGIN_DUMP, this));

    char s[100];
    ACE_OS::sprintf (s, "flag = %x\n"
		     "body size = %d\t addr = %x\n"
		     "annex addr = %x\n"
		     "seq no = %d\n",
		     this->msg_.flags,
		     this->msg_.bodySize, this->msg_.bodyAddr,
		     this->msg_.annexAddr,
		     this->msg_.seqNum);
    ACE_DEBUG ((LM_INFO, "%s", s));
    ACE_DEBUG ((LM_INFO, ACE_END_DUMP));    
}
