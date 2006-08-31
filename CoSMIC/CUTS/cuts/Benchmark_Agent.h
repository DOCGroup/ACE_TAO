// -*- C++ -*-

//=============================================================================
/**
 * @file    Benchmark_Agent.h
 *
 * @brief   Contains the benchmark agent class.
 *
 * $Id$
 *
 * @author James H. Hill <hillj@isis.vanderbilt.edu>
 */
//=============================================================================

#ifndef _CUTS_BENCHMARK_AGENT_H_
#define _CUTS_BENCHMARK_AGENT_H_

#include "cuts/config.h"
#include "cuts/CUTS_export.h"
#include <map>
#include <set>
#include <string>

// forward declarations
class CUTS_Port_Agent;

typedef std::set <CUTS_Port_Agent *> CUTS_Port_Agent_Set;

//=============================================================================
/**
 * @class CUTS_Benchmark_Agent
 *
 * @brief Class responsible for collecting all performance data.
 */
//=============================================================================

class CUTS_Export CUTS_Benchmark_Agent
{
public:
  /// Constructor.
  CUTS_Benchmark_Agent (void);

  /// Destructor.
  virtual ~CUTS_Benchmark_Agent (void);

  /// Register a port agent.
  bool register_port_agent (CUTS_Port_Agent * agent);

  /// Unregister a port agent.
  void unregister_port_agent (CUTS_Port_Agent * agent);

  /// Get the parent ID of the agent.
  long parent (void) const;

  /// Set the parent ID of the agent.
  void parent (long parent);

  /// Register an exit point w/ the agent.
  bool register_exit_point (const std::string & uuid,
                            const std::string & name);

  /// Unregister an exit point w/ the agent.
  void unregister_exit_point (const std::string & uuid);

protected:
  /// Get the collection of port agents.
  CUTS_Port_Agent_Set & port_agents (void);

  /// Type definition for mapping exit point UUIDs to names.
  typedef std::map <std::string, std::string> Exit_Points;

  /// Collection of exit points available to the agent.
  Exit_Points exit_points_;

private:
  /// Owner of this object.
  long parent_;

  /// Set of port agents managed by this benchmark agent.
  CUTS_Port_Agent_Set port_agents_;
};

#if defined (__CUTS_INLINE__)
#include "cuts/Benchmark_Agent.inl"
#endif

#endif  // !defined _CUTS_BENCHMARK_AGENT_H_
