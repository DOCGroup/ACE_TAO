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
  Time_Date_Client_i ();

  /// Destructor
  virtual ~Time_Date_Client_i ();

  /// Execute the methods.
  virtual int run (const char *, int, ACE_TCHAR *[]);

  /// Parses the command line arguments.
  virtual int parse_args (int argc, ACE_TCHAR *argv[]);

private:
  /// Instantiate the client object.
  Client<Time_Date> client_;
};

#endif /* TIME_CLIENT_I_H */
