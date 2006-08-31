// $Id$

#ifndef _CUTS_CCM_COMPONENT_REGISTRY_H_
#define _CUTS_CCM_COMPONENT_REGISTRY_H_

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Benchmark_Data_Collector_exec_export.h"
#include "Benchmark_Data_CollectorEC.h"
#include "cuts/config.h"
#include "cuts/Component_Registry.h"
#include "ace/RW_Thread_Mutex.h"

#include <map>
#include <string>

namespace CUTS
{
  //===========================================================================
  /**
   * @class Benchmark_Agent_Handler
   */
  //===========================================================================

  class Benchmark_Agent_Handler
  {
  public:
    /**
     * Callback method for handling a benchmark agent. This method
     * is called everytime an agent needs "attention".
     *
     * @param[in]     agent       Reference to the benchmark agent.
     */
    virtual int handle_agent (const char * instance,
                              ::CUTS::Benchmark_Agent_ptr agent) = 0;
  };

  //===========================================================================
  /**
   * @class CCM_Component_Registry_Node
   */
  //===========================================================================

  class BENCHMARK_DATA_COLLECTOR_EXEC_Export CCM_Component_Registry_Node :
    public CUTS_Component_Registry_Node
  {
  public:
    CCM_Component_Registry_Node (void);

    CCM_Component_Registry_Node (::CUTS::Benchmark_Agent_ptr agent);

    virtual ~CCM_Component_Registry_Node (void);

    ::CUTS::Benchmark_Agent_ptr benchmark_agent (void);

    void benchmark_agent (::CUTS::Benchmark_Agent_ptr agent);

    void reset (void);

  private:
    ::CUTS::Benchmark_Agent_ptr benchmark_agent_;
  };

  //===========================================================================
  /**
   * @class CCM_Component_Registry
   */
  //===========================================================================

  class BENCHMARK_DATA_COLLECTOR_EXEC_Export CCM_Component_Registry :
    public CUTS_Component_Registry
  {
  public:
    /// Constructor.
    CCM_Component_Registry (void);

    /// Destructor.
    ~CCM_Component_Registry (void);

    long register_component (const char * uuid,
                             ::CUTS::Benchmark_Agent_ptr agent);

    bool unregister_component (const char * uuid,
                               ::CUTS::Benchmark_Agent_ptr agent);

    long get_registration (const char * uuid,
                           bool auto_register = true);

    size_t get_benchmark_agents (Benchmark_Agent_Handler * handler,
         size_t & count);

    void reset_registration (const char * uuid);

  private:
    CCM_Component_Registry_Node * get_node (const char * uuid);

    ACE_RW_Thread_Mutex lock_;
  };
}

#include /**/ "ace/post.h"

#endif  // !defined _CUTS_CCM_COMPONENT_REGISTRY_H_
