/* -*- C++ -*- */
/**
 *  @file EventChannel_Command.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_EVENTCHANNEL_COMMAND_H
#define TAO_NS_EVENTCHANNEL_COMMAND_H
#include "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SString.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "Command.h"

/**
 * @class TAO_NS_EventChannel_Command
 *
 * @brief Command object for event channels.
 *
 */
class TAO_NOTIFY_TEST_Export TAO_NS_EventChannel_Command : public TAO_NS_Command
{
public:
  /// Constuctor
  TAO_NS_EventChannel_Command (void);

  /// Destructor
  ~TAO_NS_EventChannel_Command ();

  /// Parse args and populate options.
  virtual void init (ACE_Arg_Shifter& arg_shifter);

  /// Execute the command.
  virtual void execute_i (ACE_ENV_SINGLE_ARG_DECL);

  /// Name of object
  virtual const char* get_name (void);
  static const char* name (void);

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

  COMMAND command_;

  ACE_CString name_;

  ACE_CString factory_;

  CosNotifyChannelAdmin::ChannelID id_;

  CosNotification::QoSProperties qos_;

  CosNotification::AdminProperties admin_;

  /// = Handlers
  void handle_create (ACE_ENV_SINGLE_ARG_DECL);
  void handle_destroy (ACE_ENV_SINGLE_ARG_DECL);
  void handle_set_qos (ACE_ENV_SINGLE_ARG_DECL);
  void handle_status (ACE_ENV_SINGLE_ARG_DECL);
};

#if defined (__ACE_INLINE__)
#include "EventChannel_Command.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_EVENTCHANNEL_COMMAND_H */
