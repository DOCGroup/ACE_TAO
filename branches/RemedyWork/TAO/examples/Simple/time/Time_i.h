// -*- C++ -*-

//=============================================================================
/**
 *  @file    Time_i.h
 *
 *  $Id$
 *
 *  This class implements the Time IDL interface.
 *
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
  // = Initialization and termination methods.
  /// Constructor
  Time_i (void);

  /// Destructor
  virtual ~Time_i (void);

  /// Return the current time/date on the server
  virtual CORBA::Long current_time (void);

  /// Shutdown the server.
  virtual void shutdown (void);

  /// Set the ORB pointer.
  void orb (CORBA::ORB_ptr o);

private:
  /// ORB pointer.
  CORBA::ORB_var orb_;

  /// Keeping g++2.7.2
  ACE_UNIMPLEMENTED_FUNC (void operator= (const Time_i &))
};

#endif /* TIME_I_H */
