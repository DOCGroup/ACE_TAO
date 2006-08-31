// $Id$

//
// register_port_agent
//
CUTS_INLINE
bool CUTS_Benchmark_Agent::register_port_agent (CUTS_Port_Agent * agent)
{
  return this->port_agents_.insert (agent).second;
}

//
// unregister_port_agent
//
CUTS_INLINE
void CUTS_Benchmark_Agent::unregister_port_agent (CUTS_Port_Agent * agent)
{
  this->port_agents_.erase (agent);
}

//
// parent
//
CUTS_INLINE
void CUTS_Benchmark_Agent::parent (long parent)
{
  this->parent_ = parent;
}

CUTS_INLINE
long CUTS_Benchmark_Agent::parent (void) const
{
  return this->parent_;
}

//
// unregister_exit_point
//
CUTS_INLINE
void CUTS_Benchmark_Agent::unregister_exit_point (const std::string & uuid)
{
  this->exit_points_.erase (uuid);
}

CUTS_INLINE
bool CUTS_Benchmark_Agent::register_exit_point (const std::string & uuid,
                                                const std::string & name)
{
  return this->exit_points_.insert (
    Exit_Points::value_type (uuid, name)).second;
}

//
// port_agents
//
CUTS_INLINE
CUTS_Port_Agent_Set & CUTS_Benchmark_Agent::port_agents (void)
{
  return this->port_agents_;
}
