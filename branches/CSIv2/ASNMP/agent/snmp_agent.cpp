// $Id$

// This server daemon processes SNMPv1 requests over MIB II System group 

#include "snmp_agent.h"
#include "agent_impl.h"  // actual implementation
#include "ace/Get_Opt.h"
#include "ace/Reactor.h"

ACE_RCSID(agent, snmp_agent, "$Id$")

// module globals 
#define SNMP_AGENT_PORT (161)
#define WR_COM ("private")
#define RD_COM ("public")

snmp_agent::snmp_agent() : agent_(0)
{
  ACE_TRACE("snmp_agent::snmp_agent");
}

snmp_agent::~snmp_agent()
{
  ACE_TRACE("snmp_agent::~snmp_agent");
  delete agent_;
}


int snmp_agent::valid() const
{
  ACE_TRACE("snmp_agent::valid");
  return (agent_ != 0);
}

int snmp_agent::set_args(int argc, char *argv[])
{
  ACE_TRACE("snmp_agent::set_args");
  unsigned short port = SNMP_AGENT_PORT;
  char *rd = RD_COM, *wr = WR_COM;

  ACE_Get_Opt get_opt (argc, argv, "p:w:r:hv");
  for (int c; (c = get_opt ()) != -1; ) {
     switch (c)
       {
       case 'p':
	 port = (ACE_OS::atoi (get_opt.optarg));
	 break;
       case 'w': // write community string
	 wr = get_opt.optarg;
	 break;
       case 'r': // read community string
	 rd = get_opt.optarg;
	 break;
        
       case 'h': // help & version info  
       case 'v': 
        ACE_DEBUG ((LM_DEBUG,
	      "(%P|%t) Example SNMP Version 1 Agent - ASNMP/ACE\n"));
         return -1;
         break;

       default:
	 return -1; // invalid arg
       }
  }

   
   agent_ = new agent_impl(port, rd, wr); // opens OS IO port
   return 0;
}

int snmp_agent::run()
{
  ACE_TRACE("snmp_agent::run");
  ACE_ASSERT(agent_);

  // open service port
#if 0
  if (agent_->open_port()) {
    ACE_DEBUG((LM_DEBUG, "(%P|%t) open port %d failed\n", agent_->get_port()));
    return -1;
  } 
#endif

  // Run forever, processing SNMP requests.

  ACE_DEBUG ((LM_DEBUG,
	      "(%P|%t) starting up snmp agent (server) daemon\n"));

  agent_->process_requests();

  ACE_DEBUG ((LM_DEBUG,
	      "(%P|%t) shutting down snmp agent (server) daemon\n"));
  return 0;
}

