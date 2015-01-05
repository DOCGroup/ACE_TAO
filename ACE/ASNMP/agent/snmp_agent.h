
/* -*-C++-*- */

#ifndef SNMP_AGENT_
#define SNMP_AGENT_
//=============================================================================
/**
 *  @file    snmp_agent.h
 *
 *  $Id$
 *
 *  @author Michael R. MacFaden (mrm@cisco.com)
 */
//=============================================================================


class agent_impl;

/**
 * @class snmp_agent
 *
 * @brief An SNMP v1 agent class
 *
 * Contructs agents that are capable of processing SNMPv1 requests
 * over the MIB II System group
 */
class snmp_agent {

  public:
  snmp_agent(); // uses port 161, public, private
  ~snmp_agent();

  int valid() const;
  // did object construct ok

  int set_args(int argc, char *argv[]);
  // runtime arguments such as community strings, port to use

  int run();
  // begin processing requests

  private:
  snmp_agent(const snmp_agent&);
  agent_impl *agent_;
};

#endif // SNMP_AGENT_
