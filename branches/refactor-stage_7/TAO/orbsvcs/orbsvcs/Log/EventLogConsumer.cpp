#include "orbsvcs/Log/EventLog_i.h"
#include "orbsvcs/Log/LogMgr_i.h"
#include "orbsvcs/Log/LogNotification.h"


ACE_RCSID (Log,
           EventLogConsumer,
           "$Id$")


TAO_Event_LogConsumer::TAO_Event_LogConsumer (TAO_EventLog_i *log)
: log_ (log)
{  
}

TAO_Event_LogConsumer::~TAO_Event_LogConsumer (void)
{
}

void
TAO_Event_LogConsumer::connect (CosEventChannelAdmin::ConsumerAdmin_ptr consumer_admin)
{
  // Connect to the event channel.
  CosEventComm::PushConsumer_var myself = this->_this ();
  this->supplier_proxy_ = consumer_admin->obtain_push_supplier ();
  this->supplier_proxy_->connect_push_consumer (myself.in());
}

void
TAO_Event_LogConsumer::push (const CORBA::Any& data ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((
                CORBA::SystemException,
                CosEventComm::Disconnected
        ))
{
  // create a record list...
  DsLogAdmin::RecordList recList (1);
  recList.length (1);

  recList [0].info = data;

  // log the RecordList.
  this->log_->write_recordlist (recList ACE_ENV_ARG_PARAMETER);

  ACE_CHECK;
}

void
TAO_Event_LogConsumer::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((
                CORBA::SystemException
        ))
{
  this->supplier_proxy_->disconnect_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
}


