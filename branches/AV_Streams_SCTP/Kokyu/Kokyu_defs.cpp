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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Array<Kokyu::ConfigInfo>;
template class ACE_Array_Base<Kokyu::ConfigInfo>;
template class ACE_Array_Iterator<Kokyu::ConfigInfo>;
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Array<Kokyu::ConfigInfo>
#pragma instantiate ACE_Array_Base<Kokyu::ConfigInfo>
#pragma instantiate ACE_Array_Iterator<Kokyu::ConfigInfo>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
