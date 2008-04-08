// -*- C++ -*-

//=============================================================================
/**
 * @file MonitorPoint_T.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef MONITORPOINT_T_H
#define MONITORPOINT_T_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Monitor_Base.h"

#include "MonitorControl/MonitorControl_export.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    /**
     * @class MonitorPoint
     *
     * @brief Template class from which concrete monitor point classes are
     *        derived.
     *
     * The template parameter will be specified by a preprocessor
     * flag passed in at compile time to enable/disable monitors.
     */
    template<bool ENABLED>
    class MonitorPoint : public Monitor_Base
    {
    protected:
      MonitorPoint (const char* name);
      virtual ~MonitorPoint (void);

    public:
      /// Attaches an (optional) constraint to this monitor point.
      virtual void constraint (const char* constraint);

      /// Attaches an (optional) control action, to be triggered
      /// by satisfaction of the constraint, if any.
      virtual void control_action (Control_Action *action,
                                   const char* command = 0);
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "MonitorControl/MonitorPoint_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("MonitorPoint_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif // MONITORPOINT_T_H
