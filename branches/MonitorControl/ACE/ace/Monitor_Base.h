// -*- C++ -*-

//=============================================================================
/**
 * @file Monitor_Base.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef MONITOR_BASE_H
#define MONITOR_BASE_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/MonitorControl_Types.h"
#include "ace/Thread_Mutex.h"
#include "ace/Synch_Traits.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

class ETCL_Constraint;

namespace ACE
{
  namespace MonitorControl
  {
    class Control_Action;

    /**
     * @class Monitor_Base
     *
     * @brief Base class from which the template monitor point class is
     *        derived.
     *
     * All the operations are pure virtual so they can
     * be implemented in the 'disabled' template specialization
     * as no-ops.
     */
    class ACE_Export Monitor_Base : public MC_Generic
    {
    public:
      typedef MonitorControl_Types::ConstraintList CONSTRAINTS;
      typedef CONSTRAINTS::Iterator CONSTRAINT_ITERATOR;
      
      /// Implemented by the most-derived class. Does the actual
      /// work of fetching the monitored value.
      virtual void update (void) = 0;

      /// Updates the monitor's data if it is a numeric floating point.
      virtual void receive (double value) = 0;

      /// Updates the monitor's data if it is an integer size.
      virtual void receive (size_t value) = 0;
        
      /// Accessors for the constraint list.
      void constraints (const char* expression,
                        Control_Action* action = 0);
      CONSTRAINTS& constraints (void);

      /// Reset function.
      virtual void clear (void) = 0;

      /// Data accessor.
      MonitorControl_Types::Data retrieve (void) const;

      MonitorControl_Types::Data retrieve_and_clear (void);

    protected:
      Monitor_Base (void);
      Monitor_Base (const char* name);
      virtual ~Monitor_Base (void);

    /// @@@ (JP) To keep overhead down for system monitors, we
    /// may want to remove these from here and duplicate them, in 
    /// MonitorPoint<true> and in a new class below this
    /// one and above the ACE-specific monitors.
    protected:
      MonitorControl_Types::Data data_;
      mutable ACE_SYNCH_MUTEX mutex_;
      
      CONSTRAINTS constraints_;
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif // MONITOR_BASE_H
