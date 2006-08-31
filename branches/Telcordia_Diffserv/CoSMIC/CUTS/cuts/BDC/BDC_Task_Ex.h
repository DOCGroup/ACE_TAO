// -*- C++ -*-

#ifndef _BDC_TASK_EX_H_
#define _BDC_TASK_EX_H_

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "cuts/config.h"
#include "BDC_Task.h"

// forward declarations
class CUTS_Database_Service;

namespace CUTS
{
  //=========================================================================
  /**
   * @class BDC_Task_Ex
   */
  //=========================================================================

  class BDC_Task_Ex : public BDC_Task
  {
  public:
    /**
     * Initializing constructor. This method initializes the active
     * object, which requires reference to the metrics collection
     * and database service.
     *
     * @param[in]   service       Reference to the database service.
     */
    BDC_Task_Ex (CUTS_Database_Service & service);

    /// Destructor.
    virtual ~BDC_Task_Ex (void);

  protected:
    /// Enqueues a benchmark agent for data collection.
    virtual int handle_agent (const char * instance,
                              CUTS::Benchmark_Agent_ptr agent);

    /// Input handler for the task.
    virtual int handle_input (ACE_HANDLE handle);

  private:
    /// Helper method to archive the current metrics.
    void archive_metrics (void);

    /// Reference to the database service.
    CUTS_Database_Service & database_service_;
  };
}

#if defined (__CUTS_INLINE__)
#include "BDC_Task_Ex.inl"
#endif

#include /**/ "ace/post.h"

#endif  // !defined _BDC_TASK_EX_H_
