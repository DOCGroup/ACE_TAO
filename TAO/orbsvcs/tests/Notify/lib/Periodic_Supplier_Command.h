/* -*- C++ -*- */
/**
 *  @file Periodic_Supplier_Command.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_Tests_PERIODICSUPPLIER_COMMAND_H
#define TAO_Notify_Tests_PERIODICSUPPLIER_COMMAND_H
#include /**/ "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SString.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "Command.h"

class TAO_Notify_Tests_Periodic_Supplier;

/**
 * @class TAO_Notify_Tests_Periodic_Supplier_Command
 *
 * @brief Command for Periodic Suppliers.
 */
class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_Periodic_Supplier_Command : public TAO_Notify_Tests_Command
{
public:
  /// Constructor
  TAO_Notify_Tests_Periodic_Supplier_Command ();

  /// Destructor
  ~TAO_Notify_Tests_Periodic_Supplier_Command ();

  /// Parse args and populate options.
  virtual void init (ACE_Arg_Shifter& arg_shifter);

  /// Execute the command.
  virtual void execute_i ();

  /// Name of object
  virtual const char* get_name ();
  static const char* name ();

protected:
  enum COMMAND
  {
    CREATE,
    DESTROY,
    SET_QOS,
    SET_ADMIN,
    OFFER,
    DISCONNECT,
    DEACTIVATE,
    DUMP_STATE
  };

  /// My name.
  ACE_CString name_;

  /// QoS
  CosNotification::QoSProperties qos_;

  // Offer change
  CosNotification::EventTypeSeq added_;
  CosNotification::EventTypeSeq removed_;

  ///= Methods
  // Retreive the suppliers.
  TAO_Notify_Tests_Periodic_Supplier* supplier ();

  ///= Handlers
  void handle_create ();
  void handle_offers ();
  void handle_disconnect ();
  void handle_deactivate ();
  void handle_status ();
  void handle_set_qos ();
};

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_PERIODICSUPPLIER_COMMAND_H */
