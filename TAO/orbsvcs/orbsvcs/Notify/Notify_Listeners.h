// $Id$
#ifndef TAO_NOTIFY_LISTENERS_H
#define TAO_NOTIFY_LISTENERS_H

#include "Notify_Types.h"

class TAO_Notify_Event_Listener
{
 public:
  virtual void dispatch_event (const CORBA::Any & data, CORBA::Environment &ACE_TRY_ENV) = 0;
  virtual void dispatch_event (const CosNotification::StructuredEvent & notification, CORBA::Environment &ACE_TRY_ENV) = 0;
};

class TAO_Notify_Update_Listener
{
 public:
  virtual void dispatch_update (EVENTTYPE_LIST& added, EVENTTYPE_LIST& removed) = 0;
};

#endif /* TAO_NOTIFY_LISTENERS_H */
