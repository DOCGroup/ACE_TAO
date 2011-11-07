// $Id$

#include "tao/ORB_Core_TSS_Resources.h"
#include "tao/ORB_Core.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ORB_Core_TSS_Resources::TAO_ORB_Core_TSS_Resources (void)
  : event_loop_thread_ (0)
  , client_leader_thread_ (0)
  , lane_ (0)
  , ts_objects_ ()
  , upcalls_temporarily_suspended_on_this_thread_ (false)
  , orb_core_ (0)
{
}

TAO_ORB_Core_TSS_Resources::~TAO_ORB_Core_TSS_Resources (void)
{
  this->fini ();
}

void
TAO_ORB_Core_TSS_Resources::fini (void)
{
  if (this->orb_core_ != 0)
    {
      this->orb_core_->tss_cleanup (this->ts_objects_);
    }
  this->orb_core_ = 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
