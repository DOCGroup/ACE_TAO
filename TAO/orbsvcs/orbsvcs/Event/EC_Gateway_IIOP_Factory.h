/* -*- C++ -*- */
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

#include "orbsvcs/Event/event_export.h"
#include "ace/Service_Config.h"
#include "ace/Time_Value.h"
#include "ace/Service_Object.h"

class TAO_ECG_ConsumerEC_Control;
class TAO_EC_Gateway_IIOP;

/**
 * @class TAO_EC_Gateway_IIOP_Factory
 *
 * @brief Factory for the EC_Gateway_IIOP settings
 *
 * Default configuration values can be found in ECG_Defaults.h
 */
class TAO_RTEvent_Export TAO_EC_Gateway_IIOP_Factory
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
};

ACE_STATIC_SVC_DECLARE (TAO_EC_Gateway_IIOP_Factory)
ACE_FACTORY_DECLARE (TAO_RTEvent, TAO_EC_Gateway_IIOP_Factory)

#include /**/ "ace/post.h"
#endif /* TAO_EC_GATEWAY_IIOP_FACTORY_H */
