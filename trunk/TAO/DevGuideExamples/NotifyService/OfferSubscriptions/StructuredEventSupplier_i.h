#ifndef _EVENTSUPPLIER_I_H_
#define _EVENTSUPPLIER_I_H_

#include <orbsvcs/CosNotifyChannelAdminS.h>

class StructuredEventSupplier_i : public POA_CosNotifyComm::StructuredPushSupplier
{
public:
    // Constructor
    StructuredEventSupplier_i(CORBA::ORB_ptr orb);
    virtual void disconnect_structured_push_supplier ()
    throw (CORBA::SystemException);        
    
    virtual void subscription_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed
    )
    throw(CORBA::SystemException, CosNotifyComm::InvalidEventType);       
private:
    CORBA::ORB_var orb_;
};

#endif 
