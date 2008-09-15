/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   BCast.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef EC_BCAST_H
#define EC_BCAST_H

#include "Driver.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class EC_BCast
 *
 * @brief Test the EC bcast
 *
 */
class EC_BCast : public EC_Driver
{
public:
  /// Constructor
  EC_BCast (void);

  // = The EC_Driver methods
  /// add some command line args to enable/disable bcastions
  virtual int parse_args (int& argc, char* argv[]);
  virtual void print_args (void) const;
  virtual void print_usage (void);

  /// set the bcastion flags
  virtual void modify_attributes (TAO_EC_Event_Channel_Attributes& attr);

  /// Don't run the suppliers, just test connect and disconnect calls.
  void execute_test (void);

  /// Don't dump the EC_Driver results, they are meaningless.
  void dump_results (void);

private:
  /// The IP address used to broadcast
  const char* bcast_address_;

  /// The port used to send and receive bcast messages...
  u_short bcast_port_;
};

#endif /* EC_BCAST_H */
