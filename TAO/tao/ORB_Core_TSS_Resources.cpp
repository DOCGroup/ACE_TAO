#include "tao/ORB_Core_TSS_Resources.h"
#include "tao/ORB_Core.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ORB_Core_TSS_Resources::TAO_ORB_Core_TSS_Resources ()
  : event_loop_thread_ (0)
  , client_leader_thread_ (0)
  , lane_ (nullptr)
  , ts_objects_ ()
  , upcalls_temporarily_suspended_on_this_thread_ (false)
  , orb_core_ (nullptr)
{
}

TAO_ORB_Core_TSS_Resources::~TAO_ORB_Core_TSS_Resources ()
{
  this->fini ();
}

void
TAO_ORB_Core_TSS_Resources::fini ()
{
  if (this->orb_core_ != nullptr)
    {
      this->orb_core_->tss_cleanup (this->ts_objects_);
    }
  this->orb_core_ = nullptr;
}

TAO_END_VERSIONED_NAMESPACE_DECL
