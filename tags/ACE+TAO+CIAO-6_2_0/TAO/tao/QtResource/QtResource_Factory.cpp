//$Id$

#include "tao/QtResource/QtResource_Factory.h"
#include "tao/debug.h"
#include "ace/QtReactor/QtReactor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  QtResource_Factory::QtResource_Factory (QApplication *qapp)
    : reactor_impl_ (0)
    , qapp_ (qapp)
  {
  }

  ACE_Reactor_Impl *
  QtResource_Factory::reactor_impl (void)
  {    // synchronized by external locks
    if (this->qapp_ == 0)
      {
        TAOLIB_ERROR ((LM_ERROR,
                    "TAO (%P|%t) - QApplication is undefined.",
                    " Cannot create ACE_QtReactor\n"));
        return 0;
      }

    if (!this->reactor_impl_)
      {
        ACE_NEW_RETURN (this->reactor_impl_,
                        ACE_QtReactor (qapp_),
                        0);

        if (TAO_debug_level > 0)
          TAOLIB_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - ACE_QtReactor created\n"));
      }

    return this->reactor_impl_;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
