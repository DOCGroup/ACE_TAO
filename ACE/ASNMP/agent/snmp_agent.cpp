// $Id$

// This server daemon processes SNMPv1 requests over MIB II System group

#include "snmp_agent.h"
#include "agent_impl.h"  // actual implementation
#include "ace/Argv_Type_Converter.h"
#include "ace/Get_Opt.h"
#include "ace/Reactor.h"

// module globals
#define SNMP_AGENT_PORT (161)
#define WR_COM ACE_TEXT ("private")
#define RD_COM ACE_TEXT ("public")

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
  const ACE_TCHAR *rd = RD_COM;
  const ACE_TCHAR *wr = WR_COM;
  ACE_Argv_Type_Converter to_tchar (argc, argv);

  ACE_Get_Opt get_opt
    (argc, to_tchar.get_TCHAR_argv (), ACE_TEXT ("p:w:r:hv"));

  for (int c; (c = get_opt ()) != -1; )
    {
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
          ACE_DEBUG
            ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Example SNMP Version 1 Agent - ASNMP/ACE\n")));
          return -1;
        default:
          return -1; // invalid arg
        }
    }

   agent_ = new agent_impl(port,
                           ACE_TEXT_ALWAYS_CHAR (rd),
                           ACE_TEXT_ALWAYS_CHAR (wr)); // opens OS IO port
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
              ACE_TEXT ("(%P|%t) starting up snmp agent (server) daemon\n")));

  agent_->process_requests();

  ACE_DEBUG
    ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) shutting down snmp agent (server) daemon\n")));
  return 0;
}

