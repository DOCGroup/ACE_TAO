//$Id$

#include "tao/FoxResource/FoxResource_Factory.h"
#include "tao/debug.h"
#include "ace/FoxReactor/FoxReactor.h"

ACE_RCSID( TAO_FoxResource,
           FoxResource_Factory,
           "$Id$");


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{

  FoxResource_Factory::FoxResource_Factory (FXApp *app)
    : reactor_impl_ (0)
    , fxapp_ (app)
  {
  }

  ACE_Reactor_Impl *
  FoxResource_Factory::reactor_impl (void)
  {
    if (this->fxapp_ == 0)
      return 0;

    // synchronized by external locks
    if (!this->reactor_impl_)
    {
      ACE_NEW_RETURN (this->reactor_impl_,
                      ACE_FoxReactor (),
                      0);

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - ACE_FoxReactor created\n"));
    }

    return this->reactor_impl_;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
