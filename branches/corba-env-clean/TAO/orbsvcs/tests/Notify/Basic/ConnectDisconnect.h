/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = FILENAME
//   ConnectDisconnect.h
//
// = DESCRIPTION
//   Test connect-disconnect methods of Notify.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

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
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class ConnectDisconnect;

class CD_Entity
{
  // The entity that connects/disconnects
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
  // Initialization and termination code
  ConnectDisconnect (void);
  virtual ~ConnectDisconnect ();

  void on_entity_destroyed (void);
  // keeps track of how many objects were destroyed.

  int parse_args(int argc, char *argv[]) ;

  void init (int argc, char *argv [] TAO_ENV_ARG_DECL);
  // initialization.

  void run_test (TAO_ENV_SINGLE_ARG_DECL);
  // Run the test.

  void end_test (TAO_ENV_SINGLE_ARG_DECL);
  // End the test.

  int check_results (void);
  // check if we got the expected results.

protected:
  void create_EC (TAO_ENV_SINGLE_ARG_DECL);
  // Create EC

  CosNotifyChannelAdmin::EventChannel_var ec_;
  // The one channel that we create using the factory.

  CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin_;
  // The consumer admin used by consumers.

  CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin_;
  // The supplier admin used by suppliers.

  TAO_Notify_PushConsumer** any_consumer_;
  TAO_Notify_StructuredPushConsumer** structured_consumer_;
  TAO_Notify_SequencePushConsumer** sequence_consumer_;
  // arrays of Consumers

  TAO_Notify_PushSupplier** any_supplier_;
  TAO_Notify_StructuredPushSupplier** structured_supplier_;
  TAO_Notify_SequencePushSupplier** sequence_supplier_;
  // arrays of Suppliers

  ACE_Atomic_Op <TAO_SYNCH_MUTEX, int> result_count_;
  // count of clients destroyed. for c consumers, s suppliers and for t times.
  // this should be (s+c)*t

  int expected_count_; // (s+c)*t

  // = command line params

  int  count_;
  // The number of iterations to connect disconnect.

  int consumers_;
  // The number of counsumers to create.

  int suppliers_;
  // The number of suppliers to create.
};

/***************************************************************************/

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* NOTIFY_TESTS_UPDATES_H */
