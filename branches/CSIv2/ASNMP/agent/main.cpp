// $Id$

// This server daemon processes SNMP Version 1 get, get-next, and set
// commands. over the MIB II "System" group only.

#include "ace/config-all.h"
#include "snmp_agent.h"

ACE_RCSID(agent, main, "$Id$")

int 
main (int argc, char *argv[])
{
  ACE::init ();
  snmp_agent the_agent;

  if (the_agent.set_args(argc, argv)) 
    return 1;

  if (!the_agent.valid()) {
    return 1;
  }

  the_agent.run(); // main loop

  return 0;
}
