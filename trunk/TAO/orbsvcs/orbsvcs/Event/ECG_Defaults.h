/* -*- C++ -*- */
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

#include /**/ "ace/post.h"
#endif /* TAO_ECG_DEFAULTS_H */
