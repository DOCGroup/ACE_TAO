#include "Server_Task.h"

Server_Task::Server_Task (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Server_Task::svc ()
{
  try
    {
      this->orb_->run ();
    }
  catch (CORBA::Exception const &)
    {
    }
  return 0;
}
