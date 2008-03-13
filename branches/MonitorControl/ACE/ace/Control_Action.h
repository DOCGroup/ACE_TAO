// -*- C++ -*-

//=============================================================================
/**
 * @file Control_Action.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef CONTROL_ACTION_H
#define CONTROL_ACTION_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/MonitorControl_Types.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    /**
     * @class ControlAction
     *
     * @brief Base class for control actions initiated by the
     * application or by constraint evaluation trigger.
     *
     * This class is extended by the application developer or
     * by the MC service itself to create a concrete class.
     */
    /**
     * @note No need to have enabled/disabled template
     *       specializations as with MonitorPoint, since
     *       for a disabled monitor point, the associated
     *       control action (if any) won't be instantiated.
     */
    class ACE_Export Control_Action : public MC_Generic
    {
    public:
      /// To be implemented by the concrete derived class.
      virtual void execute (const char* command = "") = 0;
      
    protected:
      Control_Action (void);
      Control_Action (const char* name);
      virtual ~Control_Action (void);
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif // CONTROL_ACTION_H
