// -*- C++ -*-

//=============================================================================
/**
 * @file CPULoadMonitor.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef CPULOADMONITOR_H
#define CPULOADMONITOR_H

#include /**/ "ace/pre.h"

#include "ace/Monitor_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

#include "MonitorControl/MonitorControl_export.h"

#if defined (ACE_WIN32)
#include "MonitorControl/WindowsMonitor.h"
#elif defined (ACE_HAS_KSTAT)
#include <kstat.h>
#endif

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    /**
     * @class CPULoadMonitor
     *
     * @brief Monitors percentage CPU utilization.
     */
    class MONITORCONTROL_Export CPULoadMonitor
      : public Monitor_Base
#if defined (ACE_WIN32)
      , public WindowsMonitor
#endif
    {
    public:
      CPULoadMonitor (const char* name);

      /// Implementation of the pure virtual method.
      virtual void update (void);

      /// Stores the default name, used if none is supplied by the user.
      static const char* default_name (void);

    private:
#if defined (linux)
      void access_proc_stat (unsigned long *which_idle);
#endif

#if defined (ACE_HAS_KSTAT)
      void access_kstats (unsigned long *which_idle);
#endif

    private:
      static const char* default_name_;

      /// Common to Linux and Solaris implementations.
#if defined (linux) || defined (ACE_HAS_KSTAT)
      unsigned long user_;
      unsigned long wait_;
      unsigned long kernel_;
      unsigned long idle_;
      unsigned long prev_idle_;
      double prev_total_;
#endif
#if defined (linux)
      FILE *file_ptr_;
      char buf_[1024];
#elif defined (ACE_HAS_KSTAT)
      kstat_ctl_t *kstats_;
      kstat_t *kstat_;
      kid_t kstat_id_;
#endif
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

#include /**/ "ace/post.h"

#endif // CPULOADMONITOR_H
