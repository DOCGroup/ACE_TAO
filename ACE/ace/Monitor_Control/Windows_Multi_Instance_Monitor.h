// -*- C++ -*-

//=============================================================================
/**
 * @file Windows_Multi_Instance_Monitor.h
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef WINDOWS_MULTI_INSTANCE_MONITOR_H
#define WINDOWS_MULTI_INSTANCE_MONITOR_H

#include /**/ "ace/pre.h"

#include "ace/Unbounded_Queue.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Monitor_Control/Windows_Monitor.h"

#if defined (ACE_HAS_WIN32_PDH)

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace Monitor_Control
  {
    /**
     * @class Windows_Multi_Instance_Monitor
     *
     * @brief Mixin class for concrete monitors compiled on Windows machines
     *        where the Performance Data Helper object of interest has more
     *        than once instance, for example multiple CPUs or network cards.
     */
    class MONITOR_CONTROL_Export Windows_Multi_Instance_Monitor
    {
    public:
      Windows_Multi_Instance_Monitor (Windows_Multi_Instance_Monitor const &) = delete;
      Windows_Multi_Instance_Monitor (Windows_Multi_Instance_Monitor &&) = delete;
      Windows_Multi_Instance_Monitor &operator= (Windows_Multi_Instance_Monitor const &) = delete;
      Windows_Multi_Instance_Monitor &operator= (Windows_Multi_Instance_Monitor &&) = delete;

    protected:
      explicit Windows_Multi_Instance_Monitor (ACE_TCHAR const *wildcard_path);
      virtual ~Windows_Multi_Instance_Monitor ();

      /// Platform-specific implementation.
      void update_i ();

      /// Platform-specific reset.
      void clear_impl ();

    protected:
      double value_ {};

    private:
      // The queue stores raw pointers because ACE_Unbounded_Queue requires
      // copyable values; this class owns and deletes each monitor instance.
      using INSTANCES = ACE_Unbounded_Queue<Windows_Monitor *>;
      using INSTANCES_ITERATOR = ACE_Unbounded_Queue_Const_Iterator<Windows_Monitor *>;

      INSTANCES instances_ {};
      PDH_STATUS status_ {ERROR_SUCCESS};
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* defined (ACE_HAS_WIN32_PDH) */

#endif // WINDOWS_MULTI_INSTANCE_MONITOR_H
