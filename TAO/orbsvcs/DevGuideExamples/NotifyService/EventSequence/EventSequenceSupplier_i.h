#ifndef _EVENTSUPPLIER_I_H_
#define _EVENTSUPPLIER_I_H_

#include "orbsvcs/CosNotifyChannelAdminS.h"

class EventSequenceSupplier_i : public POA_CosNotifyComm::SequencePushSupplier
{
public:
    // Constructor
    EventSequenceSupplier_i(CORBA::ORB_ptr orb);
    EventSequenceSupplier_i (EventSequenceSupplier_i const &) = delete;
    EventSequenceSupplier_i (EventSequenceSupplier_i &&) = delete;
    EventSequenceSupplier_i &operator= (EventSequenceSupplier_i const &) = delete;
    EventSequenceSupplier_i &operator= (EventSequenceSupplier_i &&) = delete;

    void disconnect_sequence_push_supplier () override;

    void subscription_change (CosNotification::EventTypeSeq const & added,
                              CosNotification::EventTypeSeq const & removed) override;
private:
    CORBA::ORB_var orb_;
};

#endif
