/* -*- C++ -*- */
/**
 *  @file PeriodicSupplier_Command.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_PERIODICSUPPLIER_COMMAND_H
#define TAO_NS_PERIODICSUPPLIER_COMMAND_H
#include "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SString.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "Command.h"

class TAO_NS_Periodic_Supplier;

/**
 * @class TAO_NS_Periodic_Supplier_Command
 *
 * @brief Command for Periodic Suppliers.
 *
 */
class TAO_NOTIFY_TEST_Export TAO_NS_Periodic_Supplier_Command : public TAO_NS_Command
{
public:
  /// Constuctor
  TAO_NS_Periodic_Supplier_Command (void);

  /// Destructor
  ~TAO_NS_Periodic_Supplier_Command ();

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
    DESTROY,
    SET_QOS,
    SET_ADMIN,
    OFFER,
    DISCONNECT,
    DEACTIVATE,
    DUMP_STATE
  };

  ACE_CString name_;

  ACE_CString admin_;

  ACE_CString poa_;

  CosNotifyChannelAdmin::InterFilterGroupOperator ifgop_;

  CosNotifyChannelAdmin::ProxyID id_;

  CosNotification::QoSProperties qos_;

  // Offer change
  CosNotification::EventTypeSeq added_;
  CosNotification::EventTypeSeq removed_;

  ///= Methods
  // Retreive the suppliers.
  TAO_NS_Periodic_Supplier* supplier (void);

  ///= Handlers
  void handle_create (ACE_ENV_SINGLE_ARG_DECL);
  void handle_offers (ACE_ENV_SINGLE_ARG_DECL);
  void handle_disconnect (ACE_ENV_SINGLE_ARG_DECL);
  void handle_deactivate (ACE_ENV_SINGLE_ARG_DECL);
  void handle_status (ACE_ENV_SINGLE_ARG_DECL);
  void handle_set_qos (ACE_ENV_SINGLE_ARG_DECL);
};

#include "ace/post.h"
#endif /* TAO_NS_PERIODICSUPPLIER_COMMAND_H */
