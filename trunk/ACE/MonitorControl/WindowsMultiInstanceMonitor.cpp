// $Id$

#include "ace/Log_Msg.h"
#include "ace/SString.h"

#include "MonitorControl/WindowsMultiInstanceMonitor.h"

#if defined (ACE_WIN32)

#include <pdhmsg.h>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    WindowsMultiInstanceMonitor::WindowsMultiInstanceMonitor (
      const ACE_TCHAR *wildcard_path)
      : instances_ (0),
        n_instances_ (0),
        status_ (ERROR_SUCCESS)
    {
      this->value_.doubleValue = 0.0;

      /// Create a string which is a concatentation of the path
      /// name of each 'instance' we need to monitor.

      DWORD paths_size = 4096;
      ACE_LPSTR paths = (ACE_LPSTR) GlobalAlloc (GPTR, paths_size);

      this->status_ = ACE_TEXT_PdhExpandCounterPath (wildcard_path,
                                                     paths,
                                                     &paths_size);

      if (PDH_MORE_DATA == this->status_)
        {
          ++paths_size;
          GlobalFree (paths);
          paths = (ACE_LPSTR) GlobalAlloc (GPTR, paths_size);

          this->status_ = ACE_TEXT_PdhExpandCounterPath (wildcard_path,
                                                         paths,
                                                         &paths_size);
        }

      if (PDH_CSTATUS_VALID_DATA != this->status_)
        {
          ACE_ERROR ((LM_ERROR,
                      "%s: PdhExpandCounterPath failed\n",
                      wildcard_path));
        }

      ACE_LPSTR path = paths;
      int index = 0;

      /// Create a regular Windows monitor for each path name.
      while (*path != 0)
        {
          WindowsMonitor *instance = new WindowsMonitor (path);
          this->instances_.enqueue_tail (instance);
          path += lstrlen (path) + 1;
        }

      GlobalFree (paths);
    }

    WindowsMultiInstanceMonitor::~WindowsMultiInstanceMonitor (void)
    {
      WindowsMonitor *instance = 0;

      /// Destroy the single instance monitors created in the constructor.
      while (this->instances_.dequeue_head (instance) == 0)
        {
          delete instance;
        }
    }

    void
    WindowsMultiInstanceMonitor::win_update (void)
    {
      WindowsMonitor **current_instance = 0;

      /// Sum the values of each single instance monitor.
      for (INSTANCES_ITERATOR i (this->instances_); !i.done (); i.advance ())
        {
          i.next (current_instance);

          (*current_instance)->win_update ();

          this->value_.doubleValue +=
            (*current_instance)->value_.doubleValue;
        }
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* defined (ACE_WIN32) */
