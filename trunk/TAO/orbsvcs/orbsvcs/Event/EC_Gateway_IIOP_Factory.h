// -*- C++ -*-

/**
 *  @file   EC_Gateway_IIOP_Factory.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen  (jwillemsen@remedy.nl)
 *
 */

#ifndef TAO_EC_GATEWAY_IIOP_FACTORY_H
#define TAO_EC_GATEWAY_IIOP_FACTORY_H

#include /**/ "ace/pre.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Time_Value.h"
#include "ace/Service_Object.h"
#include "ace/SString.h"

#include /**/ "orbsvcs/Event/event_serv_export.h"

#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ECG_ConsumerEC_Control;
class TAO_EC_Gateway_IIOP;

/**
 * @class TAO_EC_Gateway_IIOP_Factory
 *
 * @brief Factory for the EC_Gateway_IIOP settings
 *
 * Default configuration values can be found in ECG_Defaults.h
 */
class TAO_RTEvent_Serv_Export TAO_EC_Gateway_IIOP_Factory
  : public ACE_Service_Object
{
public:
  TAO_EC_Gateway_IIOP_Factory (void);
  virtual ~TAO_EC_Gateway_IIOP_Factory (void);

  /// The Service_Object entry points.
  //@{
  virtual int init (int argc, char* argv[]);
  virtual int fini (void);
  //@}

  /// Helper function to register the Gateway into the service
  /// configurator.
  static int init_svcs (void);

  /// Create the consumer event channel control.
  TAO_ECG_ConsumerEC_Control *
      create_consumerec_control (TAO_EC_Gateway_IIOP* gateway);

  /// Destroy consumer event channel control.
  void destroy_consumerec_control (TAO_ECG_ConsumerEC_Control* x);

  /// Accessors to use_ttl flag
  int use_ttl (void) const;

  /// Accessors to use_consumer_proxy_map flag
  int use_consumer_proxy_map (void) const;

private:
  /// Helper for agrument parsing.  Prints out an error message about
  /// unsupported option value.
  void unsupported_option_value (const char * option_name,
                                 const char * option_value);

protected:
  /// Use this ORB to locate global resources.
  ACE_CString orbid_;

  /// The control policy for the consumer event channel
  int consumer_ec_control_;

  /// The consumer event channel control periods in usecs
  int consumer_ec_control_period_;

  /// The control timeout in usecs for the consumer event channel
  ACE_Time_Value consumer_ec_control_timeout_;

  /// If 1, we use the TTL flags, if 0, we just ignore TTL.
  /// @note When this flag is set to 0, make sure that no recursive structures
  /// exists in the gateway setup for the same source/type combination, else
  /// one event will flow continuosly through all gateways.
  int use_ttl_;

  /// The flag for using the consumer proxy map. With 1 the consumer proxy map
  /// is used, meaning that for each unique source id we use a different
  /// proxy push consumer, if 0, we only use one proxy push consumer for all
  /// source ids.
  int use_consumer_proxy_map_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE (TAO_EC_Gateway_IIOP_Factory)
ACE_FACTORY_DECLARE (TAO_RTEvent_Serv, TAO_EC_Gateway_IIOP_Factory)

#if defined (__ACE_INLINE__)
#include "orbsvcs/Event/EC_Gateway_IIOP_Factory.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_EC_GATEWAY_IIOP_FACTORY_H */
