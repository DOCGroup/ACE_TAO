// -*- C++ -*-

//=============================================================================
/**
 * @file Message_Queue_Monitor.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef MESSAGE_QUEUE_MONITOR_H
#define MESSAGE_QUEUE_MONITOR_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Monitor_Base.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    class ACE_Export Message_Queue_Monitor : public Monitor_Base
    {
    public:
      Message_Queue_Monitor (void);
      Message_Queue_Monitor (const char* name);
      virtual ~Message_Queue_Monitor (void);

      /// Implemented by the most-derived class. Does the actual
      /// work of fetching the monitored value.
      virtual void update (void);

      /// Attaches an (optional) constraint to this monitor point.
      virtual void constraint (const char* constraint);
      
      /// Attaches an (optional) control action, to be triggered
      /// by satisfaction of the constraint, if any.
      virtual void control_action (Control_Action *action,
                                   const char* command = 0);

      /// Updates the monitor's data if it is a numeric floating point.
      virtual void receive (double value);

      /// Updates the monitor's data if it is an integer size.
      virtual void receive (size_t value);

      /// Reset function.
      virtual void clear (void);
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif // MESSAGE_QUEUE_MONITOR_H
