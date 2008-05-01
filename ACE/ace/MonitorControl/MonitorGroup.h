// -*- C++ -*-

//=============================================================================
/**
 * @file MonitorGroup.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef MONITORGROUP_T_H
#define MONITORGROUP_T_H

#include /**/ "ace/pre.h"

#include "ace/Monitor_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

#include "ace/MonitorControl/MonitorControl_export.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    /**
     * @class MonitorGroup
     *
     * @brief Updates and/or clears a group of monitors iwth a single call.
     */
    class MONITORCONTROL_Export MonitorGroup : public Monitor_Base
    {
    public:
      MonitorGroup (const char* name);

      void add_member (Monitor_Base* member);

      virtual void update (void);

      virtual void clear (void);

    private:
      typedef ACE_Unbounded_Queue<Monitor_Base *>
        MEMBERS;
      typedef ACE_Unbounded_Queue_Const_Iterator<Monitor_Base *>
        MEMBERS_ITERATOR;

      MEMBERS members_;
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

#include /**/ "ace/post.h"

#endif // MONITORGROUP_T_H
