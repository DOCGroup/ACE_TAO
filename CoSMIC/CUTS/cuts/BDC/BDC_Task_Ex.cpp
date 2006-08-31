// $Id$

#include "BDC_Task_Ex.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Reactor.h"
#include "ace/CORBA_macros.h"
#include "ace/Log_Msg.h"

#if !defined (__CUTS_INLINE__)
#include "BDC_Task_Ex.inl"
#endif

#include "cuts/svcs/dbase/DBase_Service.h"

namespace CUTS
{
  //
  // BDC_Task_Ex
  //
  BDC_Task_Ex::BDC_Task_Ex (CUTS_Database_Service & database_service)
  : database_service_ (database_service)
  {

  }

  //
  // ~BDC_Task_Ex
  //
  BDC_Task_Ex::~BDC_Task_Ex (void)
  {

  }

  //
  // handle_agent
  //
  int BDC_Task_Ex::handle_agent (const char * instance,
                                 ::CUTS::Benchmark_Agent_ptr agent)
  {
    int retval = BDC_Task::handle_agent (instance, agent);

    if (retval == 0)
    {
      this->archive_metrics ();
    }

    return retval;
  }

  //
  // handle_input
  //
  int BDC_Task_Ex::handle_input (ACE_HANDLE handle)
  {
    int retval = BDC_Task::handle_input (handle);

    if (retval == 0)
    {
      this->archive_metrics ();
    }

    return retval;
  }

  //
  // archive_metrics
  //
  void BDC_Task_Ex::archive_metrics (void)
  {
    if (this->system_metrics_ != 0)
    {
      this->database_service_.archive_system_metrics (
        *this->system_metrics_);
    }
    else
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("[%M] -%T - metrics do not exist; ")
                  ACE_TEXT ("cannot log to database\n")));
    }
  }
}
