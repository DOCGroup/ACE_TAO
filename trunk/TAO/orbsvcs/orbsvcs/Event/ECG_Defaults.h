// -*- C++ -*-

/**
 *  @file   ECG_Defaults.h
 *
 *  $Id$
 *
 * In this file we set the compile time defaults for the event
 * channel multicast gateway.
 *
 *  @author Marina Spivak (marina@atdesk.com)
 */

#ifndef TAO_ECG_DEFAULTS_H
#define TAO_ECG_DEFAULTS_H
#include /**/ "ace/pre.h"

// Any of the definitions below can be overriden in the config.h file
// or the compilation line.

#ifndef TAO_ECG_DEFAULT_SERVICE
# define TAO_ECG_DEFAULT_SERVICE ECG_MCAST_TWO_WAY
#endif /* TAO_ECG_DEFAULT_SERVICE */

#ifndef TAO_ECG_DEFAULT_HANDLER
# define TAO_ECG_DEFAULT_HANDLER ECG_HANDLER_BASIC
#endif /* TAO_ECG_DEFAULT_HANDLER */

#ifndef TAO_ECG_DEFAULT_ADDRESS_SERVER
# define TAO_ECG_DEFAULT_ADDRESS_SERVER ECG_ADDRESS_SERVER_BASIC
#endif /* TAO_ECG_DEFAULT_ADDRESS_SERVER */

/* must be provided by the user */
#ifndef TAO_ECG_DEFAULT_ADDRESS_SERVER_ARG
# define TAO_ECG_DEFAULT_ADDRESS_SERVER_ARG 0
#endif /* TAO_ECG_DEFAULT_ADDRESS_SERVER_ARG */

#ifndef TAO_ECG_DEFAULT_TTL
# define TAO_ECG_DEFAULT_TTL 0 /* don't set, use default */
#endif /* TAO_ECG_DEFAULT_TTL */

#ifndef TAO_ECG_DEFAULT_NIC
# define TAO_ECG_DEFAULT_NIC 0 /* use default */
#endif /* TAO_ECG_DEFAULT_NIC */

#ifndef TAO_ECG_DEFAULT_IP_MULTICAST_LOOP
# define TAO_ECG_DEFAULT_IP_MULTICAST_LOOP 1 /* loopback mcast messages */
#endif /* TAO_ECG_DEFAULT_IP_MULTICAST_LOOP */

#ifndef TAO_ECG_DEFAULT_NON_BLOCKING
# define TAO_ECG_DEFAULT_NON_BLOCKING 1 /* write sockets are non-blocking */
#endif /* TAO_ECG_DEFAULT_NON_BLOCKING */

#ifndef TAO_ECG_DEFAULT_IIOP_CONSUMEREC_CONTROL
# define TAO_ECG_DEFAULT_IIOP_CONSUMEREC_CONTROL 0 /* null */
#endif /* TAO_ECG_DEFAULT_IIOP_CONSUMEREC_CONTROL */

#ifndef TAO_ECG_DEFAULT_IIOP_CONSUMEREC_CONTROL_PERIOD
# define TAO_ECG_DEFAULT_IIOP_CONSUMEREC_CONTROL_PERIOD 5000000 /* usecs */
#endif /* TAO_ECG_DEFAULT_IIOP_CONSUMEREC_CONTROL_PERIOD */

#ifndef TAO_ECG_DEFAULT_IIOP_CONSUMEREC_CONTROL_TIMEOUT
# define TAO_ECG_DEFAULT_IIOP_CONSUMEREC_CONTROL_TIMEOUT 10000 /* usecs */
#endif /* TAO_ECG_DEFAULT_IIOP_CONSUMEREC_CONTROL_TIMEOUT */

#ifndef TAO_ECG_DEFAULT_IIOP_ORB_ID
# define TAO_ECG_DEFAULT_IIOP_ORB_ID "" /* */
#endif /* TAO_ECG_DEFAULT_IIOP_ORB_ID */

#ifndef TAO_ECG_DEFAULT_IIOP_USE_TTL
# define TAO_ECG_DEFAULT_IIOP_USE_TTL 1 /* use TTL */
#endif /* TAO_ECG_DEFAULT_IIOP_USE_TTL */

#ifndef TAO_ECG_DEFAULT_IIOP_USE_CONSUMER_PROXY_MAP
# define TAO_ECG_DEFAULT_IIOP_USE_CONSUMER_PROXY_MAP 1 /* use consumer proxy map */
#endif /* TAO_ECG_DEFAULT_IIOP_USE_CONSUMER_PROXY_MAP */

#include /**/ "ace/post.h"
#endif /* TAO_ECG_DEFAULTS_H */
