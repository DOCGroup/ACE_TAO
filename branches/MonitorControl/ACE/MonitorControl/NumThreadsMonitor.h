// -*- C++ -*-

//=============================================================================
/**
 * @file NumThreadsMonitor.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef NUMTHREADSMONITOR_H
#define NUMTHREADSMONITOR_H

#include /**/ "ace/pre.h"

#include "ace/Monitor_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_MONITOR_FRAMEWORK)

#if defined (ACE_WIN32)
#include "MonitorControl/WindowsMonitor.h"
#endif

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    /**
     * @class NumThreadsMonitor
     *
     * @brief Template specialization with meaningful implementation of
     * pure virtual method.
     */
    class MONITORCONTROL_Export NumThreadsMonitor
      : public Monitor_Base
#if defined (ACE_WIN32)
      , public WindowsMonitor
#endif
    {
    public:
      NumThreadsMonitor (const char* name);

      /// Implementation of the pure virtual method.
      virtual void update (void);

      /// Stores the default name, used if none is supplied to constructor.
      static const char* default_name (void);

    private:
      static const char* default_name_;

#if defined (linux)
      FILE *file_ptr_;
      char buf_[1024];
      unsigned long nthreads_;
#endif
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_MONITOR_FRAMEWORK*/

#include /**/ "ace/post.h"

#endif // NUMTHREADSMONITOR_H
