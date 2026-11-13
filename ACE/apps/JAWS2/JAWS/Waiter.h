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
using JAWS_Waiter_Base_Iterator = JAWS_Assoc_Array_Iterator<JAWS_Thread_ID, JAWS_IO_Handler *>;

class JAWS_Export JAWS_Waiter : public JAWS_Waiter_Base
{
public:
  JAWS_Waiter ();
  ~JAWS_Waiter ();

  /// Return an iterator over the waiter table.
  JAWS_Waiter_Base_Iterator &iter ();

  /// Return the index associated with the calling thread.
  int index ();

  /// Wait for an asynchronous event and return its handler. If i is provided,
  /// return the entry at that table index.
  JAWS_IO_Handler * wait_for_completion (int i = -1);

private:
  JAWS_Waiter_Base_Iterator iter_;
};

using JAWS_Waiter_Singleton = ACE_Singleton<JAWS_Waiter, ACE_SYNCH_MUTEX>;

#endif /* JAWS_WAITER_H */
