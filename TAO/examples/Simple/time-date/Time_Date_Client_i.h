// -*- C++ -*-

//=============================================================================
/**
 *  @file    Time_Date_Client_i.h
 *
 *  This class implements the interface calls for RMI.
 *
 *  @author Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
 */
//=============================================================================


#ifndef TIME_DATE_CLIENT_I_H
#define TIME_DATE_CLIENT_I_H

#include "../Simple_util.h"
#include "Time_DateC.h"

/**
 * @class Time_Date_Client_i
 *
 * @brief Time_Date_Client interface adapter.
 *
 * This class implements the interface between the interface
 * objects and the client.
 */
class Time_Date_Client_i
{
public:
  /// Constructor
  Time_Date_Client_i () = default;

  /// Destructor
  ~Time_Date_Client_i () = default;

  Time_Date_Client_i (Time_Date_Client_i const &) = delete;
  Time_Date_Client_i &operator= (Time_Date_Client_i const &) = delete;
  Time_Date_Client_i (Time_Date_Client_i &&) = delete;
  Time_Date_Client_i &operator= (Time_Date_Client_i &&) = delete;

  /// Execute the methods.
  int run (char const *, int, ACE_TCHAR *[]);

  /// Parses the command line arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

private:
  /// Instantiate the client object.
  Client<Time_Date> client_;
};

#endif /* TIME_CLIENT_I_H */
