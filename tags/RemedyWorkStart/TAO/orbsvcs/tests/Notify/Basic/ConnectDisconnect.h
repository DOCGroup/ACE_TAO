/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   ConnectDisconnect.h
 *
 *  $Id$
 *
 * Test connect-disconnect methods of Notify.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef NOTIFY_CONNECT_DISCONNECT_H
#define NOTIFY_CONNECT_DISCONNECT_H

#include "Notify_Test_Client.h"
#include "Notify_StructuredPushConsumer.h"
#include "Notify_StructuredPushSupplier.h"
#include "Notify_PushConsumer.h"
#include "Notify_PushSupplier.h"
#include "Notify_SequencePushConsumer.h"
#include "Notify_SequencePushSupplier.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class ConnectDisconnect;

/**
 * @class CD_Entity
 The entity that connects/disconnects.
 */
class CD_Entity
{
public:
  CD_Entity (ConnectDisconnect* cd, int id);
  ~CD_Entity ();
private:
  int id_;
  ConnectDisconnect* cd_;
};

#define CD_DECLARE_ENTITY(X) \
class CD_##X : public TAO_Notify_##X, public CD_Entity \
{ \
public: \
  CD_##X (ConnectDisconnect* cd, int id); \
};

CD_DECLARE_ENTITY(PushConsumer)
CD_DECLARE_ENTITY(StructuredPushConsumer)
CD_DECLARE_ENTITY(SequencePushConsumer)

CD_DECLARE_ENTITY(PushSupplier)
CD_DECLARE_ENTITY(StructuredPushSupplier)
CD_DECLARE_ENTITY(SequencePushSupplier)

/***************************************************************************/

class ConnectDisconnect : public Notify_Test_Client
{
public:
  // Initialization and termination code.
  ConnectDisconnect (void);
  virtual ~ConnectDisconnect ();

  /// Keeps track of how many objects were destroyed.
  void on_entity_destroyed (void);

  int parse_args (int argc,
                  ACE_TCHAR *argv[]);

  /// initialization.
  int init (int argc,
            ACE_TCHAR *argv []);

  /// Run the test.
  void run_test (void);

  /// End the test.
  void end_test (void);

  /// Check if we got the expected results.
  int check_results (void);

protected:
  /// Create EC.
  void create_EC (void);

  /// The one channel that we create using the factory.
  CosNotifyChannelAdmin::EventChannel_var ec_;

  /// The consumer admin used by consumers.
  CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin_;

  /// The supplier admin used by suppliers.
  CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin_;

  /// Arrays of Consumers.
  TAO_Notify_Tests_PushConsumer** any_consumer_;
  TAO_Notify_Tests_StructuredPushConsumer** structured_consumer_;
  TAO_Notify_Tests_SequencePushConsumer** sequence_consumer_;

  /// arrays of Suppliers
  TAO_Notify_Tests_PushSupplier** any_supplier_;
  TAO_Notify_Tests_StructuredPushSupplier** structured_supplier_;
  TAO_Notify_Tests_SequencePushSupplier** sequence_supplier_;

  /// Count of clients destroyed. for c consumers, s suppliers and for t times.
  /// this should be (s+c)*t.
  ACE_Atomic_Op <TAO_SYNCH_MUTEX, int> result_count_;

  int expected_count_; // (s+c)*t

  // = command line params

  /// The number of iterations to connect disconnect.
  int count_;

  /// The number of counsumers to create.
  int consumers_;

  /// The number of suppliers to create.
  int suppliers_;
};

/***************************************************************************/

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* NOTIFY_TESTS_UPDATES_H */
