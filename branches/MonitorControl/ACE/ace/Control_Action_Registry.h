// -*- C++ -*-

//=============================================================================
/**
 * @file Control_Action_Registry.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef CONTROL_ACTION_REGISTRY_H
#define CONTROL_ACTION_REGISTRY_H

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
    class Control_Action;

    /**
     * @class Control_Action_Registry
     *
     * @brief Specializaton of MC_Generic_Registry.
     *
     * Stores instances of the control actions.
     */
    class ACE_Export Control_Action_Registry
      : public MC_Generic_Registry
    {
    public:
      /// Used to help ensure that there is only a single instance
      /// per process of ControlRegistry.
      static Control_Action_Registry* instance (void);
      
      Control_Action_Registry (void);
      ~Control_Action_Registry (void);
      
      /// The lookup operation.
      Control_Action* get (const ACE_CString& name) const;
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif // CONTROL_ACTION_REGISTRY_H
