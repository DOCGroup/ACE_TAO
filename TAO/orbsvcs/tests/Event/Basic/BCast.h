/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   BCast.h
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
 */
class EC_BCast : public EC_Driver
{
public:
  /// Constructor
  EC_BCast ();

  // = The EC_Driver methods
  /// add some command line args to enable/disable bcastions
  int parse_args (int& argc, ACE_TCHAR* argv[]) override;
  void print_args () const override;
  void print_usage () override;

  /// set the bcastion flags
  void modify_attributes (TAO_EC_Event_Channel_Attributes& attr) override;

  /// Don't run the suppliers, just test connect and disconnect calls.
  void execute_test () override;

  /// Don't dump the EC_Driver results, they are meaningless.
  void dump_results () override;

private:
  /// The IP address used to broadcast
  const ACE_TCHAR *bcast_address_;

  /// The port used to send and receive bcast messages...
  u_short bcast_port_;
};

#endif /* EC_BCAST_H */
