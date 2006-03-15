// $Id$

#include "orbsvcs/Event/ECG_ConsumerEC_Control.h"

ACE_RCSID(Event, ECG_ConsumerEventChannelControl, "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ECG_ConsumerEC_Control::TAO_ECG_ConsumerEC_Control (void)
{
}

TAO_ECG_ConsumerEC_Control::~TAO_ECG_ConsumerEC_Control (void)
{
}

int
TAO_ECG_ConsumerEC_Control::activate (void)
{
  return 0;
}

int
TAO_ECG_ConsumerEC_Control::shutdown (void)
{
  return 0;
}

void
TAO_ECG_ConsumerEC_Control::event_channel_not_exist (TAO_EC_Gateway_IIOP *
                                                     ACE_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_ECG_ConsumerEC_Control::system_exception (TAO_EC_Gateway_IIOP *,
                                              CORBA::SystemException &
                                              ACE_ENV_ARG_DECL_NOT_USED)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
