//$Id$

#include /**/ "XtResource_Factory.h"
#include /**/ "debug.h"
#include /**/ "ace/XtReactor.h"

ACE_RCSID(tao, XtResource_Factory, "$Id$");

namespace TAO
{

  XtResource_Factory::XtResource_Factory (XtAppContext p_context):
    reactor_impl_( 0 ),
    context_( p_context )
  {
  }

  ACE_Reactor_Impl *
  XtResource_Factory::reactor_impl (void)
  {
    if (this->context_ == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "TAO (%P|%t) - XtAppContext is undefined. Cannot create ACE_XtReactor \n"));
      return 0;
    }

    if (!this->reactor_impl_)
    {
      ACE_NEW_RETURN (this->reactor_impl_,
                      ACE_XtReactor (this->context_),
                      0);

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - ACE_XtReactor created \n"));
    }

    return this->reactor_impl_;
  }
}
