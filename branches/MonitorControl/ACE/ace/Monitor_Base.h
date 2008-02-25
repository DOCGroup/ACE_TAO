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
      /**
       * @class InvalidOperation
       *
       * @brief Exception class, thrown by several operations
       *        in class Statistic.
       *
       * Used when an operation specific to a
       * umeric data type is called on a monitor point that
       * contains text data, or vice versa.
       */
      class InvalidOperation
      {
      public:
        InvalidOperation (void);
        ~InvalidOperation (void);
      };
    
      /// Implemented by the most-derived class. Does the actual
      /// work of fetching the monitored value.
      virtual void update (void) = 0;
      
      /// Attaches the (optional) constrain to this monitor point.
      virtual void constraint (const ETCL_Constraint* constraint) = 0;
      
      /// Updates the monitor's data if it is a numeric floating point.
      virtual void receive (double value) = 0;
      
      /// Updates the monitor's data if it is an integer size.
      virtual void receive (size_t value) = 0;
      
      /// Reset function.
      virtual void clear (void) = 0;
      
      /// Data accessor.
      MonitorControl_Types::Data retrieve (void);
      
    protected:
      Monitor_Base (void);
      Monitor_Base (const char* name);
      virtual ~Monitor_Base (void);
      
    protected:
      MonitorControl_Types::Data data_;
      ACE_SYNCH_MUTEX mutex_;
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif // MONITOR_BASE_H
