#include "LB_server.h"
#include "Factory.h"
#include "Simple.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  char const *const location1 = "MyLocation 1";
  char const *const location2 = "MyLocation 2";

  LB_server lb_server (argc, argv);

  if (lb_server.start_orb_and_poa () != 0)
    return 1;

  if (lb_server.create_object_group (location1, location2) != 0)
    return 1;

  if (lb_server.run () != 0)
    return 1;

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

  if (lb_server.destroy () != 0)
    return 1;

  return 0;
}
