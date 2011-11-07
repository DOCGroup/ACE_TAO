//$Id$

#include "tao/TkResource/TkResource_Factory.h"
#include "tao/debug.h"
#include "ace/TkReactor/TkReactor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{

  TkResource_Factory::TkResource_Factory (void)
    : reactor_impl_ (0)
  {
  }

  ACE_Reactor_Impl *
  TkResource_Factory::reactor_impl (void)
  {
    // synchronized by external locks
    if (!this->reactor_impl_)
    {
      ACE_NEW_RETURN (this->reactor_impl_,
                      ACE_TkReactor (),
                      0);

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - ACE_TkReactor created\n"));
    }

    return this->reactor_impl_;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
