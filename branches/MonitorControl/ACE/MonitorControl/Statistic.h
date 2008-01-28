// -*- C++ -*-

//=============================================================================
/**
 * @file Statistic.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef STATISTIC_H
#define STATISTIC_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Datatypes.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

class ETCL_Constraint;

namespace ACE
{
  namespace MonitorControl
  {
    /**
     * @class Statistic
     *
     * @brief Base class from which the template monitor point class is
     *        derived.
     *
     * All the operations are pure virtual so they can
     * be implemented in the 'disabled' template specialization
     * as no-ops.
     */
    class MONITORCONTROL_Export Statistic : public Generic
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
        virtual ~InvalidOperation (void);
      };
    
      /// Implemented by the most-derived class. Does the actual
      /// work of fetching the monitored value.
      virtual void update (void) = 0;
      
      /// Sets a referemce to the Statistic's parent in a group.
      virtual void group_parent (const Statistic* parent) = 0;
      
      /// Adds a Statistic to the list of children in a group.
      virtual void add_group_child (const Statistic* child) = 0;
      
      /// Attaches the (optional) constrain to this monitor point.
      virtual void constraint (const ETCL_Constraint* constraint) = 0;
      
      /// Updates the monitor's data if it is numeric.
      virtual void receive (const double data) = 0;
      
      /// Updates the monitor's data if it is textual.
      virtual void receive (const Datatypes::NameList& data) = 0;
      
      /// Reset function.
      virtual void clear (void) = 0;
      
    protected:
      Statistic (void);
      Statistic (const char* name);
      virtual ~Statistic (void);
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif // STATISTIC_H
