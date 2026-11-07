// -*- C++ -*-

//=============================================================================
/**
 * @file Monitor_Group.h
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef MONITOR_GROUP_H
#define MONITOR_GROUP_H

#include /**/ "ace/pre.h"

#include "ace/Monitor_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

#include "ace/Monitor_Control/Monitor_Control_export.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace Monitor_Control
  {
    /**
     * @class Monitor_Group
     *
     * @brief Updates and/or clears a group of monitors with a single call.
     */
    class MONITOR_CONTROL_Export Monitor_Group : public Monitor_Base
    {
    public:
      explicit Monitor_Group (char const* name);

      void add_member (Monitor_Base* member);

      void update () override;

      /// These are overridden to be no-ops.
      void receive (double data) override;
      void receive (size_t data) override;
      void receive (Monitor_Control_Types::NameList const& data) override;

      void clear () override;

    private:
      using MEMBERS = ACE_Unbounded_Queue<Monitor_Base *>;
      using MEMBERS_ITERATOR = ACE_Unbounded_Queue_Const_Iterator<Monitor_Base *>;

      MEMBERS members_;
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

#include /**/ "ace/post.h"

#endif // MONITOR_GROUP_H
