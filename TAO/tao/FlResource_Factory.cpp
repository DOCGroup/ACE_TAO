//$Id$

#include "FlResource_Factory.h"
#include "debug.h"
#include "ace/FlReactor.h"

ACE_RCSID( TAO_FlResource,
           FlResource_Factory,
           "$Id$");

namespace TAO
{

  FlResource_Factory::FlResource_Factory ():
    reactor_impl_( 0 )
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
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - ACE_FlReactor created \n"));
    }

    return this->reactor_impl_;
  }
}
