// $Id$

#include "tao/Wait_On_LF_No_Upcall.h"

#include "tao/Transport.h"
#include "tao/ORB_Core.h"
#include "debug.h"

ACE_RCSID(tao, Wait_On_LF_No_Upcall, "$Id$")

class TAO_Transport;

class TAO_Nested_Upcall_Guard
{
public:
  // Maybe we should instead just take in a ptr to
  // TAO_ORB_Core_TSS_Resources?  Or at least ORB_Core*?
  TAO_Nested_Upcall_Guard (TAO_Transport* t) : t_(t)
  {
    t_->orb_core()->get_tss_resources()->upcalls_temporarily_suspended_on_this_thread_ = true;
    if (TAO_debug_level > 6)
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Wait_On_LF_No_Upcall::wait "
                  "disabling upcalls on thread %t\n"));
  }

  ~TAO_Nested_Upcall_Guard ()
    {
    t_->orb_core()->get_tss_resources()->upcalls_temporarily_suspended_on_this_thread_ = false;
    if (TAO_debug_level > 6)
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Wait_On_LF_No_Upcall::wait "
                  "re-enabling upcalls on thread %t\n"));
    }

private:
  TAO_Nested_Upcall_Guard ();
  TAO_Nested_Upcall_Guard (const TAO_Nested_Upcall_Guard&);

  TAO_Transport* t_;
};

TAO_Wait_On_LF_No_Upcall::~TAO_Wait_On_LF_No_Upcall (void)
{
}

int
TAO_Wait_On_LF_No_Upcall::wait (ACE_Time_Value *max_wait_time,
                                TAO_Synch_Reply_Dispatcher &rd)
{
  TAO_Nested_Upcall_Guard upcall_guard(this->transport_);
  return base::wait (max_wait_time, rd);
}

