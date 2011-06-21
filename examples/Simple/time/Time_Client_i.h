// -*- C++ -*-

//=============================================================================
/**
 *  @file    Time_Client_i.h
 *
 *  $Id$
 *
 *  This class implements the client calls to the Time example.
 *
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef TIME_CLIENT_I_H
#define TIME_CLIENT_I_H

#include "../Simple_util.h"
#include "TimeC.h"

/**
 * @class Time_Client_i
 *
 * @brief Time_Client interface subclass.
 *
 * This class implements the interface between the interface
 * objects and the client.
 */
class Time_Client_i
{
public:
  // = Initialization and termination methods.
  /// Constructor
  Time_Client_i (void);

  /// Destructor
  ~Time_Client_i (void);

  /// Execute the methods
  int run (const char *, int, ACE_TCHAR**);

private:
  /// Instantiate the client object.
  Client<Time> client_;
};

#endif /* TIME_CLIENT_I_H */
