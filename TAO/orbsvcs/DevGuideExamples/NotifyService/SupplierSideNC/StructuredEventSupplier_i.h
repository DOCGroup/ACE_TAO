#ifndef _EVENTSUPPLIER_I_H_
#define _EVENTSUPPLIER_I_H_

#include "orbsvcs/CosNotifyChannelAdminS.h"

class StructuredEventSupplier_i : public POA_CosNotifyComm::StructuredPushSupplier
{
public:
    StructuredEventSupplier_i (CORBA::ORB_ptr orb);

    StructuredEventSupplier_i (StructuredEventSupplier_i const &) = delete;
    StructuredEventSupplier_i (StructuredEventSupplier_i &&) = delete;
    StructuredEventSupplier_i &operator= (StructuredEventSupplier_i const &) = delete;
    StructuredEventSupplier_i &operator= (StructuredEventSupplier_i &&) = delete;

    void disconnect_structured_push_supplier () override;

    void subscription_change (CosNotification::EventTypeSeq const & added,
                              CosNotification::EventTypeSeq const & removed) override;

private:
    CORBA::ORB_var orb_;
};

#endif
