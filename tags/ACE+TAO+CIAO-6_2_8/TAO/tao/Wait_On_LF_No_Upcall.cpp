// $Id$

#include "tao/Wait_On_LF_No_Upcall.h"
#include "tao/Nested_Upcall_Guard.h"
#include "tao/Leader_Follower.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  Wait_On_LF_No_Upcall::Wait_On_LF_No_Upcall (TAO_Transport *t)
    : TAO_Wait_On_Leader_Follower (t)
  {
  }

  Wait_On_LF_No_Upcall::~Wait_On_LF_No_Upcall (void)
  {
  }

  int
  Wait_On_LF_No_Upcall::wait (ACE_Time_Value *max_wait_time,
                              TAO_Synch_Reply_Dispatcher &rd)
  {
    Nested_Upcall_Guard upcall_guard (this->transport_);

    return TAO_Wait_On_Leader_Follower::wait (max_wait_time, rd);
  }

  bool
  Wait_On_LF_No_Upcall::can_process_upcalls (void) const
  {
    if ((this->transport_->opened_as () == TAO::TAO_SERVER_ROLE) &&
        (this->transport_->bidirectional_flag () == -1))
      {
        TAO_ORB_Core_TSS_Resources *tss =
          this->transport_->orb_core ()->get_tss_resources ();

        if (tss->upcalls_temporarily_suspended_on_this_thread_)
          {
            return false;
          }
      }

    return true;
  }

  int
  Wait_On_LF_No_Upcall::defer_upcall (ACE_Event_Handler* eh)
  {
    if (TAO_debug_level > 6)
      TAOLIB_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Wait_On_LF_No_Upcall[%d]::defer_upcall, "
                  "deferring upcall on transport "
                  "because upcalls temporarily suspended on this thread\n",
                  this->transport_->id()));
    return this->transport_->orb_core ()->leader_follower ().defer_event (eh);
  }

}

TAO_END_VERSIONED_NAMESPACE_DECL
