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
      struct Constraint
      {
        ACE_CString expr;
        Control_Action* control_action;
      };

#if defined (__BORLANDC__) && (__BORLANDC__ <= 0x570)
      // Borland C++ Builder 6 and earlier don't handle the second template
      // argument correctly. We have to pass it explicitly
      typedef ACE_Vector<Data, ACE_VECTOR_DEFAULT_SIZE> DataList;
      typedef ACE_Vector<ACE_CString, ACE_VECTOR_DEFAULT_SIZE> NameList;
      typedef ACE_Vector<Constraint, ACE_VECTOR_DEFAULT_SIZE> ConstraintList;
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
      typedef ACE_Vector<Constraint> ConstraintList;
#endif
    };

    /**
     * @class MC_Generic
     *
     * @brief Base class of Monitor_Base and ControlAction.
     */
    /// Allows us to combine
    /// operations of the respective registries into a common base class.
    class ACE_Export MC_Generic
    {
    public:
      const char* name (void) const;
      void name (const char* new_name);

    protected:
      MC_Generic (void);
      MC_Generic (const char* name);
      virtual ~MC_Generic (void);

    private:
      ACE_CString name_;
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif // MONITORCONTROL_TYPES_H
