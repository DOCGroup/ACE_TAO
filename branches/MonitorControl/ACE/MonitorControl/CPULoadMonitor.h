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
/// Must include this after the ACE headers to avoid many redefinition
/// errors.
#include <pdh.h>
#elif defined (linux)
#elif defined (sun)
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
    {
    public:
      CPULoadMonitor (void);
      
      /// Implementation of the pure virtual method.
      virtual void update (void);
      
      /// Not implemented in base class to keep down the size of
      /// disabled monitors.
      virtual void receive (double data);
      
    private:
      /// Common to Linux and Solaris implementations.
      ACE_UINT64 user_;
      ACE_UINT64 nice_;
      ACE_UINT64 kernel_;
      ACE_UINT64 idle_;
      ACE_UINT64 prev_idle_;
      ACE_UINT64 prev_total_;
#if defined (ACE_WIN32)
      HQUERY query_;
      HCOUNTER counter_;
      PDH_STATUS status_;
      PDH_FMT_COUNTERVALUE value_;
#elif defined (linux)
      FILE *file_ptr_;
      char buf_[1024];
#elif defined (sun)
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
