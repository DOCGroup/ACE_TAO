#include "tpr.h"

void *
worker(void *vp)
{
  ACE_Thread_Control tc (ACE_Thread_Manager::instance ());
  CORBA::Orbix.continueThreadDispatch (*(CORBA::Request *)vp);
  return NULL;
}


int
tpr_filter::inRequestPreMarshal(CORBA::Request &r,  CORBA::Environment&)
{
//   thread_t	tid;
//   thr_create(NULL, 0, worker, (void *)&r, THR_DETACHED, &tid);

  ACE_Thread_Manager *thr_mgr = ACE_Thread_Manager::instance ();
  thr_mgr->spawn (ACE_THR_FUNC (worker),
                  (void *) &r, 
                  THR_DETACHED);

  return -1;
}

tpr_filter tpr_dispatcher;
