// -*- C++ -*-

//=============================================================================
/**
 *  @file    Time_Date_i.h
 *
 *  This class implements the Time_Date IDL interface.
 *
 *  @author Douglas C. Schmidt <d.schmidt@vanderbilt.edu> and Michael Kircher.
 */
//=============================================================================


#ifndef TIME_DATE_I_H
#define TIME_DATE_I_H

#include "Time_DateS.h"

class Time_Date_i
  : public POA_Time_Date
{
  // = TITLE
  //    Time_Date object implementation.
  //
  // = DESCRIPTION
  //    Time_Date object implementation
public:
  /// Obtain the time and date in binary format.
  virtual void bin_date (CORBA::Long_out time_date);

  /// Obtain the time and date in string format.
  virtual void str_date (CORBA::String_out time_date);

  /// Shutdown the server.
  virtual void shutdown ();

  /// ORB pointer.
  void orb (CORBA::ORB_ptr orb_ptr);

private:
  /// ORB pointer.
  CORBA::ORB_var orb_var_;
};

#endif /* TIME_DATE_I_H */
