/* -*- C++ -*- */
/**
 *  @file   EC.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 *  Event channel
 */

#ifndef EC_H
#define EC_H

#include "orbsvcs/RtecEventCommS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class EC
 *
 * @brief Simple event channel
 */
class EC
{
public:
  /// Constructor
  EC (void);

  /// Run the test
  int run (int argc, char* argv[]);

private:
  int parse_args (int argc, char *argv[]);
};

#endif /* EC_H */
