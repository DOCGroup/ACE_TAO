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

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "MonitorControl/LowLevelMonitors_T.h"
#include "MonitorControl/MonitorPoint.h"

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
     * @note This file contains a class declarations for a specialized monitor
     *       point we will want to have at the OS level. In the
     *       enabled ("true") versions, the virtual update() method
     *       is implemented with the appropriate behavior, while in the
     *       disabled ("false") versions, it is implemented as a no-op,
     *       so the compiler may optimize it away. The purpose of the
     *       class should be self-evident from its name.
     */

    /**
     * @class CPULoadMonitor
     *
     * @brief Template specialization with meaningful implementation of
     * pure virtual method.
     */
    template<>
    class MONITORCONTROL_Export CPULoadMonitor<true>
      : public MonitorPoint<true>
#if defined (ACE_WIN32)
      , public WindowsMonitor
#endif
    {
    public:
      CPULoadMonitor (void);
      
      /// Implementation of the pure virtual method.
      virtual void update (void);
      
    private:
#if defined (linux)
      void access_proc_stat (unsigned long *which_idle);
#endif

#if defined (ACE_HAS_KSTAT)
      void access_kstats (unsigned long *which_idle);
#endif
      
    private:
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

#include /**/ "ace/post.h"

#endif // CPULOADMONITOR_H
