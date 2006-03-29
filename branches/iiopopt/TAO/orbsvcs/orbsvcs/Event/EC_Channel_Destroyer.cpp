/**
 * @file EC_Channel_Destroyer.cpp
 *
 * $Id$
 *
 * @author Marina Spivak <marina@atdesk.com>
 */

#include "orbsvcs/Event/EC_Channel_Destroyer.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_EC_Channel_Destroyer_Functor::operator() (
                           TAO_EC_Event_Channel_Base * event_channel)
  ACE_THROW_SPEC (())
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      event_channel->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHALL
    {
    }
  ACE_ENDTRY;
}

TAO_END_VERSIONED_NAMESPACE_DECL
