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
      const char *wildcard_path)
      : instances_ (0),
        n_instances_ (0),
        status_ (ERROR_SUCCESS)
    {
      this->value_.doubleValue = 0.0;
      
      DWORD paths_size = 4096;
      LPSTR paths = (LPSTR) GlobalAlloc (GPTR, paths_size);
      
      this->status_ = PdhExpandCounterPath (wildcard_path,
                                            paths, 
                                            &paths_size);

      if (PDH_MORE_DATA == this->status_)
        {
          ++paths_size;
          GlobalFree (paths);
          paths = (LPSTR) GlobalAlloc (GPTR, paths_size);
      
          this->status_ = PdhExpandCounterPath (wildcard_path,
                                                paths, 
                                                &paths_size);
        }
        
      if (PDH_CSTATUS_VALID_DATA != this->status_)
        {
          ACE_ERROR ((LM_ERROR,
                      "%s: PdhExpandCounterPath failed\n",
                      wildcard_path));
        }
        
      LPSTR path = paths;
      int index = 0;
      
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
      
      while (this->instances_.dequeue_head (instance) == 0)
        {
          delete instance;
        }
    }

    void
    WindowsMultiInstanceMonitor::win_update (void)
    {
      for (INSTANCES_ITERATOR i (this->instances_); !i.done (); i.advance ())
        {
          i.next (this->current_instance_);
          (*this->current_instance_)->win_update ();
          this->value_.doubleValue +=
            (*this->current_instance_)->value_.doubleValue;
        }
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* defined (ACE_WIN32) */
