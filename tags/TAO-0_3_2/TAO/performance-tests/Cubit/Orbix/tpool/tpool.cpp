// $Id$

#include "tpool.h"

ACE_RCSID(tpool, tpool, "$Id$")

Thread_Pool::Thread_Pool (int n_threads)
  : ACE_Task<ACE_MT_SYNCH> (ACE_Thread_Manager::instance ())
{
  if (this->activate (THR_DETACHED | THR_NEW_LWP, n_threads) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "activate failed"));
}    

int
Thread_Pool::svc (void)
{
  cout << "Pool thread activated" << endl;
  while (1) 
    {
      // Dequeue message
      ACE_Message_Block *mb;
      this->msg_queue ()->dequeue_head (mb);
      CORBA::Request *vp = (CORBA::Request *) mb->base ();
      mb->release ();
      // Hand it off to Orbix
      CORBA::Orbix.continueThreadDispatch (*vp);
    }
  return -1;
}


int
Thread_Pool::inRequestPreMarshal(CORBA::Request &r,  CORBA::Environment&)
{
//   thread_t	tid;
//   thr_create(NULL, 0, worker, (void *)&r, THR_DETACHED, &tid);

  // Enqueue the request
  ACE_Message_Block *mb = new ACE_Message_Block ( (char *) (&r));
  this->msg_queue ()->enqueue (mb);
  // Done
  return -1;
}


                             
