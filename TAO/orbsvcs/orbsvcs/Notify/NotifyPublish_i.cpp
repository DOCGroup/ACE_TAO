/* -*- C++ -*- $Id$ */
#include "orbsvcs/orbsvcs/Notify/NotifyPublish_i.h"

// Implementation skeleton constructor
TAO_NotifyPublish_i::TAO_NotifyPublish_i (void)
{
}

// Implementation skeleton destructor
TAO_NotifyPublish_i::~TAO_NotifyPublish_i (void)
{
}

void TAO_NotifyPublish_i::offer_change (
                                        const CosNotification::EventTypeSeq & /*added */,
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
