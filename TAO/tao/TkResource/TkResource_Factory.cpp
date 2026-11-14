#include "tao/TkResource/TkResource_Factory.h"
#include "tao/debug.h"
#include "ace/TkReactor/TkReactor.h"
#include <memory>
#include <new>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  ACE_Reactor_Impl *
  TkResource_Factory::reactor_impl ()
  {
    // synchronized by external locks
    if (this->reactor_impl_ == nullptr)
      {
        std::unique_ptr<ACE_TkReactor> reactor_impl {
          new (std::nothrow) ACE_TkReactor ()
        };
        if (reactor_impl == nullptr)
          return nullptr;

        if (TAO_debug_level > 0)
          TAOLIB_DEBUG ((LM_DEBUG,
                         "TAO (%P|%t) - ACE_TkReactor created\n"));

        this->reactor_impl_ = reactor_impl.get ();
        reactor_impl.release ();
      }

    return this->reactor_impl_;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
