// $Id$

#include "orbsvcs/Log/RTEventLog_i.h"
#include "orbsvcs/Log/LogMgr_i.h"
#include "orbsvcs/Log/LogNotification.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "orbsvcs/Event/EC_Dispatching.h"
#include "orbsvcs/Event/EC_ConsumerAdmin.h"
#include "orbsvcs/Event/EC_SupplierAdmin.h"
#include "orbsvcs/Event/EC_Timeout_Generator.h"
#include "orbsvcs/Event/EC_ObserverStrategy.h"
#include "orbsvcs/Event/EC_ConsumerControl.h"
#include "orbsvcs/Event/EC_SupplierControl.h"
#include "ace/Dynamic_Service.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Rtec_LogConsumer::TAO_Rtec_LogConsumer (TAO_RTEventLog_i *log)
  : log_ (log)
{
}

TAO_Rtec_LogConsumer::~TAO_Rtec_LogConsumer (void)
{
}

void
TAO_Rtec_LogConsumer::connect (RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin)
{
  RtecEventComm::PushConsumer_var myself = this->_this ();
  this->supplier_proxy_ = consumer_admin->obtain_push_supplier ();

  ACE_ConsumerQOS_Factory qos;
  qos.start_disjunction_group (1);
  qos.insert_type (ACE_ES_EVENT_ANY,
                   0);
  this->supplier_proxy_->connect_push_consumer (myself.in(), qos.get_ConsumerQOS ());
}

void
TAO_Rtec_LogConsumer::push (const RtecEventComm::EventSet& events)
{
  // create a record list...
  DsLogAdmin::RecordList recList (1);
  recList.length (1);

  recList [0].info <<= events;

  this->log_->write_recordlist (recList);


}

void
TAO_Rtec_LogConsumer::disconnect_push_consumer (void)
{
  this->supplier_proxy_->disconnect_push_supplier ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
