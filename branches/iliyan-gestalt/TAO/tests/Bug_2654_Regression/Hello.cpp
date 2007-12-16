//
// $Id$
//
#include "Hello.h"
#include "ace/Task.h"

class Killer : public ACE_Task_Base
{
public:
  Killer (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
  {
  }

  int svc (void)
  {
    ACE_DEBUG ((LM_DEBUG,"(%P|%t) server exiting\n"));
    this->orb_->shutdown (1);
    return 0;
  }

private:
  CORBA::ORB_var orb_;
};

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    count_(0)

{
}

void
Hello::set_callback (Test::CallBack_ptr cb)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) got callback \n"));
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
      Killer *k = new Killer (orb_.in ());
      k->activate();
    }
}
