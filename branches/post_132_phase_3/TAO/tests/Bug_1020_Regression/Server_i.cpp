#include "Server_i.h"
#include "Server_Task.h"

ACE_RCSID (Bug_1230_Regression,
           Server, "$Id$")

Server::Server(CORBA::ORB_ptr orb)
  : orb_(CORBA::ORB::_duplicate(orb))
{
}

void
Server::start_task(Test::Echo_ptr client
                   TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC((CORBA::SystemException))
{
  Server_Task * task =
    new Server_Task(client);
  task->activate(THR_NEW_LWP|THR_DETACHED);
}
