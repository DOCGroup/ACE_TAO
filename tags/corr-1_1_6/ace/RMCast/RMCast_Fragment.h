// $Id$

// ============================================================================
//
// = DESCRIPTION
//   The fragmentation task for the reliable multicast library
//
// = AUTHOR
//    Carlos O'Ryan <coryan@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_RMCAST_FRAGMENT_H
#define ACE_RMCAST_FRAGMENT_H
#include "ace/pre.h"

#include "RMCast_Export.h"
#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#ifndef ACE_RMCAST_DEFAULT_FRAGMENT_SIZE
# define ACE_RMCAST_DEFAULT_FRAGMENT_SIZE 1024
#endif /* ACE_RMCAST_DEFAULT_FRAGMENT_SIZE */

template <ACE_SYNCH_DECL>
class ACE_RMCast_Export ACE_RMCast_Fragment : public ACE_Task<ACE_SYNCH_USE>
{
public:
  ACE_RMCast_Fragment (ACE_Thread_Manager *thr_mgr = 0,
                       ACE_Message_Queue<ACE_SYNCH_USE> *mq = 0);
  // Constructor

  virtual ~ACE_RMCast_Fragment (void);
  // Destructor

  size_t max_fragment_size (void) const;
  // Accessor for the max_fragment size.
  // There is no modifier, the maximum fragment size is obtained using
  // feedback from the lower layer (transport?)

  // = The ACE_Task methods
  int put (ACE_Message_Block *, ACE_Time_Value *timeout = 0);

private:
  size_t max_fragment_size_;

  ACE_SYNCH_MUTEX_T mutex_;
  ACE_UINT32 sequence_number_generator_;
  // The sequence number generator
};

#if defined (__ACE_INLINE__)
#include "RMCast_Fragment.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "RMCast_Fragment.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("RMCast_Fragment.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* ACE_RMCAST_FRAGMENT_H */
