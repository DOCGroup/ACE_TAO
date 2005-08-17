/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Shutdown.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef EC_SHUTDOWN_H
#define EC_SHUTDOWN_H

#include "Driver.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class EC_Shutdown
 *
 * @brief Test the EC shutdown features
 *
 * The EC must inform its suppliers and consumers on the event of
 * its destruction.
 * This test exercises that feature of the EC.
 */
class EC_Shutdown : public EC_Driver
{
public:
  /// Constructor
  EC_Shutdown (void);

  // = The EC_Driver methods
  /// Don't run the suppliers just create the EC and then destroy it.
  void execute_test (ACE_ENV_SINGLE_ARG_DECL);

  /// Don't dump the EC_Driver results, they are meaningless.
  void dump_results (void);

  /// One of the consumers in the test has been disconnected from the EC
  virtual void consumer_disconnect (void* consumer_cookie
                                    ACE_ENV_ARG_DECL);

  /// One of the suppliers in the test has been disconnected from the EC
  virtual void supplier_disconnect (void* supplier_cookie
                                    ACE_ENV_ARG_DECL);


private:
  /// Number of consumer and supplier disconnect messages.
  int consumer_disconnects_;
  int supplier_disconnects_;
};

#endif /* EC_SHUTDOWN_H */
