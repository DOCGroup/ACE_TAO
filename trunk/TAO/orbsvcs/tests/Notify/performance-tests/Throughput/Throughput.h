/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = FILENAME
//   Throughput.h
//
// = DESCRIPTION
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef NOTIFY_Throughput_H
#define NOTIFY_Throughput_H

#include "Notify_Test_Client.h"
#include "Notify_StructuredPushConsumer.h"
#include "Notify_StructuredPushSupplier.h"
#include "ace/Task.h"
#include "ace/Stats.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class Worker : public ACE_Task_Base
{
  // = TITLE
  //   Run a server thread
  //
  // = DESCRIPTION
  //   Use the ACE_Task_Base class to run server threads
  //
public:
  Worker (void);
  // ctor

  void orb (CORBA::ORB_ptr orb);

  virtual int svc (void);
  // The thread entry point.

  int done_;

private:
  CORBA::ORB_var orb_;
  // The orb
};

/***************************************************************************/

class Notify_Throughput;

class Throughput_StructuredPushConsumer 
  : public TAO_Notify_StructuredPushConsumer
{
public:
  Throughput_StructuredPushConsumer (Notify_Throughput *test_client);
  // Contructor.

  // = StructuredPushSupplier methods
  virtual void push_structured_event (
        const CosNotification::StructuredEvent & notification
        TAO_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosEventComm::Disconnected
       ));

  void accumulate_into (ACE_Throughput_Stats &throughput) const;
  // Accumulate the throughput statistics into <throughput>

  void dump_stats (const char* msg, ACE_UINT32 gsf);
  // Accumulate the throughput statistics into <throughput>

protected:
  Notify_Throughput * test_client_;

  TAO_SYNCH_MUTEX lock_;
  // Protect internal state

  ACE_hrtime_t throughput_start_;
  // The timestamp for the first message received

  ACE_Throughput_Stats throughput_;
  // Used for reporting stats

  int push_count_;
  // The number of push() calls
};

/***************************************************************************/

class Throughput_StructuredPushSupplier 
  : public TAO_Notify_StructuredPushSupplier, 
    public ACE_Task_Base
{
public:
  Throughput_StructuredPushSupplier (Notify_Throughput * test_client);
  // Constructor.

  virtual ~Throughput_StructuredPushSupplier ();
  // Destructor.

  // = The ACE_Task_Base methods....
  virtual int svc (void);

  void accumulate_into (ACE_Throughput_Stats &throughput) const;
  // Accumulate the throughput statistics into <throughput>

  void dump_stats (const char* msg, ACE_UINT32 gsf);
  // Accumulate the throughput statistics into <throughput>

protected:
  Notify_Throughput* test_client_;

  int push_count_;
  // Count the number of push() calls

  ACE_hrtime_t throughput_start_;
  // The time for the first event sent

  ACE_Throughput_Stats throughput_;
  // Measure the elapsed time spent while sending the events.
};

/***************************************************************************/

class Notify_Throughput : public Notify_Test_Client
{
public:
  // Initialization and termination code
  Notify_Throughput (void);
  virtual ~Notify_Throughput ();

  int parse_args(int argc, char *argv[]) ;

  int init (int argc, char *argv [] TAO_ENV_ARG_DECL);
  // initialization.

  void run_test (TAO_ENV_SINGLE_ARG_DECL);
  // Run the test.

  void peer_done (void);
  // Peers call this when done.

  void dump_results (void);
  // check if we got the expected results.

  Worker worker_;
protected:
  void create_EC (TAO_ENV_SINGLE_ARG_DECL);
  // Create participants.

  CORBA::Boolean colocated_ec_;
  // is the ec colocated.

  int burst_count_;
  // How many bursts we will send from each supplier.

  int burst_pause_;
  // The time between each event burst, in microseconds.

  int burst_size_;
  // Number of events to send per supplier in every burst

  int payload_size_;
  // data size to transmit.

  char *payload_;
  // the payload.

  int consumer_count_;
  // Consumer count

  int supplier_count_;
  // Supplier count

  int perconsumer_count_;
  // Number of events received that each consumer expects to see.

  ACE_CString ec_name_;
  // The name of the EC to resolve.

  CosNotifyChannelAdmin::EventChannel_var ec_;
  // The one channel that we create using the factory.

  CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin_;
  // The consumer admin used by consumers.

  CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin_;
  // The supplier admin used by suppliers.

  Throughput_StructuredPushConsumer** consumers_;
  // Consumers

  Throughput_StructuredPushSupplier** suppliers_;
  // Suppliers

  int nthreads_;

  // = Helpers to signal done.
  int peer_done_count_;
  // how many peers are done.

  TAO_SYNCH_MUTEX lock_;
  // The lock to serialize access to members.

  TAO_SYNCH_CONDITION condition_;
  // exit wait condition
private:
  friend class Throughput_StructuredPushSupplier;
  friend class Throughput_StructuredPushConsumer;
};

/***************************************************************************/

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* NOTIFY_TESTS_EventS_H */
