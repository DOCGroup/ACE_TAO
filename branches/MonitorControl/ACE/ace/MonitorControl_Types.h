// -*- C++ -*-

//=============================================================================
/**
 * @file MonitorControl_Types.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef MONITORCONTROL_TYPES_H
#define MONITORCONTROL_TYPES_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Vector_T.h"
#include "ace/Array_Map.h"
#include "ace/SString.h"
#include "ace/Time_Value.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    class Control_Action;

    struct ACE_Export MonitorControl_Types
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
        Constraint (const Constraint& rhs);
        ~Constraint (void);

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

#include /**/ "ace/post.h"

#endif // MONITORCONTROL_TYPES_H
