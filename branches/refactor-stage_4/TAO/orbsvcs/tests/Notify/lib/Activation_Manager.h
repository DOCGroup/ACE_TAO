/* -*- C++ -*- */
/**
 *  @file Activation_Manager.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_ACTIVATION_MANAGER_H
#define TAO_NS_ACTIVATION_MANAGER_H
#include "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SString.h"
#include "ace/Hash_Map_Manager.h"
#include "Task_Callback.h"
#include "Periodic_Supplier.h"
#include "Periodic_Consumer.h"
#include "Activation_ManagerS.h"

class ACE_Barrier;

/**
 * @class TAO_NS_Activation_Manager
 *
 * @brief Class to handle Suppliers and Consumers.
 *
 */
class TAO_NOTIFY_TEST_Export TAO_NS_Activation_Manager : public TAO_NS_Task_Callback
                                                       , public POA_Notify_Test::Activation_Manager
{
  typedef ACE_Hash_Map_Manager<ACE_CString, TAO_NS_Periodic_Supplier*, ACE_SYNCH_NULL_MUTEX> TAO_NS_PeriodicSupplier_Map;
  typedef ACE_Hash_Map_Manager<ACE_CString, TAO_NS_Periodic_Consumer*, ACE_SYNCH_NULL_MUTEX> TAO_NS_PeriodicConsumer_Map;
  typedef ACE_Hash_Map_Iterator<ACE_CString, TAO_NS_Periodic_Supplier*, ACE_SYNCH_NULL_MUTEX> TAO_NS_PeriodicSupplier_Iterator;
  typedef ACE_Hash_Map_Iterator<ACE_CString, TAO_NS_Periodic_Consumer*, ACE_SYNCH_NULL_MUTEX> TAO_NS_PeriodicConsumer_Iterator;
  typedef ACE_Hash_Map_Entry<ACE_CString, TAO_NS_Periodic_Supplier*> TAO_NS_PeriodicSupplier_Entry;
  typedef ACE_Hash_Map_Entry<ACE_CString, TAO_NS_Periodic_Consumer*> TAO_NS_PeriodicConsumer_Entry;

public:
  /// Constuctor
  TAO_NS_Activation_Manager (void);

  /// Destructor
  virtual ~TAO_NS_Activation_Manager ();

  /// Interface impl.
  virtual void start (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  /// Register Supplier
  void _register (TAO_NS_Periodic_Supplier* supplier, const char* obj_name ACE_ENV_ARG_DECL);
  /// Register Consumer
  void _register (TAO_NS_Periodic_Consumer* consumer, const char* obj_name ACE_ENV_ARG_DECL);

  /// Resolve Supplier
  void resolve (TAO_NS_Periodic_Supplier*& supplier, const char* obj_name ACE_ENV_ARG_DECL);

  /// Resolve Consumer
  void resolve (TAO_NS_Periodic_Consumer*& consumer, const char* obj_name ACE_ENV_ARG_DECL);

  // Activate the tasks for each supplier.
  int activate_suppliers (void);

  // Supplier Count
  int supplier_count (void);

  // Consumer Count
  int consumer_count (void);

  /// Wait till active suppliers and consumers are done.
  void wait_for_completion (void);

  /// TAO_NS_Task_Callback methods
  virtual void done (TAO_NS_Periodic_Supplier* supplier);
  virtual void done (TAO_NS_Periodic_Consumer* consumer);

  // Dump stats gathered during exec..
  void dump_stats (int dump_samples);

  // Returns 0 if the file could be opened for write.
  int ior_output_file (const ACE_TCHAR *file_name);

  // Returns 1 if the file could be opened for read.
  int ior_input_file (const ACE_TCHAR *file_name);

  void write_ior (ACE_ENV_SINGLE_ARG_DECL);
  void wait_for_start_signal (ACE_ENV_SINGLE_ARG_DECL);
  void signal_peer (ACE_ENV_SINGLE_ARG_DECL);

protected:
  /// Lock to serialize internal state.
  TAO_SYNCH_MUTEX lock_;

  /// The barrier to synch all supplier on so that everyone starts at the same instance.
  ACE_Barrier* barrier_;

  /// Number of Active suppliers (still running a ACE_Task and sending events)
  int active_suppliers_;

  /// Number of Active consumers (still receiving events)
  int active_consumers_;

  // Condition that all active suppliers and consumers are done.
  TAO_SYNCH_CONDITION active_done_;

  // Map that stores suppliers.
  TAO_NS_PeriodicSupplier_Map supplier_map_;

  // Map that stores consumers.
  TAO_NS_PeriodicConsumer_Map consumer_map_;

  /// The file for output
  FILE *ior_output_file_;

  /// The file for input
  ACE_CString ior_input_file_;

  /// Set when the start method is called.
  CORBA::Boolean started_;

  // Condition that the start method has been called.
  TAO_SYNCH_CONDITION started_condition_;
};

#include "ace/post.h"
#endif /* TAO_NS_ACTIVATION_MANAGER_H */
