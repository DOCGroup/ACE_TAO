/* -*- C++ -*- */
/**
 *  @file   Gateway.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen  (jwillemsen@remedy.nl)
 *
 *  IIOP Gateway
 */
#ifndef GATEWAY_H
#define GATEWAY_H

#include "orbsvcs/RtecEventCommS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Gateway
 *
 * @brief Simple gateway
 */
class Gateway
{
public:
  /// Constructor
  Gateway (void);

  /// Run the test
  int run (int argc, char* argv[]);

private:
  int parse_args (int argc, char *argv[]);
};

#endif /* GATEWAY_H */
