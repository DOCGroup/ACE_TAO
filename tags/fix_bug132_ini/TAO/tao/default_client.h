// This may look like C, but it's really -*- C++ -*-

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
#include "ace/pre.h"

#include "tao/Client_Strategy_Factory.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"

/**
 * @class TAO_Default_Client_Strategy_Factory
 *
 * @brief This is the "default" client strategy factor for TAO.  It
 * includes strategies that are configured through command-line
 * options so that everything operates as if there were no
 * dynamically-linkable strategies.
 */
class TAO_Export TAO_Default_Client_Strategy_Factory : public TAO_Client_Strategy_Factory
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  TAO_Default_Client_Strategy_Factory (void);

  /// Destructor.
  virtual ~TAO_Default_Client_Strategy_Factory (void);

  // = Service Configurator hooks.
  /// Dynamic linking hook
  virtual int init (int argc, char* argv[]);

  /// Parse svc.conf arguments
  int parse_args (int argc, char* argv[]);

  // = Check Client_Strategy_Factory.h for the documentation of the
  //   following methods.
  ACE_Lock* create_profile_lock (void);
  TAO_Transport_Mux_Strategy *create_transport_mux_strategy (TAO_Transport *transport);
  TAO_Wait_Strategy *create_wait_strategy (TAO_Transport *transport);
  virtual ACE_Lock *create_ft_service_retention_id_lock (void);

private:
  enum Lock_Type
  {
    TAO_NULL_LOCK,
    TAO_THREAD_LOCK
  };

  /// the lock type for forwarding IIOP Profile
  Lock_Type profile_lock_type_;

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
    TAO_WAIT_ON_READ
  };

  /// The wait-for-reply strategy.
  Wait_Strategy wait_strategy_;
};

#if defined (__ACE_INLINE__)
# include "tao/default_client.i"
#endif /* __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE_EXPORT (TAO, TAO_Default_Client_Strategy_Factory)
ACE_FACTORY_DECLARE (TAO, TAO_Default_Client_Strategy_Factory)

#include "ace/post.h"
#endif /* TAO_DEFAULT_CLIENT_H */
