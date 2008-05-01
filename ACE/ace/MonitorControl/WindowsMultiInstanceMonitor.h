// -*- C++ -*-

//=============================================================================
/**
 * @file WindowsMultiInstanceMonitor.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef WINDOWSMULTIINSTANCEMONITOR_H
#define WINDOWSMULTIINSTANCEMONITOR_H

#include /**/ "ace/pre.h"

#include "ace/Unbounded_Queue.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "MonitorControl/WindowsMonitor.h"

#if defined (ACE_WIN32)

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    /**
     * @class WindowsMultiInstanceMonitor
     *
     * @brief Mixin class for concrete monitors compiled on Windows machines
     *        where the Performance Data Helper object of interest has more
     *        than once instance, for example multiple CPUs or network cards.
     */
    class MONITORCONTROL_Export WindowsMultiInstanceMonitor
    {
    protected:
      WindowsMultiInstanceMonitor (const ACE_TCHAR *wildcard_path);
      virtual ~WindowsMultiInstanceMonitor (void);

      /// Implementation of the pure virtual method.
      void win_update (void);

    protected:
      PDH_FMT_COUNTERVALUE value_;

    private:
      typedef ACE_Unbounded_Queue<WindowsMonitor *>
        INSTANCES;
      typedef ACE_Unbounded_Queue_Const_Iterator<WindowsMonitor *>
        INSTANCES_ITERATOR;

      INSTANCES instances_;
      DWORD n_instances_;
      PDH_STATUS status_;
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* defined (ACE_WIN32) */

#endif // WINDOWSMULTIINSTANCEMONITOR_H
