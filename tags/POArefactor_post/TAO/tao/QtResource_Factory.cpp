//$Id$

#include "QtResource_Factory.h"
#include "debug.h"
#include "ace/QtReactor.h"

ACE_RCSID( TAO_QtResource,
           QtResource_Factory,
           "$Id$");

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
        ACE_ERROR ((LM_ERROR,
                    "TAO (%P|%t) - QApplication is undefined.",
                    " Cannot create ACE_XtReactor \n"));
        return 0;
      }

    if (!this->reactor_impl_)
      {

        ACE_NEW_RETURN (this->reactor_impl_,
                        ACE_QtReactor (qapp_),
                        0);

        if (TAO_debug_level > 0)
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - ACE_QtReactor created\n"));
      }

    return this->reactor_impl_;
  }
}
