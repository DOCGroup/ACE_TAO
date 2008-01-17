// -*- C++ -*-

//=============================================================================
/**
 * @file ControlActionRegistry.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef CONTROLACTIONREGISTRY_H
#define CONTROLACTIONREGISTRY_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "MonitorControl/GenericRegistry.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

class ControlAction;

namespace ACE
{
  namespace MonitorControl
  {
    /**
     * @class ControlActionRegistry
     *
     * @brief Specializaton of GenericRegistry.
     *
     * Stores instances of the control actions.
     */
    class MONITORCONTROL_Export ControlActionRegistry
      : public GenericRegistry
    {
    public:
      /// Used to help ensure that there is only a single instance
      /// per process of ControlRegistry.
      static ControlActionRegistry* instance (void);
      
      ControlActionRegistry (void);
      ~ControlActionRegistry (void);
      
      /// The lookup operation.
      ControlAction* get (const ACE_CString& name) const;
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif // CONTROLACTIONREGISTRY_H
