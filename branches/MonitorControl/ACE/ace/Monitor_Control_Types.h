// -*- C++ -*-

//=============================================================================
/**
 * @file Monitor_Control_Types.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef MONITOR_CONTROL_TYPES_H
#define MONITOR_CONTROL_TYPES_H

#include /**/ "ace/pre.h"

#include "ace/Vector_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

#include "ace/Array_Map.h"
#include "ace/SString.h"
#include "ace/Time_Value.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    class Control_Action;

    struct ACE_Export Monitor_Control_Types
    {
      /**
       * @brief An instance is contained by each enabled monitor point.
       */
      struct ACE_Export Data
      {
        Data (void);
        ACE_Time_Value timestamp_;
        double value_;
      };

      /**
       * @brief Bundles the constrain string with its associated
       *        trigger action.
       */
      struct ACE_Export Constraint
      {
        Constraint (void);
        ~Constraint (void);

        /// Implemented explicitly so reference counting of control
        /// actions can be managed.
        Constraint (const Constraint& rhs);
        Constraint& operator= (const Constraint& rhs);

        ACE_CString expr;
        Control_Action* control_action;
      };

#if defined (__BORLANDC__) && (__BORLANDC__ <= 0x570)
      // Borland C++ Builder 6 and earlier don't handle the second template
      // argument correctly. We have to pass it explicitly
      typedef ACE_Vector<Data, ACE_VECTOR_DEFAULT_SIZE> DataList;
      typedef ACE_Vector<ACE_CString, ACE_VECTOR_DEFAULT_SIZE> NameList;
      typedef ACE_Array_Map<long, Constraint, std::equal_to<long> > ConstraintList;
#else
      /**
       * @brief Used by the admin class as a container for the data from
       *        a group of monitor points.
       */
      typedef ACE_Vector<Data> DataList;

      /**
       * @brief Used in various places to pass around a set of string names.
       */
      typedef ACE_Vector<ACE_CString> NameList;

      /**
       * @brief Holder for a monitor point's constraints.
       */
      typedef ACE_Array_Map<long, Constraint> ConstraintList;
#endif
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

#include /**/ "ace/post.h"

#endif // MONITOR_CONTROL_TYPES_H
