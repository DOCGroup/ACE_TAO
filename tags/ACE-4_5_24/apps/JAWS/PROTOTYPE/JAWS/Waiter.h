/* -*- c++ -*- */
// $Id$

#if !defined (JAWS_WAITER_H)
#define JAWS_WAITER_H

#include "ace/Singleton.h"

#include "JAWS/Assoc_Array.h"

class JAWS_IO_Handler;

typedef JAWS_Assoc_Array<ACE_Thread_ID, JAWS_IO_Handler *> JAWS_Waiter_Base;
typedef JAWS_Assoc_Array_Iterator<ACE_Thread_ID, JAWS_IO_Handler *>
        JAWS_Waiter_Base_Iterator;

class JAWS_Waiter : public JAWS_Waiter_Base
{
public:
  JAWS_Waiter (void);
  ~JAWS_Waiter (void);

  JAWS_Waiter_Base_Iterator &iter (void);
  // Returns an iterator to the headers container.

  int index (void);
  // Returns the index into the table associated with calling thread.

  JAWS_IO_Handler * wait_for_completion (int i = -1);
  // The entry point for this class, handles outstanding asynchronous
  // events.  Can optionally accept a parameter that points to which
  // table entry to return.

private:
  JAWS_Waiter_Base_Iterator iter_;

};

typedef ACE_Singleton<JAWS_Waiter, ACE_SYNCH_MUTEX> JAWS_Waiter_Singleton;

#endif /* JAWS_WAITER_H */
