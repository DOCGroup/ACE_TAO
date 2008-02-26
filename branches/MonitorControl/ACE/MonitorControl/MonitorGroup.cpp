// $Id$

#include "MonitorControl/MonitorControl_export.h"
#include "MonitorControl/MonitorGroup_T.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    template<>
    void MONITORCONTROL_Export
    MonitorGroup<false>::add_member (Monitor_Base* /* member */)
    {
    }

    template<>
    void MONITORCONTROL_Export
    MonitorGroup<true>::add_member (Monitor_Base* member)
    {
      this->members_.enqueue_tail (member);
    }
    
    template<>
    void MONITORCONTROL_Export
    MonitorGroup<false>::update (void)
    {
    }
    
    template<>
    void MONITORCONTROL_Export
    MonitorGroup<true>::update (void)
    {
      for (MEMBERS_ITERATOR i (this->members_); !i.done (); i.advance ())
        {
          i.next (this->current_member_);
          (*this->current_member_)->update ();
        }
    }
        
    template<>
    void MONITORCONTROL_Export
    MonitorGroup<false>::clear (void)
    {
    }
    
    template<>
    void MONITORCONTROL_Export
    MonitorGroup<true>::clear (void)
    {
      for (MEMBERS_ITERATOR i (this->members_); !i.done (); i.advance ())
        {
          i.next (this->current_member_);
          (*this->current_member_)->clear ();
        }
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

