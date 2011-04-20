// $Id$

#include "tao/Wait_On_LF_No_Upcall.h"

#include "tao/Transport.h"
#include "tao/ORB_Core.h"
#include "tao/ORB_Core_TSS_Resources.h"
#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Transport;

namespace TAO
{
  /**
   * @class Nested_Upcall_Guard
   *
   * @brief: Magic class that sets the status of the thread in the
   * TSS.
   *
   */
  class Nested_Upcall_Guard
  {
  public:
    // Maybe we should instead just take in a ptr to
    // TAO_ORB_Core_TSS_Resources?  Or at least ORB_Core*?
    explicit Nested_Upcall_Guard (TAO_Transport *t)
      : t_ (t)
    {
      TAO_ORB_Core_TSS_Resources *tss =
        t_->orb_core ()->get_tss_resources ();

      tss->upcalls_temporarily_suspended_on_this_thread_ = true;

      if (TAO_debug_level > 6)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - Wait_On_LF_No_Upcall::wait, "
                    "disabling upcalls\n"));
    }

    ~Nested_Upcall_Guard (void)
    {
      TAO_ORB_Core_TSS_Resources *tss = t_->orb_core ()->get_tss_resources ();

      tss->upcalls_temporarily_suspended_on_this_thread_ = false;

      if (TAO_debug_level > 6)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - Wait_On_LF_No_Upcall::wait, "
                      "re-enabling upcalls\n"));
        }
    }

  private:
    Nested_Upcall_Guard (void);

    /// Disallow copying and assignment.
    Nested_Upcall_Guard (const Nested_Upcall_Guard&);
    Nested_Upcall_Guard &operator= (const Nested_Upcall_Guard&);

  private:
    /// Pointer to the transport that we plan to use.
    TAO_Transport *t_;
  };

  //=================================================================

  Wait_On_LF_No_Upcall::Wait_On_LF_No_Upcall (TAO_Transport *t)
    : base (t)
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

    return base::wait (max_wait_time, rd);
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
}

TAO_END_VERSIONED_NAMESPACE_DECL
