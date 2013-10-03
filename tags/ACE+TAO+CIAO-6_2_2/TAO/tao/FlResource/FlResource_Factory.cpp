// -*- C++ -*-
// $Id$

#include "tao/FlResource/FlResource_Factory.h"
#include "tao/debug.h"
#include "ace/FlReactor/FlReactor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  FlResource_Factory::FlResource_Factory (void)
    : reactor_impl_( 0 )
  {
  }

  ACE_Reactor_Impl *
  FlResource_Factory::reactor_impl (void)
  {
    // synchronized by external locks
    if (!this->reactor_impl_)
    {
      ACE_NEW_RETURN (this->reactor_impl_,
                      ACE_FlReactor (),
                      0);

      if (TAO_debug_level > 0)
        TAOLIB_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - ACE_FlReactor created\n"));
    }

    return this->reactor_impl_;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
