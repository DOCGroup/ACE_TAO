/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = TITLE
//   A simple test for the Event Channel.
//
// = DESCRIPTION
//   This test exercises the basic features of the EC, such as
//   filtering, correlation, timeouts, etc.
//   The test creates a colocated EC, some suppliers, some consumers
//   and exercises each feature, one at a time.
//
// ============================================================================

#ifndef EC_BASIC_H
#define EC_BASIC_H

#include "ace/SString.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/High_Res_Timer.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/Channel_Clients_T.h"

class ECB_Driver
{
  //
  // = TITLE
  //   A class to control the tests.
  //
  // = DESCRIPTION
  //   This class creates multiple tests and runs them in sequence,
  //   providing proper environment for each one (the event channel,
  //   the ORB configuration, etc.
  //
public:
  ECB_Driver (void);

  int run (int argc, char* argv[]);
  // Run the test.

private:
  int shutdown (CORBA::Environment&);
  // Called when the main thread is about to shutdown.

  int parse_args (int argc, char* argv[]);
  // parse the command line arguments

  void dump_results (void);
  // Dump the results to the standard output.

private:
  const char* pid_filename_;
  // The name of a file where the process stores its pid

  ACE_hrtime_t test_start_;
  ACE_hrtime_t test_stop_;
  // Measure the test elapsed time as well as mark the beginning of
  // the frames.

  CORBA::ORB_var orb_;
  // The ORB, so we can shut it down.
};

// ****************************************************************

class ECB_Test
{
  // = TITLE
  //   The base class for all the tests.
  //
  // = DESCRIPTION
  //   All the tests inherit from this class, it is used by the
  //   consumer and suppliers as callbacks.
  //
public:
  virtual ~ECB_Test (void);
  // Destructor

  virtual void push (int consumer_number,
                     const RtecEventComm::EventSet& events,
                     CORBA::Environment &_env) = 0;
  // The callback from the Consumer....
};

class ECB_Consumer : public POA_RtecEventComm::PushConsumer
{
public:
  // = TITLE
  //   The consumers for all the tests.
  //
  // = DESCRIPTION
  //   To simplify each test implementation a generic consumer class
  //   is provided.
  //   It dispatches the events back to a Base_Test.
  //
  ECB_Consumer (ECB_Test* test,
                int consumer_id);

  void open (const char* name,
             RtecEventChannelAdmin::EventChannel_ptr event_channel,
             RtecScheduler::Scheduler_ptr scheduler,
             CORBA::Environment& _env);
  // This method connects the consumer to the EC.

  void close (CORBA::Environment &_env);
  // Disconnect from the EC.

  RtecScheduler::handle_t rt_info (void) const;
  // The RT_Info for this object

  void connect (const RtecEventChannelAdmin::ConsumerQOS& qos,
                CORBA::Environment &_env);
  void disconnect (CORBA::Environment &_env);
  // Disconnect from the supplier, but do not forget about it or close
  // it.

  // = The POA_RtecEventComm::PushComsumer methods.
  virtual void push (const RtecEventComm::EventSet& events,
                     CORBA::Environment &_env);
  virtual void disconnect_push_consumer (CORBA::Environment &);

private:
  ECB_Test* test_;
  // To callback.

  int consumer_id_;
  // So we can give our identity back to the test.

  RtecScheduler::handle_t rt_info_;
  // The handle for our RT_Info description.

  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
  // We talk to the EC using this proxy.

  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin_;
  // We talk to the EC using this proxy.
};

class ECB_Supplier : public POA_RtecEventComm::PushSupplier
{
public:
  // = TITLE
  //   Implement one of the consumers in this test.
  //
  ECB_Supplier (ECB_Test* test,
                int supplier_id);
  // We generate an id based on the name....

  void open (const char* name,
             RtecEventChannelAdmin::EventChannel_ptr event_channel,
             RtecScheduler::Scheduler_ptr scheduler,
             CORBA::Environment& _env);
  // This method connects the supplier to the EC.

  void close (CORBA::Environment &_env);
  // Disconnect from the EC.

  RtecScheduler::handle_t rt_info (void) const;
  // The RT_Info for this object

  void connect (const RtecEventChannelAdmin::SupplierQOS& qos,
                CORBA::Environment &_env);
  void disconnect (CORBA::Environment &_env);
  // Disconnect from the EC, but do not forget about it or close
  // it.

  void send_event (RtecEventComm::EventSet& events,
                   CORBA::Environment &_env);
  // Send one event, the supplier provides the SourceID

  // = The POA_RtecEventComm::PushSupplier methods.
  virtual void disconnect_push_supplier (CORBA::Environment &);

private:
  ECB_Test* test_;
  // To callback.

  int supplier_id_;
  // This is NOT the supplier ID for the EC, just a number for the

