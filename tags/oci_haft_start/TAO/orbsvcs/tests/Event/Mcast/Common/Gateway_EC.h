/* -*- C++ -*- */
// $Id$

#ifndef GATEWAY_EC_H
#define GATEWAY_EC_H

#include "EC_Wrapper.h"
#include "ECMcastTests_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Gateway_EC
 *
 * @brief Runs multicast federated Event Channel server.
 *
 * Runs Event Channel and  mcast receiver and/or sender (as specified
 * in service config file).
 */
class ECMcastTests_Export Gateway_EC
{
public:

  /// Constructor.
  Gateway_EC (void);

  /// Run the server.
  /// Valid arguments: [-i ior_filename].
  int run (int argc, char ** argv);

private:

  /// Helpers.
  //@{
  int parse_args (int argc, char *argv[]);
  int check_for_nil (CORBA::Object_ptr obj, const char *message);
  void write_ior_file (CORBA::ORB_ptr orb,
                       RtecEventChannelAdmin::EventChannel_ptr ec
                       ACE_ENV_ARG_DECL);
  //@}

  /// Event Channel ior is written to this file.
  /// Default is "gateway-ec.ior", which can be overridden with the "-i"
  /// argument option.
  const char * ec_ior_file_;
};

#endif /* GATEWAY_EC_H */
