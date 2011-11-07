// $Id$

#include "Server_i.h"
#include "Server_Task.h"

Server::Server(CORBA::ORB_ptr orb)
  : orb_(CORBA::ORB::_duplicate(orb))
{
}

void
Server::start_task(Test::Echo_ptr client)
{
  Server_Task * task =
    new Server_Task(client);
  task->activate(THR_NEW_LWP|THR_DETACHED);
}
