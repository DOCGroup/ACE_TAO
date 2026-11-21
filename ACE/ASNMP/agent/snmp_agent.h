
/* -*-C++-*- */

#ifndef SNMP_AGENT_
#define SNMP_AGENT_
#include <memory>
//=============================================================================
/**
 *  @file    snmp_agent.h
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
class snmp_agent
{
public:
  snmp_agent (); // uses port 161, public, private
  ~snmp_agent ();
  snmp_agent (snmp_agent const &) = delete;
  snmp_agent (snmp_agent &&) = delete;
  snmp_agent &operator= (snmp_agent const &) = delete;
  snmp_agent &operator= (snmp_agent &&) = delete;

  /// Returns whether construction succeeded.
  bool valid () const;

  /// Sets runtime arguments such as the community strings and port.
  int set_args (int argc, char *argv[]);

  /// Begins processing requests.
  int run ();

private:
  std::unique_ptr<agent_impl> agent_;
};

#endif // SNMP_AGENT_
