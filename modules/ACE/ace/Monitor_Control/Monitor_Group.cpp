// $Id$

#include "ace/Monitor_Control/Monitor_Group.h"

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace Monitor_Control
  {
    Monitor_Group::Monitor_Group (const char* name)
      : Monitor_Base (name)
    {}

    void
    Monitor_Group::add_member (Monitor_Base* member)
    {
      this->members_.enqueue_tail (member);
    }

    void
    Monitor_Group::update (void)
    {
      for (MEMBERS_ITERATOR i (this->members_); !i.done (); i.advance ())
        {
          Monitor_Base **member;
          i.next (member);
          (*member)->update ();
        }
    }

    void
    Monitor_Group::clear (void)
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

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

