/* $Id$ */
#ifndef OBJECT_COUNTER_H
#define OBJECT_COUNTER_H
#include "ace/pre.h"

#include "ace/Singleton.h"
#include "ace/Synch.h"
#include "ace/SString.h"
#include <sstream>

class ACE_Object_Counter
  {
  public:

    struct object_id {
      uint32_t id;
      uint64_t tid;
      uint32_t queue_id;
    };

    ACE_Object_Counter ();

    object_id increment (void);

  private:
    unsigned int m_counter;
};

class ACE_Queue_Counter
{
 public:
    ACE_Queue_Counter ();

    uint32_t increment (void);

 private:
    uint32_t m_counter;
};


typedef ACE_TSS_Singleton<ACE_Object_Counter, ACE_Null_Mutex> ObjectCounterSingleton;
#define ACE_OBJECT_COUNTER ObjectCounterSingleton::instance()

#include "ace/post.h"
#endif /* OBJECT_COUNTER_H */

