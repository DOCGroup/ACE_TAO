/* -*- c++ -*- */
// $Id$

#if !defined (JAWS_WAITER_H)
#define JAWS_WAITER_H

#include "ace/Singleton.h"

#include "JAWS/Assoc_Array.h"
#include "JAWS/Data_Block.h"

typedef JAWS_Assoc_Array<ACE_Thread_ID, JAWS_IO_Handler *> JAWS_Waiter_Base;
typedef JAWS_Assoc_Array_Iterator<ACE_Thread_ID, JAWS_IO_Handler *>
        JAWS_Waiter_Base_Iterator;

class JAWS_Waiter : public JAWS_Waiter_Base
{
public:
  JAWS_Waiter (void);
  virtual ~JAWS_Waiter (void);

  JAWS_Waiter_Base_Iterator &iter (void);
  // returns an iterator to the headers container

  JAWS_IO_Handler * wait_for_completion (void);

private:
  JAWS_Waiter_Base_Iterator iter_;

};

typedef ACE_Singleton<JAWS_Waiter, ACE_SYNCH_MUTEX> JAWS_Waiter_Singleton;

#endif /* JAWS_WAITER_H */
