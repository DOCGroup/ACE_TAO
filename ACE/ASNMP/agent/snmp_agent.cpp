// This server daemon processes SNMPv1 requests over MIB II System group

#include "snmp_agent.h"
#include "agent_impl.h"  // actual implementation
#include "ace/Argv_Type_Converter.h"
#include "ace/Get_Opt.h"

constexpr unsigned short SNMP_AGENT_PORT = 161;
ACE_TCHAR const RD_COM[] = ACE_TEXT ("public");
ACE_TCHAR const WR_COM[] = ACE_TEXT ("private");

snmp_agent::snmp_agent ()
{
  ACE_TRACE ("snmp_agent::snmp_agent");
}

snmp_agent::~snmp_agent ()
{
  ACE_TRACE ("snmp_agent::~snmp_agent");
}

bool snmp_agent::valid () const
{
  ACE_TRACE ("snmp_agent::valid");
  return agent_ != nullptr;
}

int snmp_agent::set_args (int argc, char *argv[])
{
  ACE_TRACE ("snmp_agent::set_args");
  unsigned short port {SNMP_AGENT_PORT};
  ACE_TCHAR const *rd = RD_COM;
  ACE_TCHAR const *wr = WR_COM;
  ACE_Argv_Type_Converter to_tchar (argc, argv);

  ACE_Get_Opt get_opt
    (argc, to_tchar.get_TCHAR_argv (), ACE_TEXT ("p:w:r:hv"));

  for (int option = get_opt (); option != -1; option = get_opt ())
    {
      switch (option)
        {
        case 'p':
          port = static_cast<unsigned short> (ACE_OS::atoi (get_opt.optarg));
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

  agent_ = std::make_unique<agent_impl> (port,
                                         ACE_TEXT_ALWAYS_CHAR (rd),
                                         ACE_TEXT_ALWAYS_CHAR (wr)); // opens OS IO port
  return 0;
}

int snmp_agent::run ()
{
  ACE_TRACE ("snmp_agent::run");
  ACE_ASSERT (agent_);

  // Run forever, processing SNMP requests.

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) starting up snmp agent (server) daemon\n")));

  int const result = agent_->process_requests ();

  ACE_DEBUG
    ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) shutting down snmp agent (server) daemon\n")));
  return result;
}

