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
      MonitorPoint (const char* name,
                    MonitorControl_Types::InfoType type);
                    
      virtual ~MonitorPoint (void);
      
    public:  
      /// Sets a referemce to the Statistic's parent in a group.
      virtual void group_parent (const Monitor_Base* parent);
      
      /// Adds a Statistic to the list of children in a group.
      virtual void add_group_child (const Monitor_Base* child);
      
      /// Attaches the (optional) constrain to this monitor point.
      virtual void constraint (const ETCL_Constraint* constraint);
      
      /// Updates the monitor's data if it is numeric.
      virtual void receive (const double data);
      
      /// Reset function.
      virtual void clear (void);
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
