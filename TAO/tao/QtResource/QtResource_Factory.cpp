#include "tao/QtResource/QtResource_Factory.h"
#include "tao/debug.h"
#include "ace/QtReactor/QtReactor.h"
#include <cerrno>
#include <memory>
#include <new>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  QtResource_Factory::QtResource_Factory (QApplication * const qapp)
    : qapp_ (qapp)
  {
  }

  ACE_Reactor_Impl *
  QtResource_Factory::reactor_impl ()
  {
    // synchronized by external locks
    if (this->qapp_ == nullptr)
      {
        TAOLIB_ERROR ((LM_ERROR,
                       "TAO (%P|%t) - QApplication is undefined."
                       " Cannot create ACE_QtReactor\n"));
        return nullptr;
      }

    if (!this->reactor_impl_)
      {
        std::unique_ptr<ACE_QtReactor> reactor_impl {
          new (std::nothrow) ACE_QtReactor (this->qapp_)
        };
        if (reactor_impl == nullptr)
          {
            errno = ENOMEM;
            return nullptr;
          }

        if (TAO_debug_level > 0)
          {
            TAOLIB_DEBUG ((LM_DEBUG,
                           "TAO (%P|%t) - ACE_QtReactor created\n"));
          }

        this->reactor_impl_ = reactor_impl.release ();
      }

    return this->reactor_impl_;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
