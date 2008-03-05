// $Id$

#include "MonitorControl/BytesSentMonitor.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    BytesSentMonitor<true>::BytesSentMonitor (void)
      : MonitorPoint<true> ("BytesSent")
#if defined (ACE_WIN32)
      , WindowsMultiInstanceMonitor (
          "\\Network Interface(*)\\Bytes Sent/sec")
#endif
    {}

    void
    BytesSentMonitor<true>::update (void)
    {
#if defined (ACE_WIN32)
      this->win_update ();

      /// Stores value and timestamp with thread-safety.
      this->receive (this->value_.doubleValue);
#endif
    }

//#if defined (linux)
    void
    BytesSentMonitor<true>::access_proc_stat (void)
    {
      this->file_ptr_ = ACE_OS::fopen ("/proc/net/dev", "r");
      
      if (this->file_ptr_ == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "bytes sent - opening /proc/net/dev failed\n"));
          return;
        }
        
      void* dummy = ACE_OS::fgets (buf_, sizeof (buf_), file_ptr_);
          ACE_DEBUG ((LM_DEBUG, "1st dummy: %s\n", buf_));
      dummy = ACE_OS::fgets (buf_, sizeof (buf_), file_ptr_);
          ACE_DEBUG ((LM_DEBUG, "2nd dummy: %s\n", buf_));
      
      while (ACE_OS::fgets (buf_, sizeof (buf_), file_ptr_) != 0)
        {
          ACE_DEBUG ((LM_DEBUG, "%s\n", buf_));
        }
    }
//#endif
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

