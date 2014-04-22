// -*- C++ -*-

//=============================================================================
/**
 *  @file    Nested_Upcall_Guard.h
 *
 *  $Id$
 *
 *  @author  Chris Cleeland <cleeland@ociweb.com>
 */
//=============================================================================


#ifndef TAO_NESTED_UPCALL_GUARD_H
#define TAO_NESTED_UPCALL_GUARD_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Transport.h"
#include "tao/ORB_Core.h"
#include "tao/ORB_Core_TSS_Resources.h"
#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL
namespace TAO
{
  /**
   * @class Nested_Upcall_Guard
   *
   * @brief: Magic class that sets the status of the thread in the
   * TSS.
   *
   * This class was originally internal to the Wait_On_LF_NoUpcall
   * implementation, but now it needs to be shared with the LF
   * Connect wait as well.
   */
  class Nested_Upcall_Guard
  {
  public:
    // Maybe we should instead just take in a ptr to
    // TAO_ORB_Core_TSS_Resources?  Or at least ORB_Core*?
    explicit Nested_Upcall_Guard (TAO_Transport *t, bool enable=true)
      : t_ (t),
        enable_ (enable)
    {
      if (!this->enable_)
        {
          return;
        }

      TAO_ORB_Core_TSS_Resources *tss =
        t_->orb_core ()->get_tss_resources ();

      tss->upcalls_temporarily_suspended_on_this_thread_ = true;

      if (TAO_debug_level > 6)
        TAOLIB_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - Wait_On_LF_No_Upcall[%d]::wait, "
                    "disabling upcalls\n", t->id ()));
    }

    ~Nested_Upcall_Guard (void)
    {
      if (!this->enable_)
        {
          return;
        }

      TAO_ORB_Core_TSS_Resources *tss =
        this->t_->orb_core ()->get_tss_resources ();

      tss->upcalls_temporarily_suspended_on_this_thread_ = false;

      if (TAO_debug_level > 6)
        {
          TAOLIB_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - Wait_On_LF_No_Upcall[%d]::wait, "
                      "re-enabling upcalls\n", this->t_->id ()));
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

    /// A flag to support conditional waiting in the LF_Connect_Strategy
    bool enable_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_NESTED_UPCALL_GUARD_H */
