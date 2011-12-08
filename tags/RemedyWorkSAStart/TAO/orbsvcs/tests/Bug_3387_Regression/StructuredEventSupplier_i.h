// -*- C++ -*-
// $Id$
#ifndef _EVENTSUPPLIER_I_H_
#define _EVENTSUPPLIER_I_H_

#include "orbsvcs/CosNotifyCommS.h"

class StructuredEventSupplier_i :
  public virtual POA_CosNotifyComm::StructuredPushSupplier
{
public:
    // Constructor
    StructuredEventSupplier_i(CORBA::ORB_ptr orb);
    virtual void disconnect_structured_push_supplier ();


    virtual void subscription_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed
                                      );

private:
    CORBA::ORB_var orb_;
};

#endif
