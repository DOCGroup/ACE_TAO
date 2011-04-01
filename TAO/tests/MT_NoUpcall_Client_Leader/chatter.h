#if !defined( CHATTER_H)
#define CHATTER_H

#include "worker.h"

#if 0
#include "ace/Condition_T.h"
#include "ace/Mutex.h"
#endif

class Chatter : public Worker
{
public:
  Chatter (CORBA::ORB_ptr orb, const char *ior, ACE_Condition<ACE_Mutex>& cond);
  virtual int svc (void);
  int farewell (ACE_ENV_SINGLE_ARG_DECL);
  long nrequests (void);
  long nreplies (void);
public:
  long nrequests_;
  long nreplies_;
private:
  CORBA::String_var ior_;
  ACE_Condition<ACE_Mutex>& cond_;
};
#endif /* CHATTER_H */
