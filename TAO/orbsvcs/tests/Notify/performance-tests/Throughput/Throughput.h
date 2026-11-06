/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Throughput.h
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
#include <atomic>
#include <string>
#include <vector>

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
  Worker () = default;

  Worker (Worker const&) = delete;
  Worker (Worker&&) = delete;
  Worker& operator= (Worker const&) = delete;
  Worker& operator= (Worker&&) = delete;

  void orb (CORBA::ORB_ptr orb);

  /// The thread entry point.
  int svc () override;

  std::atomic<bool> done_ {};

private:
  /// The orb
  CORBA::ORB_var orb_ {};
};

/***************************************************************************/

class Notify_Throughput;

class Throughput_StructuredPushConsumer
  : public TAO_Notify_Tests_StructuredPushConsumer
{
public:
  /// Constructor.
  explicit Throughput_StructuredPushConsumer (Notify_Throughput& test_client);

  Throughput_StructuredPushConsumer (Throughput_StructuredPushConsumer const&) = delete;
  Throughput_StructuredPushConsumer (Throughput_StructuredPushConsumer&&) = delete;
  Throughput_StructuredPushConsumer& operator= (Throughput_StructuredPushConsumer const&) = delete;
  Throughput_StructuredPushConsumer& operator= (Throughput_StructuredPushConsumer&&) = delete;

  // = StructuredPushSupplier methods
  void push_structured_event (CosNotification::StructuredEvent const& notification) override;

  /// Accumulate the throughput statistics into <throughput>
  void accumulate_into (ACE_Throughput_Stats& throughput) const;

  /// Accumulate the throughput statistics into <throughput>
  void dump_stats (ACE_TCHAR const* msg,
                   ACE_High_Res_Timer::global_scale_factor_type gsf);

protected:
  /// The test client owns the POA which controls this servant's lifetime.
  Notify_Throughput& test_client_;

  /// Protect internal state
  TAO_SYNCH_MUTEX lock_;

  /// The timestamp for the first message received
  ACE_hrtime_t throughput_start_ {};

  /// Used for reporting stats
  ACE_Throughput_Stats throughput_;

  /// The number of push() calls
  int push_count_ {};
};

/***************************************************************************/

class Throughput_StructuredPushSupplier
  : public TAO_Notify_Tests_StructuredPushSupplier,
    public ACE_Task_Base
{
public:
  /// Constructor.
  explicit Throughput_StructuredPushSupplier (Notify_Throughput& test_client);

  /// Destructor.
  ~Throughput_StructuredPushSupplier () override = default;

  Throughput_StructuredPushSupplier (Throughput_StructuredPushSupplier const&) = delete;
  Throughput_StructuredPushSupplier (Throughput_StructuredPushSupplier&&) = delete;
  Throughput_StructuredPushSupplier& operator= (Throughput_StructuredPushSupplier const&) = delete;
  Throughput_StructuredPushSupplier& operator= (Throughput_StructuredPushSupplier&&) = delete;

  // = The ACE_Task_Base methods....
  int svc () override;

  /// Accumulate the throughput statistics into <throughput>
  void accumulate_into (ACE_Throughput_Stats& throughput) const;

  /// Accumulate the throughput statistics into <throughput>
  void dump_stats (ACE_TCHAR const* msg,
                   ACE_High_Res_Timer::global_scale_factor_type gsf);

protected:
  /// The test client owns the POA which controls this servant's lifetime.
  Notify_Throughput& test_client_;

  /// Count the number of push() calls
  int push_count_ {};

  /// The time for the first event sent
  ACE_hrtime_t throughput_start_ {};

  /// Measure the elapsed time spent while sending the events.
  ACE_Throughput_Stats throughput_;
};

/***************************************************************************/

class Notify_Throughput : public Notify_Test_Client
{
public:
  // Initialization and termination code
  Notify_Throughput ();
  ~Notify_Throughput () override;

  Notify_Throughput (Notify_Throughput const&) = delete;
  Notify_Throughput (Notify_Throughput&&) = delete;
  Notify_Throughput& operator= (Notify_Throughput const&) = delete;
  Notify_Throughput& operator= (Notify_Throughput&&) = delete;

  int parse_args (int argc, ACE_TCHAR *argv[]) override;

  /// initialization.
  int init (int argc, ACE_TCHAR *argv []) override;

  /// Run the test.
  void run_test ();

  /// Peers call this when done.
  void peer_done ();

  /// check if we got the expected results.
  void dump_results ();

  Worker worker_;
protected:
  /// Create participants.
  void create_EC ();

  /// is the ec collocated.
  struct Options
  {
    bool collocated_ec {};
    int burst_count {1};
    int burst_pause {10000};
    int burst_size {1000};
    int payload_size {1024};
    int consumer_count {1};
    int supplier_count {1};
    std::string ec_name;
    bool expected_count_set {};
  } options_;

  /// Number of events received that each consumer expects to see.
  int perconsumer_count_ {};

  /// The one channel that we create using the factory.
  CosNotifyChannelAdmin::EventChannel_var ec_;

  /// The consumer admin used by consumers.
  CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin_;

  /// The supplier admin used by suppliers.
  CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin_;

  /// Consumers
  /// These are non-owning pointers; the POA holds the servant references.
  std::vector<Throughput_StructuredPushConsumer*> consumers_;

  /// Suppliers
  /// These are non-owning pointers; the POA holds the servant references.
  std::vector<Throughput_StructuredPushSupplier*> suppliers_;

  std::string payload_;

  // = Helpers to signal done.
  /// how many peers are done.
  int peer_done_count_ {};

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
