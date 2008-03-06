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
#elif defined (linux)
      , start_ (0UL)
      , file_ptr_ (0)
#endif
    {
#if defined (linux)
      for (unsigned long i = 0UL; i < MAX_INTERFACES; ++i)
        {
          this->sent_array_[i] = 0UL;
        }
      
      /// Read the file once to get a base value that we can subtract
      /// from subsequent readings to get bytes sent since we started
      /// monitoring.  
      this->file_ptr_ = ACE_OS::fopen ("/proc/net/dev", "r");
      
      void* dummy = ACE_OS::fgets (buf_, sizeof (buf_), file_ptr_);
      dummy = ACE_OS::fgets (buf_, sizeof (buf_), file_ptr_);
      
      unsigned long iface_sent = 0UL;
      ACE_UINT32 iface_index = 0UL;

      while (ACE_OS::fgets (buf_, sizeof (buf_), file_ptr_) != 0)
        {
          sscanf (this->buf_,
                  " %*[^:]: %*u %*u %*u %*u %*u %*u %*u %*u %lu %*u",
                  &iface_sent);
          this->start_ += iface_sent;
          
          ++iface_index;
        }

      (void) ACE_OS::fclose (this->file_ptr_);
#endif
    }

    void
    BytesSentMonitor<true>::update (void)
    {
#if defined (ACE_WIN32)
      this->win_update ();

      /// Stores value and timestamp with thread-safety.
      this->receive (this->value_.doubleValue);
#elif defined (linux)
      this->file_ptr_ = ACE_OS::fopen ("/proc/net/dev", "r");
      
      if (this->file_ptr_ == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "bytes sent - opening /proc/net/dev failed\n"));
          return;
        }
        
      /// Throw away the first 2 lines of the file, which are file
      /// and column headers.  
      void* dummy = ACE_OS::fgets (buf_, sizeof (buf_), file_ptr_);
      dummy = ACE_OS::fgets (buf_, sizeof (buf_), file_ptr_);

      unsigned long iface_sent = 0UL;
      ACE_UINT32 iface_index = 0UL;
      ACE_UINT64 bytes_sent = 0UL;
          
      while (ACE_OS::fgets (buf_, sizeof (buf_), file_ptr_) != 0)
        {
          sscanf (this->buf_,
                  " %*[^:]: %*u %*u %*u %*u %*u %*u %*u %*u %lu %*u",
                  &iface_sent);
                  
          /// If the monitor runs long enough, the system counters will
          /// wrap around, the statement below is an attempt to correct
          /// for that.
          this->sent_array_[iface_index] +=
            iface_sent
            - static_cast<unsigned long> (this->sent_array_[iface_index]);
          bytes_sent += this->sent_array_[iface_index];
          
          ++iface_index;
        }

      this->receive (static_cast<double> (bytes_sent - this->start_));
        
      (void) ACE_OS::fclose (this->file_ptr_);
#endif
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

