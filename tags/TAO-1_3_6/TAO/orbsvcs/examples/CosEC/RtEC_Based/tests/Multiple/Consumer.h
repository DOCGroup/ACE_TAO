/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = FILENAME
//   Consumer.h
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// = DESCRIPTION
//   Defines a simple Push Consumer.
//
// ============================================================================

#ifndef COSECCONSUMER_H_
#define COSECCONSUMER_H_

#include "orbsvcs/CosEventCommC.h"
#include "orbsvcs/CosEventChannelAdminC.h"
#include "orbsvcs/CosEventCommS.h"
#include "orbsvcs/CosEventChannelAdminS.h"
#include "Multiple.h"

class Consumer : public POA_CosEventComm::PushConsumer, public Multiple
{
  // = TITLE
  //   Consumer
  //
  // = DESCRIPTION
  //   The Consumer is a simple PushConsumer that connects to the
  //   CosEC and receives events from it.

public:
  // = Initializatiopn and termination methods.
  Consumer ();
  // Constructor.

  int init_Consumer (void);
  // Initialize the Consumer.

  void open (CosEventChannelAdmin::EventChannel_ptr event_channel
             ACE_ENV_ARG_DECL);
  // This method connects the consumer to the EC.

  void close (ACE_ENV_SINGLE_ARG_DECL);
  // Disconnect from the EC.

  void connect (ACE_ENV_SINGLE_ARG_DECL);
  // Connect the Consumer to the EventChannel.

  void disconnect (ACE_ENV_SINGLE_ARG_DECL);
  // Disconnect from the supplier, but do not forget about it or close
  // it.

  virtual void push (const CORBA::Any &data
                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosEventComm::Disconnected
      ));
  // push the event to the consumer.

  virtual void disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
  // disconnect the consumer from the EC.

  // = CosEC_Multiple methods.
  virtual int parse_args (int argc, char *argv []);
  // Parse the command line options.

private:
  CosEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
  // We talk to the EC using this proxy.

  CosEventChannelAdmin::ConsumerAdmin_var consumer_admin_;
  // We talk to the EC using this proxy.

  int event_count_;
  // The number of Events to receive before switching off.
};

#endif /* COSECCONSUMER_H_ */
