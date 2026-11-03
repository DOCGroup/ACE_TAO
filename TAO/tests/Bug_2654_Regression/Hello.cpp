#include "Hello.h"
#include "ace/Task.h"

ACE_THR_FUNC_RETURN killer (void *arg)
{
  // Adopt the ORB reference transferred by Hello::method.
  CORBA::ORB_var const orb = static_cast<CORBA::ORB_ptr> (arg);

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) server exiting\n"));
  orb->shutdown (true);

  return 0;
}

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{}

void
Hello::set_callback (Test::CallBack_ptr cb)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) got callback\n"));
  this->callback_ = Test::CallBack::_duplicate (cb);
  this->callback_->method2 ();
}

void
Hello::method (CORBA::Short count)
{
  if (++this->count_ > 10)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) supplied count = %d\n", count));
      PortableServer::POA_var const poa = this->_default_POA ();
      PortableServer::POAManager_var const mgr = poa->the_POAManager ();
      mgr->hold_requests (false);
      CORBA::ORB_var orb = CORBA::ORB::_duplicate (this->orb_.in ());
      if (ACE_Thread_Manager::instance ()->spawn_n (1, killer, orb.in ()) == -1)
        ACE_ERROR ((LM_ERROR, "Unable to start server shutdown thread\n"));
      else
        (void) orb._retn ();
    }
}
