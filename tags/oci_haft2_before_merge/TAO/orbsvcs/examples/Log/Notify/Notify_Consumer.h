/* -*- C++ -*- */

// ============================================================================
/**
 *  @file   Notify_Consumer.h
 *
 *  $Id$
 *
 *  An example of using the Notify_Logging_Service.
 *  The Notify_Consumer consumes log-generated events.
 *
 *
 *  @author D A Hanvey (d.hanvey@qub.ac.uk)
 */
// ============================================================================

#ifndef NOTIFY_CONSUMER_H
#define NOTIFY_CONSUMER_H

#include "orbsvcs/DsNotifyLogAdminS.h"
#include "orbsvcs/DsEventLogAdminC.h"
#include "orbsvcs/CosEventCommS.h"
#include "orbsvcs/CosNotifyCommS.h"
#include "orbsvcs/CosNamingC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Consumer : public POA_CosNotifyComm::PushConsumer, public PortableServer::RefCountServantBase
{
  // = TITLE
  //   Simple consumer object
  //
  // = DESCRIPTION
  //   This class is a consumer of log generated events.
  //
public:
  Consumer (void);
  // Constructor

  int run (int argc, char* argv[]);
  // Run the test

protected:

  CosNotifyChannelAdmin::ProxyID proxy_supplier_id_;
  // The proxy_supplier id.

  // = Methods

  // Destructor

  // = NotifyPublish method
    virtual void offer_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType
      ));

  // = StructuredPushSupplier methods
virtual void push (const CORBA::Any &event
                     ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventComm::Disconnected
                   ));


  virtual void disconnect_push_consumer (
        ACE_ENV_SINGLE_ARG_DECL
        )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

private:
  CORBA::ULong event_count_;
  // Keep track of the number of events received.

  // = Data Members
  CORBA::ORB_var orb_;
  // The ORB that we use.

  CosNaming::NamingContext_var naming_context_;
  // Handle to the name service.

  DsNotifyLogAdmin::NotifyLogFactory_var notify_log_factory_;

  CosNotifyChannelAdmin::ProxyPushSupplier_var proxy_supplier_;


};

#endif /* NOTIFY_CONSUMER_H */
