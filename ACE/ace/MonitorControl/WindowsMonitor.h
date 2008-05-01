// -*- C++ -*-

//=============================================================================
/**
 * @file WindowsMonitor.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef WINDOWSMONITOR_H
#define WINDOWSMONITOR_H

#include /**/ "ace/pre.h"

#include "ace/MonitorControl/MonitorControl_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_WIN32)

/// Must include this after the ACE headers to avoid many redefinition
/// errors.
#include <pdh.h>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    /**
     * @class WindowsMonitor
     *
     * @brief Mixin class for concrete monitors compiled on Windows machines.
     */
    class MONITORCONTROL_Export WindowsMonitor
    {
      friend class WindowsMultiInstanceMonitor;

    protected:
      WindowsMonitor (const ACE_TCHAR *path);

      void win_update (void);

    protected:
      PDH_FMT_COUNTERVALUE value_;

    /// Make these persistent as members so they don't have to be
    /// created & destroyed with each update.
    private:
      HQUERY query_;
      HCOUNTER counter_;
      PDH_STATUS status_;
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* defined (ACE_WIN32) */

#endif // WINDOWSMONITOR_H
