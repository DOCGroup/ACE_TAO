//=============================================================================
/**
 *  @file    timeout_i.h
 *
 *  Implements the CORBA object.
 *
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================

#ifndef TIMEOUT_I_H
#define TIMEOUT_I_H

#include "ace/High_Res_Timer.h"
#include "timeoutS.h"

class Timeout_i
: public POA_TimeoutObj
{
public:
  Timeout_i (CORBA::ORB_ptr orb);

  ~Timeout_i () = default;
  Timeout_i (Timeout_i const &) = delete;
  Timeout_i &operator= (Timeout_i const &) = delete;
  Timeout_i (Timeout_i &&) = delete;
  Timeout_i &operator= (Timeout_i &&) = delete;

  void sendTimeToWait (CORBA::Long msec) override;

  void shutdown () override;

private:
  CORBA::ORB_var orb_;
};

class TimeoutHandler_i
: public POA_AMI_TimeoutObjHandler
{
public:
  TimeoutHandler_i ();

  ~TimeoutHandler_i () = default;
  TimeoutHandler_i (TimeoutHandler_i const &) = delete;
  TimeoutHandler_i &operator= (TimeoutHandler_i const &) = delete;
  TimeoutHandler_i (TimeoutHandler_i &&) = delete;
  TimeoutHandler_i &operator= (TimeoutHandler_i &&) = delete;

  void sendTimeToWait () override;

  void sendTimeToWait_excep (::Messaging::ExceptionHolder *excep_holder) override;

  void reset_reply_counter ();

  void reset_reply_excep_counter ();

  unsigned short reply_counter ();

  unsigned short reply_excep_counter ();

  /// Start the time measurement
  void start ();

  /// Return the elapsed time
  ACE_Time_Value &elapsed_time ();

private:
  unsigned short reply_counter_;

  unsigned short reply_excep_counter_;

  ACE_High_Res_Timer timer_;

  ACE_Time_Value elapsed_time_;
};

#endif /* TIMEOUT_I_H */
