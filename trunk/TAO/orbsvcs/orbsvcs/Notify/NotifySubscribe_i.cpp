/* -*- C++ -*- $Id$ */
#include "orbsvcs/orbsvcs/Notify/NotifySubscribe_i.h"

// Implementation skeleton constructor
TAO_NotifySubscribe_i::TAO_NotifySubscribe_i (void)
{
}

// Implementation skeleton destructor
TAO_NotifySubscribe_i::~TAO_NotifySubscribe_i (void)
{
}

void TAO_NotifySubscribe_i::subscription_change (
                                                 const CosNotification::EventTypeSeq & /*added*/,
                                                 const CosNotification::EventTypeSeq & /*removed*/,
                                                 CORBA::Environment & //ACE_TRY_ENV
                                                 )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyComm::InvalidEventType
  ))
{
  //Add your implementation here
}
