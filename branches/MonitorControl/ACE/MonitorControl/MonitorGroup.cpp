// $Id$

#include "MonitorControl/MonitorGroup.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    MonitorGroup::MonitorGroup (const char* name)
      : Monitor_Base (name)
    {}

    void
    MonitorGroup::add_member (Monitor_Base* member)
    {
      this->members_.enqueue_tail (member);
    }

    void
    MonitorGroup::update (void)
    {
      for (MEMBERS_ITERATOR i (this->members_); !i.done (); i.advance ())
        {
          Monitor_Base **member;
          i.next (member);
          (*member)->update ();
        }
    }

    void
    MonitorGroup::clear (void)
    {
      for (MEMBERS_ITERATOR i (this->members_); !i.done (); i.advance ())
        {
          Monitor_Base **member;
          i.next (member);
          (*member)->clear ();
        }
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

