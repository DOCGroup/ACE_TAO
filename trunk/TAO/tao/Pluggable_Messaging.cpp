// $Id$

#include "tao/Pluggable_Messaging.h"
#include "tao/Pluggable.h"

#if !defined (__ACE_INLINE__)
# include "tao/Pluggable_Messaging.i"
#endif /* __ACE_INLINE__ */


ACE_RCSID(tao, Pluggable_Messaging, "$Id$")

TAO_Pluggable_Messaging::~TAO_Pluggable_Messaging (void)
{

}

int
TAO_Pluggable_Messaging::more_messages (void)
{
  ACE_NOTSUP_RETURN (-1);
}
