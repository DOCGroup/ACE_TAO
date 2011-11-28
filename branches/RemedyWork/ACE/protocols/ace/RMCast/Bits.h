// $Id$
// author    : Boris Kolpackov <boris@kolpackov.net>

#ifndef ACE_RMCAST_BITS_H
#define ACE_RMCAST_BITS_H

#include "ace/Auto_Ptr.h"
#include "ace/Thread_Mutex.h"
#include "ace/Condition_T.h"
#include "ace/Synch_Traits.h"

namespace ACE_RMCast
{
  typedef ACE_SYNCH_MUTEX Mutex;
// FUZZ: disable check_for_ACE_Guard
  typedef ACE_Guard<Mutex> Lock;
  typedef ACE_Condition<Mutex> Condition;
// FUZZ: enable check_for_ACE_Guard
}


#endif  // ACE_RMCAST_BITS_H
