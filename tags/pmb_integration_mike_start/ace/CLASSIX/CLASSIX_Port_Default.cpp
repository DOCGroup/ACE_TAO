/* -*- C++ -*- */
// $Id$

// Defines the ClassiX IPC address family address format.

#define ACE_BUILD_DLL
#include "ace/CLASSIX/CLASSIX_Port_Default.h"

#if !defined (__ACE_INLINE__)
#include "ace/CLASSIX/CLASSIX_Port_Default.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_CLASSIX_Port_Default)

/* ------------------------------------------------------------------------- */
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Singleton<ACE_CLASSIX_Port_Default, 
                             ACE_SYNCH_NULL_MUTEX>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiation ACE_Singleton<ACE_CLASSIX_Port_Default, 
                             ACE_SYNCH_NULL_MUTEX>

#endif
