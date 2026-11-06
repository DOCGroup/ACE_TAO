#include "ace/Monitor_Control/Windows_Multi_Instance_Monitor.h"

#if defined (ACE_HAS_WIN32_PDH)

#include "ace/Log_Category.h"
#include "ace/SString.h"
#include "ace/os_include/os_pdhmsg.h"
#include <memory>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace Monitor_Control
  {
    Windows_Multi_Instance_Monitor::Windows_Multi_Instance_Monitor (
      ACE_TCHAR const *wildcard_path)
    {
      /// Create a string which is a concatentation of the path
      /// name of each 'instance' we need to monitor.

      DWORD paths_size = 4096;
      std::unique_ptr<void, void (*) (void *)> paths (
        GlobalAlloc (GPTR, paths_size),
        [] (void *memory)
        {
          if (memory != nullptr)
            GlobalFree (static_cast<HGLOBAL> (memory));
        });

      if (paths == nullptr)
        {
          ACELIB_ERROR ((LM_ERROR, ACE_TEXT ("GlobalAlloc failed\n")));
          return;
        }

      ACE_LPSTR path_buffer = static_cast<ACE_LPSTR> (paths.get ());
      this->status_ = ACE_TEXT_PdhExpandCounterPath (wildcard_path,
                                                     path_buffer,
                                                     &paths_size);

      if (PDH_MORE_DATA == static_cast<DWORD> (this->status_))
        {
          ++paths_size;
          paths.reset (GlobalAlloc (GPTR, paths_size));

          if (paths == nullptr)
            {
              ACELIB_ERROR ((LM_ERROR, ACE_TEXT ("GlobalAlloc failed\n")));
              return;
            }

          path_buffer = static_cast<ACE_LPSTR> (paths.get ());
          this->status_ = ACE_TEXT_PdhExpandCounterPath (wildcard_path,
                                                         path_buffer,
                                                         &paths_size);
        }

      if (PDH_CSTATUS_VALID_DATA != static_cast<DWORD> (this->status_))
        {
          ACELIB_ERROR ((LM_ERROR,
                      ACE_TEXT ("%s: PdhExpandCounterPath failed\n"),
                      wildcard_path));
        }

      ACE_LPSTR path = path_buffer;

      /// Create a regular Windows monitor for each path name.
      while (*path != ACE_TEXT ('\0'))
        {
          std::unique_ptr<Windows_Monitor> instance {
            new Windows_Monitor (path)
          };

          if (this->instances_.enqueue_tail (instance.get ()) == -1)
            ACELIB_ERROR ((LM_ERROR,
                           ACE_TEXT ("Failed to queue Windows monitor instance\n")));
          else
            instance.release ();

          path += ACE_OS::strlen (path) + 1;
        }
    }

    Windows_Multi_Instance_Monitor::~Windows_Multi_Instance_Monitor ()
    {
      Windows_Monitor *instance {};

      /// Destroy the single instance monitors created in the constructor.
      while (this->instances_.dequeue_head (instance) == 0)
        {
          std::unique_ptr<Windows_Monitor> instance_owner {instance};
        }
    }

    void
    Windows_Multi_Instance_Monitor::update_i ()
    {
      Windows_Monitor **current_instance {};

      /// Sum the values of each single instance monitor.
      for (INSTANCES_ITERATOR i (this->instances_); !i.done (); i.advance ())
        {
          i.next (current_instance);

          (*current_instance)->update_i ();

          this->value_ += (*current_instance)->value_;
        }
    }

    void
    Windows_Multi_Instance_Monitor::clear_impl ()
    {
      Windows_Monitor **current_instance {};

      /// Sum the values of each single instance monitor.
      for (INSTANCES_ITERATOR i (this->instances_); !i.done (); i.advance ())
        {
          i.next (current_instance);

          (*current_instance)->clear_impl ();
        }
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* defined (ACE_HAS_WIN32_PDH) */
