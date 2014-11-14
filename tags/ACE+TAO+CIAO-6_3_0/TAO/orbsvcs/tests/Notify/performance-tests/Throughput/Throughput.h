/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Throughput.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef NOTIFY_Throughput_H
#define NOTIFY_Throughput_H

#include "Notify_Test_Client.h"
#include "Notify_StructuredPushConsumer.h"
#include "Notify_StructuredPushSupplier.h"
#include "ace/Task.h"
#include "ace/Stats.h"
#include "ace/Throughput_Stats.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class Worker
 *
 * @brief Run a server thread
 *
 * Use the ACE_Task_Base class to run server threads
 */
class Worker : public ACE_Task_Base
{
public:
  /// ctor
  Worker (void);

  void orb (CORBA::ORB_ptr orb);

  /// The thread entry point.
  virtual int svc (void);

  int done_;

private:
  /// The orb
  CORBA::ORB_var orb_;
};

/***************************************************************************/

class Notify_Throughput;

class Throughput_StructuredPushConsumer
  : public TAO_Notify_Tests_StructuredPushConsumer
{
public:
  /// Contructor.
  Throughput_StructuredPushConsumer (Notify_Throughput *test_client);

  // = StructuredPushSupplier methods
  virtual void push_structured_event (
        const CosNotification::StructuredEvent & notification
      );

  /// Accumulate the throughput statistics into <throughput>
  void accumulate_into (ACE_Throughput_Stats &throughput) const;

  /// Accumulate the throughput statistics into <throughput>
  void dump_stats (const ACE_TCHAR* msg,
                   ACE_High_Res_Timer::global_scale_factor_type gsf);

protected:
  Notify_Throughput * test_client_;

  /// Protect internal state
  TAO_SYNCH_MUTEX lock_;

  /// The timestamp for the first message received
  ACE_hrtime_t throughput_start_;

  /// Used for reporting stats
  ACE_Throughput_Stats throughput_;

  /// The number of push() calls
  int push_count_;
};

/***************************************************************************/

class Throughput_StructuredPushSupplier
  : public TAO_Notify_Tests_StructuredPushSupplier,
    public ACE_Task_Base
{
public:
  /// Constructor.
  Throughput_StructuredPushSupplier (Notify_Throughput * test_client);

  /// Destructor.
  virtual ~Throughput_StructuredPushSupplier ();

  // = The ACE_Task_Base methods....
  virtual int svc (void);

  /// Accumulate the throughput statistics into <throughput>
  void accumulate_into (ACE_Throughput_Stats &throughput) const;

  /// Accumulate the throughput statistics into <throughput>
  void dump_stats (const ACE_TCHAR* msg,
                   ACE_High_Res_Timer::global_scale_factor_type gsf);

protected:
  Notify_Throughput* test_client_;

  /// Count the number of push() calls
  int push_count_;

  /// The time for the first event sent
  ACE_hrtime_t throughput_start_;

  /// Measure the elapsed time spent while sending the events.
  ACE_Throughput_Stats throughput_;
};

/***************************************************************************/

class Notify_Throughput : public Notify_Test_Client
{
public:
  // Initialization and termination code
  Notify_Throughput (void);
  virtual ~Notify_Throughput ();

  int parse_args(int argc, ACE_TCHAR *argv[]) ;

  /// initialization.
  int init (int argc, ACE_TCHAR *argv []);

  /// Run the test.
  void run_test (void);

  /// Peers call this when done.
  void peer_done (void);

  /// check if we got the expected results.
  void dump_results (void);

  Worker worker_;
protected:
  /// Create participants.
  void create_EC (void);

  /// is the ec collocated.
  CORBA::Boolean collocated_ec_;

  /// How many bursts we will send from each supplier.
  int burst_count_;

  /// The time between each event burst, in microseconds.
  int burst_pause_;

  /// Number of events to send per supplier in every burst
  int burst_size_;

  /// data size to transmit.
  int payload_size_;

  /// the payload.
  char *payload_;

  /// Consumer count
  int consumer_count_;

  /// Supplier count
  int supplier_count_;

  /// Number of events received that each consumer expects to see.
  int perconsumer_count_;

  /// The name of the EC to resolve.
  ACE_CString ec_name_;

  /// The one channel that we create using the factory.
  CosNotifyChannelAdmin::EventChannel_var ec_;

  /// The consumer admin used by consumers.
  CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin_;

  /// The supplier admin used by suppliers.
  CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin_;

  /// Consumers
  Throughput_StructuredPushConsumer** consumers_;

  /// Suppliers
  Throughput_StructuredPushSupplier** suppliers_;

  int nthreads_;

  // = Helpers to signal done.
  /// how many peers are done.
  int peer_done_count_;

  /// The lock to serialize access to members.
  TAO_SYNCH_MUTEX lock_;

  /// exit wait condition
  TAO_SYNCH_CONDITION condition_;
private:
  friend class Throughput_StructuredPushSupplier;
  friend class Throughput_StructuredPushConsumer;
};

/***************************************************************************/

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* NOTIFY_TESTS_EventS_H */
