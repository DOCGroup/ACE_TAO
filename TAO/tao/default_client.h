// -*- C++ -*-

//=============================================================================
/**
 *  @file     default_client.h
 *
 *  $Id$
 *
 *  @author  Chris Cleeland
 */
//=============================================================================


#ifndef TAO_DEFAULT_CLIENT_H
#define TAO_DEFAULT_CLIENT_H

#include /**/ "ace/pre.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Client_Strategy_Factory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Default_Client_Strategy_Factory
 *
 * @brief This is the "default" client strategy factor for TAO.  It
 * includes strategies that are configured through command-line
 * options so that everything operates as if there were no
 * dynamically-linkable strategies.
 */
class TAO_Export TAO_Default_Client_Strategy_Factory
  : public TAO_Client_Strategy_Factory
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  TAO_Default_Client_Strategy_Factory (void);

  /// Destructor.
  virtual ~TAO_Default_Client_Strategy_Factory (void);

  // = Service Configurator hooks.
  /// Dynamic linking hook
  virtual int init (int argc, ACE_TCHAR* argv[]);

  /// Parse svc.conf arguments
  int parse_args (int argc, ACE_TCHAR* argv[]);

  // = Check Client_Strategy_Factory.h for the documentation of the
  //   following methods.
  virtual TAO_Transport_Mux_Strategy *create_transport_mux_strategy (TAO_Transport *transport);
  virtual ACE_Lock *create_transport_mux_strategy_lock (void);
  virtual int reply_dispatcher_table_size (void) const;
  virtual int allow_callback (void);
  virtual TAO_Wait_Strategy *create_wait_strategy (TAO_Transport *transport);
  virtual TAO_Connect_Strategy *create_connect_strategy (TAO_ORB_Core *);
  virtual bool use_cleanup_options (void) const;
  virtual Connect_Strategy connect_strategy (void) const;

protected:
  void report_option_value_error (const ACE_TCHAR* option_name,
                                  const ACE_TCHAR* option_value);

private:
  enum Lock_Type
  {
    TAO_NULL_LOCK,
    TAO_THREAD_LOCK
  };

  enum Transport_Mux_Strategy
  {
    TAO_MUXED_TMS,
    TAO_EXCLUSIVE_TMS
  };

  /// The client Request Mux Strategy.
  Transport_Mux_Strategy transport_mux_strategy_;

  enum Wait_Strategy
  {
    TAO_WAIT_ON_LEADER_FOLLOWER,
    TAO_WAIT_ON_REACTOR,
    TAO_WAIT_ON_READ,
    TAO_WAIT_ON_LF_NO_UPCALL
  };

  /// The wait-for-reply strategy.
  Wait_Strategy wait_strategy_;

  /// The connection initiation strategy.
  Connect_Strategy connect_strategy_;

  /// Size of the reply dispatcher table
  int rd_table_size_;

  /// Type of lock for the muxed_strategy
  Lock_Type muxed_strategy_lock_type_;

  /// Cleanupoptions for RW strategy
  bool use_cleanup_options_;
};

ACE_STATIC_SVC_DECLARE_EXPORT (TAO, TAO_Default_Client_Strategy_Factory)
ACE_FACTORY_DECLARE (TAO, TAO_Default_Client_Strategy_Factory)

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_DEFAULT_CLIENT_H */
