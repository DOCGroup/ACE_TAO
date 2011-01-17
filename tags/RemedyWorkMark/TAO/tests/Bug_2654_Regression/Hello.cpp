// $Id$

#include "Hello.h"
#include "ace/Task.h"

ACE_THR_FUNC_RETURN killer (void *arg)
{
  CORBA::ORB_var orb = reinterpret_cast<CORBA::ORB_ptr> (arg);

  ACE_DEBUG ((LM_DEBUG,"(%P|%t) server exiting\n"));
  orb->shutdown (1);

  return 0;
}

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    count_(0)

{
}

void
Hello::set_callback (Test::CallBack_ptr cb)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) got callback\n"));
  this->callback_ = Test::CallBack::_duplicate(cb);
  this->callback_->method2();
}

void
Hello::method (CORBA::Short count)
{
  if (++this->count_ > 10)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) supplied count = %d\n", count));
      PortableServer::POA_var poa = this->_default_POA();
      PortableServer::POAManager_var mgr = poa->the_POAManager();
      mgr->hold_requests(false);
      // Pass duplicated ptr to a thread and let the thread to free the reference.
      CORBA::ORB_ptr orb = CORBA::ORB::_duplicate (this->orb_.in ());
      ACE_Thread_Manager::instance()->spawn_n (1,
                                               ACE_THR_FUNC (killer),
                                               static_cast<void*> (orb));
    }
}
