#include "tao/FoxResource/FoxResource_Factory.h"
#include "tao/debug.h"
#include "ace/FoxReactor/FoxReactor.h"
#include <cerrno>
#include <memory>
#include <new>

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
      {
        return nullptr;
      }

    // synchronized by external locks
    if (this->reactor_impl_ == nullptr)
      {
        std::unique_ptr<ACE_FoxReactor> reactor_impl {
          new (std::nothrow) ACE_FoxReactor (this->fxapp_)
        };
        if (reactor_impl == nullptr)
          {
            errno = ENOMEM;
            return nullptr;
          }

        if (TAO_debug_level > 0)
          {
            TAOLIB_DEBUG ((LM_DEBUG,
                           "TAO (%P|%t) - ACE_FoxReactor created\n"));
          }

        this->reactor_impl_ = reactor_impl.release ();
      }

    return this->reactor_impl_;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
