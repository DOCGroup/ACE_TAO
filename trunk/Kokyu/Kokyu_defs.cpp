// $Id$

#include "Kokyu_defs.h"

#if ! defined (__ACE_INLINE__)
#include "Kokyu_defs.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Kokyu, Kokyu_defs, "$Id$")

namespace Kokyu
{
   Dispatch_Command::~Dispatch_Command (void)
   {
   }

  Dispatcher_Attributes::Dispatcher_Attributes ()
    :immediate_activation_ (0)
  {
  }
}
