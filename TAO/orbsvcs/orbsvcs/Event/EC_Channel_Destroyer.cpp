/**
 * @file EC_Channel_Destroyer.cpp
 *
 * @author Marina Spivak <marina@atdesk.com>
 */

#include "EC_Channel_Destroyer.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_EC_Channel_Destroyer_Functor::operator() (
                           TAO_EC_Event_Channel_Base * event_channel) throw ()
{
  try
    {
      event_channel->destroy ();
    }
  catch (...)
    {
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
