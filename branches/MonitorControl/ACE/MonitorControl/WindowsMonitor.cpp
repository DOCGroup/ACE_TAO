// $Id$

#include "ace/Log_Msg.h"

#include "MonitorControl/WindowsMonitor.h"

#if defined (ACE_WIN32)

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    WindowsMonitor::WindowsMonitor (const char *path)
      : query_ (0),
        counter_ (0),
        status_ (ERROR_SUCCESS)
    {
      /// Create a query and a counter here so it doesn't have
      /// to be done with each update.
    
      this->status_ = PdhOpenQuery (0, 0, &this->query_);

      if (ERROR_SUCCESS != this->status_)
        {
          ACE_ERROR ((LM_DEBUG, "PdhOpenQuery failed\n"));
        }

      this->status_ =
        PdhAddCounter (this->query_,
                       path,
                       0,
                       &this->counter_);

      if (ERROR_SUCCESS != this->status_)
        {
          ACE_ERROR ((LM_DEBUG, "PdhAddCounter %s failed\n", path));
        }
    }

    void
    WindowsMonitor::win_update (void)
    {
      PdhCollectQueryData (this->query_);
      PdhGetFormattedCounterValue (this->counter_,
                                   PDH_FMT_DOUBLE,
                                   0,
                                   &this->value_);
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* defined (ACE_WIN32) */
