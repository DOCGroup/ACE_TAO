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
      ~Message_Queue_Monitor (void);
    
      /// Implemented by the most-derived class. Does the actual
      /// work of fetching the monitored value.
      virtual void update (void);
      
      /// Sets a referemce to the Statistic's parent in a group.
      virtual void group_parent (const Monitor_Base* parent);
      
      /// Adds a Statistic to the list of children in a group.
      virtual void add_group_child (const Monitor_Base* child);
      
      /// Attaches the (optional) constrain to this monitor point.
      virtual void constraint (const ETCL_Constraint* constraint);
      
      /// Updates the monitor's data if it is a numeric floating point.
      virtual void receive (const double data);
      
      /// Updates the monitor's data if it is an integer size.
      virtual void receive (const size_t data);
      
      /// Updates the monitor's data if it is textual.
      virtual void receive (const MonitorControl_Types::NameList& data);
      
      /// Reset function.
      virtual void clear (void);
    };
  }
}    

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif // MESSAGE_QUEUE_MONITOR_H
