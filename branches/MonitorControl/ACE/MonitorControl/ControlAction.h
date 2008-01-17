// -*- C++ -*-

//=============================================================================
/**
 * @file ControlAction.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef CONTROLACTION_H
#define CONTROLACTION_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "MonitorControl/Datatypes.h"

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
    class MONITORCONTROL_Export ControlAction : public Generic
    {
    public:
      /// To be implemented by the concrete derived class.
      virtual void execute (const char* command) = 0;
      
    protected:
      ControlAction (void);
      ControlAction (const char* name);
      virtual ~ControlAction (void);
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif // CONTROLACTION_H
