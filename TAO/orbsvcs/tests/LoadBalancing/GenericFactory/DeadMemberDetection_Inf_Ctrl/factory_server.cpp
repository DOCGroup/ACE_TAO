// $Id$

#include "LB_Factory_Server.h"
#include "Factory.h"
#include "Simple.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  LB_Factory_Server lb_server (argc, argv);

  if (lb_server.init () == -1)
    return 1;

  lb_server.run ();

  ACE_DEBUG ((LM_DEBUG, "(%P|%t)%T server - event loop finished\n"));

  if (lb_server.destroy () == -1)
    return 1;

  return 0;
}
