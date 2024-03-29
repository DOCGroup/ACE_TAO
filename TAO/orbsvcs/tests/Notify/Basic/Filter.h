/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Filter.h
 *
 * Filter test any supplier to any consumer.
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef NOTIFY_TESTS_FILTER_H
#define NOTIFY_TESTS_FILTER_H

#include "Notify_Test_Client.h"
#include "Notify_PushConsumer.h"
#include "Notify_PushSupplier.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class Filter;

class Filter : public Notify_Test_Client
{
public:
  // Initialization and termination code
  Filter ();
  virtual ~Filter ();

  /// initialization.
  int init (int argc,
            ACE_TCHAR *argv []);

  /// Run the test.
  void run_test ();

protected:
  /// Create EC
  void create_EC ();

  /// Run some tests to check the filters.
  void run_filter_test (CosNotifyFilter::FilterAdmin_ptr);

  /// Print the filter ids.
  void print_filters (CosNotifyFilter::FilterAdmin_ptr filter_seq);

  /// Verify filter count.
  void verify_filter_count (CosNotifyFilter::FilterAdmin_ptr filter_admin, CORBA::ULong expected_count);

  /// Add a filter.
  CosNotifyFilter::FilterID add_filter (CosNotifyFilter::FilterAdmin_ptr filter_admin);

  /// The default filter factory.
  CosNotifyFilter::FilterFactory_var ffact_;

  /// Number of events received so far.
  ACE_Atomic_Op <TAO_SYNCH_MUTEX, int> result_count_;

  /// Number of events to send
  int event_count_;

  /// The one channel that we create using the factory.
  CosNotifyChannelAdmin::EventChannel_var ec_;

  /// The consumer admin used by consumers.
  CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin_;

  /// The supplier admin used by suppliers.
  CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin_;

  /// Consumer.
  TAO_Notify_Tests_PushConsumer* consumer_;

  /// Supplier.
  TAO_Notify_Tests_PushSupplier* supplier_;
};

/***************************************************************************/

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* NOTIFY_TESTS_FILTER_H */
