// -*- C++ -*-

//=============================================================================
/**
 *  @file    Echo_Client_i.h
 *
 *  $Id$
 *
 *  This class implements the  interface calls for RMI.
 *
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef ECHO_CLIENT_I_H
#define ECHO_CLIENT_I_H

#include "Simple_util.h"
#include "EchoC.h"

/**
 * @class Echo_Client_i
 *
 * @brief Echo_Client interface subclass.
 *
 * This class implements the interface between the interface
 * objects and the client .
 */
class Echo_Client_i
{
public:
  // = Initialization and termination methods.
  /// Constructor
  Echo_Client_i (void);

  /// Destructor
  ~Echo_Client_i (void);

  /// Execute the methods
  int run (const char *, int, ACE_TCHAR **);

private:
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// Instantiate the client object.
  Client<Echo> client_;

  size_t payload_length_;
};

#endif /* TIME_CLIENT_I_H */
