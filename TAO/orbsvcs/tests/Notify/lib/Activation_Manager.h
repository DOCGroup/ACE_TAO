/* -*- C++ -*- */
/**
 *  @file Activation_Manager.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_Tests_ACTIVATION_MANAGER_H
#define TAO_Notify_Tests_ACTIVATION_MANAGER_H
#include /**/ "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Hash_Map_Manager.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Null_Mutex.h"
#include "ace/SString.h"
#include "Task_Callback.h"
#include "Periodic_Supplier.h"
#include "Periodic_Consumer.h"
#include "Activation_ManagerS.h"

#include <memory>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Barrier;
ACE_END_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_Tests_Activation_Manager
 *
 * @brief Class to handle Suppliers and Consumers.
 */
class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_Activation_Manager : public TAO_Notify_Tests_Task_Callback
                                                       , public POA_Notify_Test::Activation_Manager
{
  struct File_Closer
  {
    void operator() (FILE *file) const noexcept;
  };

  using File_Handle = std::unique_ptr<FILE, File_Closer>;

  using TAO_Notify_Tests_PeriodicSupplier_Map =
    ACE_Hash_Map_Manager<ACE_CString, TAO_Notify_Tests_Periodic_Supplier*, ACE_SYNCH_NULL_MUTEX>;
  using TAO_Notify_Tests_PeriodicConsumer_Map =
    ACE_Hash_Map_Manager<ACE_CString, TAO_Notify_Tests_Periodic_Consumer*, ACE_SYNCH_NULL_MUTEX>;
  using TAO_Notify_Tests_PeriodicSupplier_Iterator =
    ACE_Hash_Map_Iterator<ACE_CString, TAO_Notify_Tests_Periodic_Supplier*, ACE_SYNCH_NULL_MUTEX>;
  using TAO_Notify_Tests_PeriodicConsumer_Iterator =
    ACE_Hash_Map_Iterator<ACE_CString, TAO_Notify_Tests_Periodic_Consumer*, ACE_SYNCH_NULL_MUTEX>;
  using TAO_Notify_Tests_PeriodicSupplier_Entry = ACE_Hash_Map_Entry<ACE_CString, TAO_Notify_Tests_Periodic_Supplier*>;
  using TAO_Notify_Tests_PeriodicConsumer_Entry = ACE_Hash_Map_Entry<ACE_CString, TAO_Notify_Tests_Periodic_Consumer*>;

public:
  TAO_Notify_Tests_Activation_Manager ();

  TAO_Notify_Tests_Activation_Manager (TAO_Notify_Tests_Activation_Manager const &) = delete;
  TAO_Notify_Tests_Activation_Manager &operator=(TAO_Notify_Tests_Activation_Manager const &) = delete;
  TAO_Notify_Tests_Activation_Manager (TAO_Notify_Tests_Activation_Manager &&) = delete;
  TAO_Notify_Tests_Activation_Manager &operator=(TAO_Notify_Tests_Activation_Manager &&) = delete;

  virtual ~TAO_Notify_Tests_Activation_Manager ();

  void start () override;

  /// Registers a supplier.
  void _register (TAO_Notify_Tests_Periodic_Supplier *supplier, char const *obj_name);
  /// Registers a consumer.
  void _register (TAO_Notify_Tests_Periodic_Consumer *consumer, char const *obj_name);

  /// Resolves a supplier by object name.
  void resolve (TAO_Notify_Tests_Periodic_Supplier *&supplier, char const *obj_name);

  /// Resolves a consumer by object name.
  void resolve (TAO_Notify_Tests_Periodic_Consumer *&consumer, char const *obj_name);

  /// Activates all supplier tasks.
  int activate_suppliers ();

  /// Returns the number of registered suppliers.
  int supplier_count ();

  /// Returns the number of registered consumers.
  int consumer_count ();

  /// Wait till active suppliers and consumers are done.
  void wait_for_completion ();

  void done (TAO_Notify_Tests_Periodic_Supplier* supplier) override;
  void done (TAO_Notify_Tests_Periodic_Consumer* consumer) override;

  /// Dumps statistics gathered during execution.
  void dump_stats (int dump_samples);

  /// Opens the output file for writing and returns 0 on success.
  int ior_output_file (ACE_TCHAR const *file_name);

  /// Stores the file name used to read the peer's IOR and returns 0.
  int ior_input_file (ACE_TCHAR const *file_name);

  void write_ior ();
  void wait_for_start_signal ();
  void signal_peer ();

protected:
  /// Lock to serialize internal state.
  TAO_SYNCH_MUTEX lock_;

  /// The barrier to synch all supplier on so that everyone starts at the same instance.
  std::unique_ptr<ACE_Barrier> barrier_ {};

  /// Number of Active suppliers (still running a ACE_Task and sending events)
  int active_suppliers_;

  /// Number of Active consumers (still receiving events)
  int active_consumers_;

  /// Signals when all active suppliers and consumers are done.
  TAO_SYNCH_CONDITION active_done_;

  /// Registered suppliers.
  TAO_Notify_Tests_PeriodicSupplier_Map supplier_map_;

  /// Registered consumers.
  TAO_Notify_Tests_PeriodicConsumer_Map consumer_map_;

  /// Owned output stream, closed after writing or when the manager is destroyed.
  File_Handle ior_output_file_;

  /// File name used as the peer's IOR input.
  ACE_TString ior_input_file_;

  /// Set when the start method is called.
  bool started_;

  /// Signals when the start method has been called.
  TAO_SYNCH_CONDITION started_condition_;
};

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_ACTIVATION_MANAGER_H */
