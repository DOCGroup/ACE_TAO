//$Id$

#include /**/ "TkResource_Factory.h"
#include /**/ "debug.h"
#include /**/ "ace/TkReactor.h"

ACE_RCSID(tao, TkResource_Factory, "$Id$");


namespace TAO
{

  TkResource_Factory::TkResource_Factory ():
    reactor_impl_( 0 )
  {
  }

  ACE_Reactor_Impl *
  TkResource_Factory::reactor_impl (void)
  {
    if (!this->reactor_impl_)
    {
      ACE_NEW_RETURN (this->reactor_impl_,
                      ACE_TkReactor (),
                      0);

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - ACE_TkReactor created \n"));
    }

    return this->reactor_impl_;
  }
}
