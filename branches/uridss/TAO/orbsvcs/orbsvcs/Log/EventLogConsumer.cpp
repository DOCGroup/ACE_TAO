#include "orbsvcs/Log/EventLog_i.h"
#include "orbsvcs/Log/LogMgr_i.h"
#include "orbsvcs/Log/LogNotification.h"

ACE_RCSID (Log,
           EventLogConsumer,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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

  // @@ The current revision of the specification (formal/03-07-01)
  // states:
  //
  // * When a push operation is invoked and a log is full, then a
  //   NO_RESOURCE (sic) SystemException is raised with a LOGFULL
  //   minor code.
  //
  // * When a push operation is invoked on a log that is off-duty,
  //   then a NO_RESOURCE (sic) SystemException is raised with a
  //   LOGOFFDUTY minor code.
  //
  // * When a push operation is invoked on a log that is locked, then
  //   a NO_PERMISSIONS (sic) SystemException is raised with a LOGLOCKED
  //   minor code.
  //
  // * When a push operation is invoked on a log that is disabled,
  //   then a TRANSIENT SystemException is raised with a LOGDISABLED
  //   minor code.
  //
  // But neither the Telecom Logging or the CORBA specification define
  // the values for these minor codes.
  //
  // I have submitted a defect report to the OMG for clarification.
  //    --jtc
  ACE_TRY 
    {
      // log the RecordList.
      this->log_->write_recordlist (recList ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (DsLogAdmin::LogFull, ex)
    {
      ACE_THROW (CORBA::NO_RESOURCES ());
    }
  ACE_CATCH (DsLogAdmin::LogOffDuty, ex)
    {
      ACE_THROW (CORBA::NO_RESOURCES ());
    }
  ACE_CATCH (DsLogAdmin::LogLocked, ex)
    {
      ACE_THROW (CORBA::NO_PERMISSION ());
    }
  ACE_CATCH (DsLogAdmin::LogDisabled, ex)
    {
      ACE_THROW (CORBA::TRANSIENT ());
    }
  ACE_ENDTRY;
}

void
TAO_Event_LogConsumer::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((
                CORBA::SystemException
        ))
{
  this->supplier_proxy_->disconnect_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
}

TAO_END_VERSIONED_NAMESPACE_DECL
