// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Hash_Map_Manager_rt.cpp
//
// = AUTHOR
//    Jeremy M. Nolle <jmn3@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_HASH_MAP_MANAGER_RT_CPP
#define ACE_HASH_MAP_MANAGER_RT_CPP

#include "ace/Hash_Map_Manager_rt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (__ACE_INLINE__)
# include "ace/Hash_Map_Manager_rt.i"
#elif defined (__SUNPRO_CC) && (__SUNPRO_CC == 0x420)
// If ACE_INLINE is on and we are on SunCC, undefine ACE_INLINE,
// include Hash_Map_Manager_Tcmd
, and then redefine ACE_INLINE.
// This nonsense is necessary since SunCC (version 4.2) cannot inline
// the code in Hash_Map_Manager_T.i (with the fast option).
# undef ACE_INLINE
# define ACE_INLINE
# include "ace/Hash_Map_Manager_rt.i"
# undef ACE_INLINE
# define ACE_INLINE inline
#endif /* __ACE_INLINE__ */

#include "ace/Synch.h"
#include "ace/Service_Config.h"
#include "ace/Malloc.h"

Hash_Map_Manager_rt::Hash_Map_Manager_rt()
{
//this is a stub for now
}

Hash_Map_Manager_rt::~Hash_Map_Manager_rt()
{
// and destub me
}


#endif /* ACE_HASH_MAP_MANAGER_RT_CPP */
