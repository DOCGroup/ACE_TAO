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

#ifndef MONITORGROUP_H
#define MONITORGROUP_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "MonitorControl/MonitorControl_export.h"
#include "MonitorControl/MonitorGroup_T.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    template<>
    class MONITORCONTROL_Export MonitorGroup<true> : public Monitor_Base
    {
    public:
      void add_member (Monitor_Base* member);
      
      virtual void update (void);
      
      virtual void constraint (const ETCL_Constraint* constraint);
      
      virtual void clear (void);
      
    private:
      typedef ACE_Unbounded_Queue<Monitor_Base *>
        MEMBERS;
      typedef ACE_Unbounded_Queue_Const_Iterator<Monitor_Base *>
        MEMBERS_ITERATOR;
        
      MEMBERS members_;
      Monitor_Base **current_member_;
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif // MONITORGROUP_H
