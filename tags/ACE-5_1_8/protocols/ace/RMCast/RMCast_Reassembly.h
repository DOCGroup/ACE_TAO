// $Id$

// ============================================================================
//
// = DESCRIPTION
//   The reassembly task for the reliable multicast library
//
// = AUTHOR
//    Carlos O'Ryan <coryan@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_RMCAST_REASSEMBLY_H
#define ACE_RMCAST_REASSEMBLY_H
#include "ace/pre.h"

#include "RMCast_Export.h"
#include "ace/Task.h"
#include "ace/Hash_Map_Manager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_RMCast_Partial_Message;

template <ACE_SYNCH_DECL>
class ACE_RMCast_Export ACE_RMCast_Reassembly : public ACE_Task<ACE_SYNCH_USE>
{
public:
  ACE_RMCast_Reassembly (ACE_Thread_Manager *thr_mgr = 0,
                       ACE_Message_Queue<ACE_SYNCH_USE> *mq = 0);
  // Constructor

  virtual ~ACE_RMCast_Reassembly (void);
  // Destructor

  // = The ACE_Task methods
  int put (ACE_Message_Block *, ACE_Time_Value *timeout = 0);

private:
  ACE_SYNCH_MUTEX_T mutex_;
  typedef
      ACE_Hash_Map_Manager<ACE_UINT32,ACE_RMCast_Partial_Message*,ACE_Null_Mutex>
      Message_Map;
  typedef
      ACE_Hash_Map_Iterator<ACE_UINT32,ACE_RMCast_Partial_Message*,ACE_Null_Mutex>
      Message_Map_Iterator;

  Message_Map messages_;
  // The array of partially received messages
};

#if defined (__ACE_INLINE__)
#include "RMCast_Reassembly.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "RMCast_Reassembly.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("RMCast_Reassembly.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* ACE_RMCAST_REASSEMBLY_H */
