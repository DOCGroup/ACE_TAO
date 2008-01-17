// -*- C++ -*-

//=============================================================================
/**
 * @file MonitorPointRegistry.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef MONITORPOINTREGISTRY_H
#define MONITORPOINTREGISTRY_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "MonitorControl/GenericRegistry.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

class Statistic;

namespace ACE
{
  namespace MonitorControl
  {
    /**
     * @class MonitorPointRegistry
     *
     * @brief Specializaton of GenericRegistry.
     *
     * Stores instances of the monitor point classes.
     */
    class MONITORCONTROL_Export MonitorPointRegistry
      : public GenericRegistry
    {
    public:
      /// Used to help ensure that there is only a single instance
      /// per process of MonitorPointRegistry.
      static MonitorPointRegistry* instance (void);
      
      MonitorPointRegistry (void);
      ~MonitorPointRegistry (void);
      
      /// The lookup operation.
      Statistic* get (const ACE_CString& name) const;
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif // MONITORPOINTREGISTRY_H
