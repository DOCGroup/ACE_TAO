/* -*- C++ -*- */
/**
 *  @file EventChannel_Command.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_Tests_EVENTCHANNEL_COMMAND_H
#define TAO_Notify_Tests_EVENTCHANNEL_COMMAND_H
#include /**/ "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SString.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "Command.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL
class TAO_Notify_Service;
TAO_END_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_Tests_EventChannel_Command
 *
 * @brief Command object for event channels.
 */
class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_EventChannel_Command : public TAO_Notify_Tests_Command
{
public:
  /// Constructor
  TAO_Notify_Tests_EventChannel_Command ();

  /// Destructor
  virtual ~TAO_Notify_Tests_EventChannel_Command ();

  /// Parse args and populate options.
  virtual void init (ACE_Arg_Shifter& arg_shifter);

  /// Execute the command.
  virtual void execute_i ();

  /// Name of object
  virtual const char* get_name ();
  static const char* name ();

protected:
  ///= Data Members
  enum COMMAND
  {
    CREATE,
    DESTROY,
    SET_QOS,
    SET_ADMIN,
    DUMP_STATE
  };

  ACE_CString name_;

  ACE_CString factory_;

  CosNotifyChannelAdmin::ChannelID id_;

  CosNotification::QoSProperties qos_;

  CosNotification::AdminProperties admin_;

  /// Flag to tell if the Channel Factory should be collocated.
  CORBA::Boolean collocated_;

  /// Helper method to create a collocated ECF.
  void create_collocated_ecf ();

  /// = Handlers
  void handle_create ();
  void handle_destroy ();
  void handle_set_qos ();
  void handle_status ();
};

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_EVENTCHANNEL_COMMAND_H */
