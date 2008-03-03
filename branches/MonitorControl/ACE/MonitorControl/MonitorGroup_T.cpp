// $Id$

#ifndef MONITORGROUP_T_CPP
#define MONITORGROUP_T_CPP

#include "MonitorControl/MonitorGroup_T.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    template<>
    void
    MonitorGroup<false>::add_member (Monitor_Base* /* member */)
    {
    }

    template<>
    void
    MonitorGroup<true>::add_member (Monitor_Base* member)
    {
      this->members_.enqueue_tail (member);
    }

    template<>
    void
    MonitorGroup<false>::update (void)
    {
    }

    template<>
    void
    MonitorGroup<true>::update (void)
    {
      for (MEMBERS_ITERATOR i (this->members_); !i.done (); i.advance ())
        {
          Monitor_Base **member;
          i.next (member);
          (*member)->update ();
        }
    }

    template<>
    void
    MonitorGroup<false>::clear (void)
    {
    }

    template<>
    void
    MonitorGroup<true>::clear (void)
    {
      for (MEMBERS_ITERATOR i (this->members_); !i.done (); i.advance ())
        {
          Monitor_Base **member;
          i.next (member);
          (*member)->clear ();
        }
    }
    template<bool ENABLED>
    MonitorGroup<ENABLED>::MonitorGroup (const char* name)
      : Monitor_Base (name)
    {}

    template<bool ENABLED>
    void
    MonitorGroup<ENABLED>::constraint (
      const char* /* constraint */)
    {
    }

    template<bool ENABLED>
    void
    MonitorGroup<ENABLED>::control_action (
      Control_Action* /* action */,
      const char* /* command */)
    {
    }

    template<bool ENABLED>
    void
    MonitorGroup<ENABLED>::receive (double /* data */)
    {
    }

    template<bool ENABLED>
    void
    MonitorGroup<ENABLED>::receive (size_t /* data */)
    {
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif // MONITORGROUP_T_CPP

