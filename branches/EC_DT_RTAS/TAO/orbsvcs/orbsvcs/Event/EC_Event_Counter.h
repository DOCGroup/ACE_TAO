/* $Id$ */
#ifndef EC_EVENT_COUNTER_H
#define EC_EVENT_COUNTER_H
#include "ace/pre.h"

#include "ace/Singleton.h"
#include "ace/Synch.h"
#include "ace/SString.h"
#include <sstream>

class EC_Event_Counter
{
 public:

  struct event_id {
    uint32_t id;
    uint32_t tid;
  };

  EC_Event_Counter ();

  inline event_id increment (void);

 private:
  unsigned int m_counter;
};

#include "EC_Event_Counter.i"

typedef ACE_TSS_Singleton<EC_Event_Counter, ACE_Null_Mutex> EventCounterSingleton;
#define EC_EVENT_COUNTER EventCounterSingleton::instance()

#include "ace/post.h"
#endif /* EC_EVENT_COUNTER_H */

