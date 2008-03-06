// $Id$

#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"

#include "MonitorControl/LinuxNetworkInterfaceMonitor.h"

#if defined (linux)

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    LinuxNetworkInterfaceMonitor::LinuxNetworkInterfaceMonitor (
      const char *scan_format)
      : value_ (0UL),
        start_ (0UL),
        scan_format_ (scan_format)
    {
      for (unsigned long i = 0UL; i < MAX_INTERFACES; ++i)
        {
          this->value_array_[i] = 0UL;
        }
      
      /// Read the file once to get a base value that we can subtract
      /// from subsequent readings to get bytes sent since we started
      /// monitoring.  
      char buf[1024];
      FILE* fp = ACE_OS::fopen ("/proc/net/dev", "r");
      
      if (fp == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "bytes sent - opening /proc/net/dev failed\n"));
          return;
        }
          
      // TODO - something if dummy comes up 0.        
      void* dummy = ACE_OS::fgets (buf, sizeof (buf), fp);
      dummy = ACE_OS::fgets (buf, sizeof (buf), fp);
      
      unsigned long iface_value = 0UL;
      ACE_UINT32 iface_index = 0UL;

      while (ACE_OS::fgets (buf, sizeof (buf), fp) != 0)
        {
          sscanf (buf, this->scan_format_.c_str (), &iface_value);
          this->start_ += iface_value;
          
          ++iface_index;
        }

      (void) ACE_OS::fclose (fp);
    }

    void
    LinuxNetworkInterfaceMonitor::lin_update (void)
    {
      char buf[1024];
      FILE* fp = ACE_OS::fopen ("/proc/net/dev", "r");
      
      if (fp == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "bytes sent - opening /proc/net/dev failed\n"));
          return;
        }
        
      /// Throw away the first 2 lines of the file, which are file
      /// and column headers.  
      void* dummy = ACE_OS::fgets (buf, sizeof (buf), fp);
      dummy = ACE_OS::fgets (buf, sizeof (buf), fp);

      unsigned long iface_value = 0UL;
      ACE_UINT32 iface_index = 0UL;
      ACE_UINT64 total_value = 0UL;
          
      while (ACE_OS::fgets (buf, sizeof (buf), fp) != 0)
        {
          sscanf (buf, this->scan_format_.c_str (), &iface_value);
                  
          /// If the monitor runs long enough, the system counters will
          /// wrap around, the statement below is an attempt to correct
          /// for that.
          this->value_array_[iface_index] +=
            iface_value
            - static_cast<unsigned long> (this->value_array_[iface_index]);
          total_value += this->value_array_[iface_index];
          
          ++iface_index;
        }

      this->value_ = total_value - this->start_;       
      (void) ACE_OS::fclose (fp);
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* defined (linux) */
