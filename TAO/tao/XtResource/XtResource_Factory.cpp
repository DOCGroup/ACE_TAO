#include "tao/XtResource/XtResource_Factory.h"
#include "tao/debug.h"
#include "ace/XtReactor/XtReactor.h"
#include <memory>
#include <new>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  XtResource_Factory::XtResource_Factory (XtAppContext const p_context)
    : context_ (p_context)
  {
  }

  ACE_Reactor_Impl *
  XtResource_Factory::reactor_impl ()
  {
    // synchronized by external locks
    if (!this->context_)
      {
        TAOLIB_ERROR ((LM_ERROR,
                       ACE_TEXT ("TAO (%P|%t) - XtAppContext is undefined.")
                       ACE_TEXT (" Cannot create ACE_XtReactor\n")));
        return nullptr;
      }

    if (!this->reactor_impl_)
      {
        std::unique_ptr<ACE_XtReactor> reactor_impl {
          new (std::nothrow) ACE_XtReactor (this->context_)
        };
        if (reactor_impl == nullptr)
          return nullptr;

        if (TAO_debug_level > 0)
          TAOLIB_DEBUG ((LM_DEBUG,
                         "TAO (%P|%t) - ACE_XtReactor created\n"));

        this->reactor_impl_ = reactor_impl.get ();
        reactor_impl.release ();
      }

    return this->reactor_impl_;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
