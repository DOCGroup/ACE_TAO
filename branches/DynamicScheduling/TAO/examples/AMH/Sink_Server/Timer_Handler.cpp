// -*- C++ -*-

// =========================================================================
/**
 *  @file  Timer_Handler.cpp
 *
 *  $Id$
 *
 *  @desc  Fires servant upcall after a specified period of delay
 *
 *  @author Mayur Deshpande <mayur@ics.uci.edu>
 *
 */
// =========================================================================

#include "ace/Timer_Queue.h"
#include "TestS.h"

// @@ Mayur, please do not program like a Java programmer.  Please
//    move the class declaration to a separate header file, and keep
//    the implementation code in this file.
class Timer_Handler : public ACE_Event_Handler
{
public:

  Timer_Handler (Test::Timestamp send_time)
    : send_time_ (send_time)
  {
  }

  virtual int handle_timeout (const ACE_Time_Value &time,
                              const void *arg)
  {
    // @@ Mayur, please use an ACE_static_cast.  The C sledgehammer
    //    cast isn't necessary.
    Test::AMH_RoundtripResponseHandler* rh =
      (Test::AMH_RoundtripResponseHandler*) arg;

    // @@ Mayur, you're not supporting emulating exceptions.  This
    //    should be something like:
    //
    //      rh->test_method (this->send_time_
    //                       ACE_ENV_ARG_PARAMETER);
    //      ACE_TRY_CHECK;
    //
    //    You'll of course have to add the appropriate ACE_TRY/CATCH
    //    block.  Note that you're also missing the "this->" before
    //    "send_time_".  Please review the ACE/TAO coding/style
    //    guidelines.
    rh->test_method (send_time_);

    // We had duplicated the RH, so release it after we finish using
    // it.
    CORBA::release (rh);

    // @@ Mayur, if you're not going to use the "time" parameter,
    //    please mark it as "unused" the standard C++ way.
    //    Specifically, comment out the parameter name or remove it
    //    completely, e.g.:
    //
    //      virtual int handle_timeout (const ACE_Time_Value &,
    //                                        const void *arg)
    //
    //    ACE_UNUSED_ARG is a hack to silence compiler warnings.
    //    There are use cases for it, but this is not one of them.
    ACE_UNUSED_ARG (time);
    return -1;
  }


  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask mask)
  {
    // deregister from reactor and destruct

    // @@ Mayur, please remove these ACE_UNUSED_ARG macros in favor of
    //    the standard C++ method for marking parameters as unused, as
    //    described above.
    ACE_UNUSED_ARG (handle);
    ACE_UNUSED_ARG (mask);
    delete this;
    return 0;
  }

private:
  Test::Timestamp send_time_;
};