  RtecScheduler::handle_t rt_info_;
  // The handle for our RT_Info description.

  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy_;
  // We talk to the EC using this proxy.

  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin_;
  // We talk to the EC using this proxy.
};

// ****************************************************************

// @@ TODO WE may need to split each test to its own file, but only
// once the number of tests justifies that change.

class ECB_SupplierID_Test : public ECB_Test
{
  //
  // = TITLE
  //   Verifies that multiple suppliers can have the same ID.
  //
  // = DESCRIPTION
  //   This class creates two consumers and two suppliers, the
  //   suppliers connect with the same ID, but send out different
  //   events. The consumers subscribe by supplier ID.
  //
  //   The test executes several phases (see the enum below).
  //
  // = TODO
  //   The test could be more flexible, but functionality is more
  //   important than flexiblitity.
  //
public:
  ECB_SupplierID_Test (void);

  enum
  {
    PHASE_0, // Verifies filtering for two suppliers with the same ID
    PHASE_1, // Disconnect one of the suppliers
    PHASE_2, // Reconnect the supplier
    PHASE_3, // Disconnect a consumer
    PHASE_4, // Reconnect the consumer
    PHASE_5, // Disconnect both consumers
    PHASE_6, // Reconnect both consumers
    PHASE_7, // Disconnect both suppliers and reconnect them
    PHASE_END // To check that no events are received after the test ends.
  };

  enum
  {
    EVENTS_SENT = 10, // How many events we send on each test
    SUPPLIER_ID = 1
  };

  void run (CORBA::ORB_ptr orb,
            RtecEventChannelAdmin::EventChannel_ptr event_channel,
            RtecScheduler::Scheduler_ptr scheduler,
            CORBA::Environment& _env);
  // Start the test.

  int dump_results (void);
  // Print out the results, returns -1 if an error was detected.

  virtual void push (int consumer_id,
                     const RtecEventComm::EventSet& events,
                     CORBA::Environment &_env);
  // The callback from the Consumer....


private:
  ECB_Consumer consumer0_;
  ECB_Consumer consumer1_;
  // The consumers...

  ECB_Supplier supplier0_;
  ECB_Supplier supplier1_;
  // The suppliers...

  CORBA::ULong event_count_[ECB_SupplierID_Test::PHASE_END + 1];
  // Count the correctly received events.

  CORBA::ULong error_count_[ECB_SupplierID_Test::PHASE_END + 1];
  // Store the number of errors for each phase.

  int phase_;
  // Keep track of the test we are running...
};

// ****************************************************************

class ECB_Correlation_Test : public ECB_Test
{
  //
  // = TITLE
  //   Verifies that correlation works.
  //
  // = DESCRIPTION
  //   This class creates one consumers and two suppliers, the
  //   consumer subscribe for several event correlations.
  //   The correlations are satisfied in several different ways by
  //   both suppliers (see below).by:
  //
  // = TODO
  //
public:
  ECB_Correlation_Test (void);

  enum
  {
    PHASE_0, // One supplier sends A the other sends B
    PHASE_1, // Both suppliers send event A and B (in a single event)
    PHASE_2, // Both suppliers alternate A and B
    PHASE_3, // One supplier sends A the other sends A and B (in a
             // single event)
    PHASE_4, // One supplier sends A the other alternates A and B
    PHASE_5, // Both suppliers send A
    PHASE_END // To check that no events are received after the test ends.
  };

  enum
  {
    EVENTS_SENT = 10, // How many events we send on each test
    EVENT_A = 100, // The event "A"
    EVENT_B = 200, // The event "B"
    SUPPLIER_ID_0 = 100, // The supplier ID for supplier0
    SUPPLIER_ID_1 = 200  // The supplier ID for supplier1
  };

  void run (CORBA::ORB_ptr orb,
            RtecEventChannelAdmin::EventChannel_ptr event_channel,
            RtecScheduler::Scheduler_ptr scheduler,
            CORBA::Environment& _env);
  // Start the test.

  int dump_results (void);
  // Print out the results, returns -1 if an error was detected.

  virtual void push (int consumer_id,
                     const RtecEventComm::EventSet& events,
                     CORBA::Environment &_env);
  // The callback from the Consumer....

private:
  ECB_Consumer consumer_;
  // The consumer...

  ECB_Supplier supplier0_;
  ECB_Supplier supplier1_;
  // The suppliers...

  CORBA::ULong event_count_[ECB_Correlation_Test::PHASE_END + 1];
  // Count the correctly received events.

  CORBA::ULong error_count_[ECB_Correlation_Test::PHASE_END + 1];
  // Store the number of errors for each phase.

  int phase_;
  // Keep track of the test we are running...
};

// ****************************************************************

#if defined (__ACE_INLINE__)
#include "EC_Basic.i"
#endif /* __ACE_INLINE__ */

#endif /* EC_BASIC_H */
