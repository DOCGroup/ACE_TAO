//$Id$

#include "Pluggable_Messaging_Utils.h"
#include "ORB_Core.h"

#if !defined (__ACE_INLINE__)
#include "tao/Pluggable_Messaging_Utils.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           Pluggable_Messaging_Utils,
           "$Id$")


TAO_Pluggable_Reply_Params::TAO_Pluggable_Reply_Params (
    TAO_Transport *t
  )
  : input_cdr_ (0)
  ,  transport_ (t)
{
}
