/* -*- C++ -*- */
/**
 *  @file SupplierAdmin_Command.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_Tests_SUPPLIERADMIN_COMMAND_H
#define TAO_Notify_Tests_SUPPLIERADMIN_COMMAND_H
#include "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SString.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "Command.h"

/**
 * @class TAO_Notify_Tests_SupplierAdmin_Command
 *
 * @brief Command for Supplier Admins.
 *
 */
class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_SupplierAdmin_Command : public TAO_Notify_Tests_Command
{
public:
  /// Constuctor
  TAO_Notify_Tests_SupplierAdmin_Command (void);

  /// Destructor
  ~TAO_Notify_Tests_SupplierAdmin_Command ();

  /// Parse args and populate options.
  virtual void init (ACE_Arg_Shifter& arg_shifter);

  /// Execute the command.
  virtual void execute_i (ACE_ENV_SINGLE_ARG_DECL);

  /// Name of object
  virtual const char* get_name (void);
  static const char* name (void);

protected:

#ifdef HPUX_11
   // Remove clash with /usr/include/machine/cpu.h and /usr/include/pa/cpu.h
#undef COMMAND
#endif
  enum COMMAND
  {
    CREATE,
    OFFER,
    DESTROY,
    SET_QOS,
    SET_ADMIN,
    DUMP_STATE
  };

  ACE_CString name_;

  ACE_CString factory_;

  CosNotifyChannelAdmin::InterFilterGroupOperator ifgop_;

  CosNotifyChannelAdmin::AdminID id_;

  CosNotification::QoSProperties qos_;

  // Subscription change
  CosNotification::EventTypeSeq added_;
  CosNotification::EventTypeSeq removed_;

  /// = Handlers
  void handle_create (ACE_ENV_SINGLE_ARG_DECL);
  void handle_offers (ACE_ENV_SINGLE_ARG_DECL);
  void handle_status (ACE_ENV_SINGLE_ARG_DECL);
  void handle_set_qos (ACE_ENV_SINGLE_ARG_DECL);
};

#include "ace/post.h"
#endif /* TAO_Notify_Tests_SUPPLIERADMIN_COMMAND_H */
