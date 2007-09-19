// $Id$

/* -*-C++-*- */

#ifndef SNMP_AGENT_
#define SNMP_AGENT_
// ============================================================================
//
// = LIBRARY
//    ASNMP
//
// = FILENAME
//    snmp_agent.h
//
// = AUTHOR
//    Michael R. MacFaden (mrm@cisco.com)
//
// ============================================================================

class agent_impl;

class snmp_agent {
  // = TITLE
  //   An SNMP v1 agent class
  //
  // = DESCRIPTION
  //   Contructs agents that are capable of processing SNMPv1 requests
  //   over the MIB II System group

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
