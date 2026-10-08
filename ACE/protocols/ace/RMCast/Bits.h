// author    : Boris Kolpackov <boris@kolpackov.net>

#ifndef ACE_RMCAST_BITS_H
#define ACE_RMCAST_BITS_H

#include <memory>
#include "ace/Thread_Mutex.h"
#include "ace/Condition_T.h"
#include "ace/Synch_Traits.h"

namespace ACE_RMCast
{
  using Mutex = ACE_SYNCH_MUTEX;
// FUZZ: disable check_for_ACE_Guard
  using Lock = ACE_Guard<Mutex>;
  using Condition = ACE_Condition<Mutex>;
// FUZZ: enable check_for_ACE_Guard
}


#endif  // ACE_RMCAST_BITS_H
