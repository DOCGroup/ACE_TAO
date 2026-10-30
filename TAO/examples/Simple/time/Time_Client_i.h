// -*- C++ -*-

//=============================================================================
/**
 *  @file    Time_Client_i.h
 *
 *  This class implements the client calls to the Time example.
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
  /// Constructor
  Time_Client_i () = default;

  /// Destructor
  ~Time_Client_i () = default;

  Time_Client_i (Time_Client_i const &) = delete;
  Time_Client_i &operator= (Time_Client_i const &) = delete;
  Time_Client_i (Time_Client_i &&) = delete;
  Time_Client_i &operator= (Time_Client_i &&) = delete;

  /// Execute the methods
  int run (char const *, int, ACE_TCHAR **);

private:
  /// Instantiate the client object.
  Client<Time> client_;
};

#endif /* TIME_CLIENT_I_H */
