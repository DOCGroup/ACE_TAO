// $Id$

#include "EC_Event_Channel.h"
#include "EC_Default_Factory.h"
#include "ace/Dynamic_Service.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Event_Channel.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Event_Channel, "$Id$")

TAO_EC_Event_Channel::
TAO_EC_Event_Channel (const TAO_EC_Event_Channel_Attributes& attr,
                      TAO_EC_Factory* factory,
                      int own_factory)
  : TAO_EC_Event_Channel_Base (attr, factory, own_factory)
{
  if (this->factory () == 0)
    {
      this->factory (
             ACE_Dynamic_Service<TAO_EC_Factory>::instance ("EC_Factory"),
             0);

      if (this->factory () == 0)
        {
          TAO_EC_Factory *f = 0;
          ACE_NEW (f,
                   TAO_EC_Default_Factory);
          this->factory (f, 1);
        }
    }
  this->scheduler_ =
    CORBA::Object::_duplicate (attr.scheduler);

  this->create_strategies ();
}
