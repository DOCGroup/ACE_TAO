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

#include "ace/MonitorControl_Types.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_ENABLE_MONITORS)

#include "ace/Refcountable_T.h"
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
    class ACE_Export Monitor_Base
      : private ACE_Refcountable_T<ACE_SYNCH_MUTEX>
    {
    public:
      typedef MonitorControl_Types::ConstraintList CONSTRAINTS;
      typedef CONSTRAINTS::const_iterator CONSTRAINT_ITERATOR;

      /// Implemented by the most-derived class. Does the actual
      /// work of fetching the monitored value.
      virtual void update (void) = 0;

      /// Updates the monitor's data if it is a numeric floating point.
      virtual void receive (double value);

      /// Updates the monitor's data if it is an integer size.
      virtual void receive (size_t value);

      /// Add a constraint, returns the so called constraint id
      long add_constraint (const char* expression,
                           Control_Action* action = 0);

      /// Remove a constraint
      Control_Action* remove_constraint (const long constraint_id);

      /// Get all constraints
      CONSTRAINTS& constraints (void);

      /// Reset function.
      void clear (void);

      /// Data accessors.
      void retrieve (MonitorControl_Types::Data& data) const;
      void retrieve_and_clear (MonitorControl_Types::Data& data);

      /// Common to all monitors.
      void add_to_registry (const ACE_Time_Value& time = ACE_Time_Value::zero);
      void remove_from_registry (void);

      const char* name (void) const;
      void name (const char* new_name);

      /// Refcounting methods.
      void add_ref (void);
      void remove_ref (void);

    protected:
      Monitor_Base (void);
      Monitor_Base (const char* name);
      virtual ~Monitor_Base (void);
      virtual void clear_i (void);

    protected:
      MonitorControl_Types::Data data_;
      mutable ACE_SYNCH_MUTEX mutex_;

      CONSTRAINTS constraints_;

    private:
      ACE_CString name_;
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/Monitor_Base.inl"
#endif /* __ACE_INLINE__ */

#endif /* ACE_ENABLE_MONITORS */

#include /**/ "ace/post.h"

#endif // MONITOR_BASE_H
