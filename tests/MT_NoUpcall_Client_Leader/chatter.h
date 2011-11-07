// $Id$

#if !defined( CHATTER_H)
#define CHATTER_H

#include "worker.h"

#include "ace/Condition_T.h"
#include "ace/Mutex.h"

class Chatter : public Worker
{
public:
  Chatter (CORBA::ORB_ptr orb, const ACE_TCHAR *ior, ACE_Condition<ACE_Mutex>& cond);
  virtual int svc (void);
  int farewell ();
  long nrequests (void);
  long nreplies (void);
public:
  volatile long nrequests_;
  volatile long nreplies_;
private:
  const ACE_TCHAR* ior_;
  ACE_Condition<ACE_Mutex>& cond_;
  TAO_SYNCH_MUTEX request_reply_count_mutex_;
};
#endif /* CHATTER_H */
