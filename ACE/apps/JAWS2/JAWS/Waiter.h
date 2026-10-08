/* -*- c++ -*- */
#ifndef JAWS_WAITER_H
#define JAWS_WAITER_H

#include "ace/Singleton.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch_Traits.h"
#include "JAWS/Assoc_Array.h"
#include "JAWS/Export.h"

class JAWS_IO_Handler;

using JAWS_Thread_ID = ACE_thread_t;

using JAWS_Waiter_Base = JAWS_Assoc_Array<JAWS_Thread_ID, JAWS_IO_Handler *>;
typedef JAWS_Assoc_Array_Iterator<JAWS_Thread_ID, JAWS_IO_Handler *>
        JAWS_Waiter_Base_Iterator;

class JAWS_Export JAWS_Waiter : public JAWS_Waiter_Base
{
public:
  JAWS_Waiter ();
  ~JAWS_Waiter ();

  JAWS_Waiter_Base_Iterator &iter ();
  // Returns an iterator to the headers container.

  int index ();
  // Returns the index into the table associated with calling thread.

  JAWS_IO_Handler * wait_for_completion (int i = -1);
  // The entry point for this class, handles outstanding asynchronous
  // events.  Can optionally accept a parameter that points to which
  // table entry to return.

private:
  JAWS_Waiter_Base_Iterator iter_;
};

using JAWS_Waiter_Singleton = ACE_Singleton<JAWS_Waiter, ACE_SYNCH_MUTEX>;

#endif /* JAWS_WAITER_H */
