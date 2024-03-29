// -*- C++ -*-

//=============================================================================
/**
 *  @file    Time_i.h
 *
 *  This class implements the Time IDL interface.
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================


#ifndef TIME_I_H
#define TIME_I_H

#include "TimeS.h"

/**
 * @class Time_i:
 *
 * @brief Time Object Implementation
 *
 * Implementation of a simple object that has two methods, one that
 * return the current time/date on the server and the other that
 * shuts down the server.
 */
class Time_i: public POA_Time
{
public:
  /// Constructor
  Time_i ();

  /// Destructor
  virtual ~Time_i ();

  /// Return the current time/date on the server
  virtual CORBA::Long current_time ();

  /// Shutdown the server.
  virtual void shutdown ();

  /// Set the ORB pointer.
  void orb (CORBA::ORB_ptr o);

private:
  /// ORB pointer.
  CORBA::ORB_var orb_;

  void operator= (const Time_i &);
};

#endif /* TIME_I_H */
