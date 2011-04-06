//=============================================================================
/**
 *  @file    Custom_Handler.cpp
 *
 *  $Id$
 *
 *  This is a custom event handler to be used with the thread timer queue
 *  adapter, and its appropriate upcall.
 *
 *
 *  @author Alon Diamant <diamant.alon@gmail.com
 */
//=============================================================================


#include "Custom_Handler.h"
#include "ace/OS_NS_stdio.h"

Custom_Handler::Custom_Handler(const ACE_Time_Value &expiration_time)
  :  expires_ (expiration_time),
     id_ (0)
{
}

Custom_Handler::~Custom_Handler (void)
{
}

void
Custom_Handler::set_id (int id)
{
  this->id_ = id;
}

// This is the method invoked when the Timer expires.
int
Custom_Handler::on_timeout (const ACE_Time_Value &current_time,
                            const void *)
{
  ACE_Time_Value delay = current_time - this->expires_;

  // No need to protect this printf is always called from a Async safe
  // point.
  ACE_OS::printf ("\nexpiring timer %d at %lu.%7.7lu secs\n"
                  "\tthere was a %lu.%7.7lu secs delay\n",
                  this->id_,
                  static_cast<unsigned long> (current_time.sec ()),
                  static_cast<unsigned long> (current_time.usec ()),
                  static_cast<unsigned long> (delay.sec ()),
                  static_cast<unsigned long> (delay.usec ()));

  // Notice this delete is protected.
  delete this;

  return 0;
}

int Custom_Handler_Upcall::registration(TTimerQueue& , Custom_Handler* , const void* )
{
    ACE_TRACE(ACE_TEXT ("registration"));

    return 0;
}

int Custom_Handler_Upcall::preinvoke(TTimerQueue& , Custom_Handler* , const void* , int , const ACE_Time_Value& , const void*& )
{
    ACE_TRACE(ACE_TEXT ("preinvoke"));

    return 0;
}

int Custom_Handler_Upcall::timeout(TTimerQueue& , Custom_Handler* handler, const void* arg, int , const ACE_Time_Value& cur_time)
{
    ACE_TRACE(ACE_TEXT ("timeout"));

    // Do the actual timer call
    handler->on_timeout(cur_time, arg);

    return 0;
}

int Custom_Handler_Upcall::postinvoke(TTimerQueue& , Custom_Handler* , const void* , int , const ACE_Time_Value& , const void* )
{
    ACE_TRACE(ACE_TEXT ("postinvoke"));

    return 0;
}

int Custom_Handler_Upcall::cancel_type(TTimerQueue& , Custom_Handler* , int , int& )
{
    ACE_TRACE(ACE_TEXT ("cancel_type"));

    return 0;
}

int Custom_Handler_Upcall::cancel_timer(TTimerQueue& , Custom_Handler* handler, int , int )
{
    ACE_TRACE(ACE_TEXT ("cancel_timer"));
    delete handler;
    return 0;
}

int Custom_Handler_Upcall::deletion(TTimerQueue& , Custom_Handler* handler, const void* )
{
    ACE_TRACE(ACE_TEXT ("deletion"));
    delete handler;
    return 0;
}

