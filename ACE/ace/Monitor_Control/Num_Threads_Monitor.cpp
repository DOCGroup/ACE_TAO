#include "ace/Monitor_Control/Num_Threads_Monitor.h"

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

#if defined (ACE_LINUX)
#include "ace/OS_NS_stdio.h"
#endif

#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"

#include <cstdio>
#include <cstring>
#include <memory>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace Monitor_Control
  {
    Num_Threads_Monitor::Num_Threads_Monitor (char const* name)
      : Monitor_Base (name, Monitor_Control_Types::MC_NUMBER)
#if defined (ACE_HAS_WIN32_PDH)
      , Windows_Monitor (ACE_TEXT ("\\System\\Threads"))
#endif
    {
    }

    void
    Num_Threads_Monitor::update ()
    {
#if defined (ACE_HAS_WIN32_PDH)
      this->update_i ();
      this->receive (this->value_);
#elif defined (ACE_LINUX)
      std::unique_ptr<FILE, int (*) (FILE *)> file_ptr (
        ACE_OS::fopen (ACE_TEXT ("/proc/self/status"), ACE_TEXT ("r")), ACE_OS::fclose);

      if (!file_ptr)
        {
          ACELIB_ERROR ((LM_ERROR,
                      ACE_TEXT ("Num threads - opening ")
                      ACE_TEXT ("/proc/self/status failed\n")));
          return;
        }

      char buf[1024] {};
      unsigned long nthreads {};
      while (std::fgets (buf, sizeof (buf), file_ptr.get ()) != nullptr)
        {
          char const * const item = std::strtok (buf, " \t\n");
          char const * const arg = std::strtok (nullptr, "\n");

          if (item == nullptr || arg == nullptr)
            {
              continue;
            }

          if (std::strcmp (item, "Threads:") == 0)
            {
              std::sscanf (arg, "%lu", &nthreads);
              break;
            }
        }

      /// Stores value and timestamp with thread-safety.
      this->receive (static_cast<double> (nthreads));
#endif
    }

    char const*
    Num_Threads_Monitor::default_name ()
    {
      return "OS/System/NumThreads";
    }

    void
    Num_Threads_Monitor::clear_i ()
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
