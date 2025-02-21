#include "ace/Monitor_Control/Memory_Usage_Monitor.h"

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace Monitor_Control
  {
    const char* Memory_Usage_Monitor::default_name_ =
      "OS/Memory/TotalUsage";

    Memory_Usage_Monitor::Memory_Usage_Monitor (const char* name)
      : Monitor_Base (name, Monitor_Control_Types::MC_NUMBER)
#if defined (ACE_HAS_WIN32_PDH)
      , Windows_Monitor (ACE_TEXT ("\\Memory\\% Committed Bytes In Use"))
#endif
    {
    }

    void
    Memory_Usage_Monitor::update ()
    {
#if defined (ACE_HAS_WIN32_PDH)
      this->update_i ();
      this->receive (this->value_);
#elif defined (ACE_HAS_LINUX_SYSINFO)
      if (::sysinfo (&this->sysinfo_) != 0)
        {
          ACELIB_ERROR ((LM_ERROR,
                      ACE_TEXT ("Memory usage - sysinfo() failed\n")));
          return;
        }

      double used_ram = this->sysinfo_.totalram - this->sysinfo_.freeram;
      double percent_mem_usage = used_ram / this->sysinfo_.totalram * 100.0;

      this->receive (percent_mem_usage);
#endif
    }

    const char*
    Memory_Usage_Monitor::default_name ()
    {
      return Memory_Usage_Monitor::default_name_;
    }

    void
    Memory_Usage_Monitor::clear_i ()
    {
#if defined (ACE_HAS_WIN32_PDH)
      this->clear_impl ();
#endif

      this->Monitor_Base::clear_i ();
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */
