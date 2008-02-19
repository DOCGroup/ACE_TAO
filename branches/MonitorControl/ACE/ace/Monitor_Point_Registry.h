// -*- C++ -*-

//=============================================================================
/**
 * @file Monitor_Point_Registry.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef MONITOR_POINT_REGISTRY_H
#define MONITOR_POINT_REGISTRY_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/MC_Generic_Registry.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    class Monitor_Base;

    /**
     * @class Monitor_Point_Registry
     *
     * @brief Specializaton of MC_Generic_Registry.
     *
     * Stores instances of the monitor point classes.
     */
    class ACE_Export Monitor_Point_Registry
      : public MC_Generic_Registry
    {
    public:
      /// Used to help ensure that there is only a single instance
      /// per process of MonitorPointRegistry.
      static Monitor_Point_Registry* instance (void);

      /// The lookup operation.
      Monitor_Base* get (const ACE_CString& name) const;
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif // MONITOR_POINT_REGISTRY_H
