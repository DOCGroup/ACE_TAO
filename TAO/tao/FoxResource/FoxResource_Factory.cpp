#include "tao/FoxResource/FoxResource_Factory.h"
#include "tao/debug.h"
#include "ace/FoxReactor/FoxReactor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  FoxResource_Factory::FoxResource_Factory (FXApp *app)
    : fxapp_ (app)
  {
  }

  ACE_Reactor_Impl *
  FoxResource_Factory::reactor_impl ()
  {
    if (this->fxapp_ == nullptr)
      return nullptr;

    // synchronized by external locks
    if (!this->reactor_impl_)
    {
      ACE_NEW_RETURN (this->reactor_impl_,
                      ACE_FoxReactor (),
                      nullptr);

      if (TAO_debug_level > 0)
        TAOLIB_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - ACE_FoxReactor created\n"));
    }

    return this->reactor_impl_;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
