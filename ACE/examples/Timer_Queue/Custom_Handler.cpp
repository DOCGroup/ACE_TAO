//=============================================================================
/**
 *  @file    Custom_Handler.cpp
 *
 *  This is a custom event handler to be used with the thread timer queue
 *  adapter, and its appropriate upcall.
 *
 *  @author Alon Diamant <diamant.alon@gmail.com
 */
//=============================================================================


#include "Custom_Handler.h"
#include <atomic>
#include <cstdio>

Custom_Handler::Custom_Handler (
  ACE_Time_Value const &expiration_time,
  std::shared_ptr<std::atomic<long>> const &id)
  : expires_ (expiration_time),
    id_ (id)
{
}

Custom_Handler::~Custom_Handler () = default;

// This is the method invoked when the Timer expires.
int
Custom_Handler::on_timeout (ACE_Time_Value const &current_time,
                            void const *)
{
  ACE_Time_Value const delay = current_time - this->expires_;

  // No need to protect this printf is always called from a Async safe
  // point.
  std::printf ("\nexpiring timer %ld at %lu.%7.7lu secs\n"
                  "\tthere was a %lu.%7.7lu secs delay\n",
                  this->id_->load (std::memory_order_relaxed),
                  static_cast<unsigned long> (current_time.sec ()),
                  static_cast<unsigned long> (current_time.usec ()),
                  static_cast<unsigned long> (delay.sec ()),
                  static_cast<unsigned long> (delay.usec ()));

  // Notice this delete is protected.
  delete this;

  return 0;
}

int
Custom_Handler_Upcall::registration (TTimerQueue &,
                                    Custom_Handler *,
                                    void const *)
{
  ACE_TRACE ("registration");
  return 0;
}

int
Custom_Handler_Upcall::preinvoke (TTimerQueue &,
                                  Custom_Handler *,
                                  void const *,
                                  int,
                                  ACE_Time_Value const &,
                                  void const *&)
{
  ACE_TRACE ("preinvoke");
  return 0;
}

int
Custom_Handler_Upcall::timeout (TTimerQueue &,
                                Custom_Handler *handler,
                                void const *arg,
                                int,
                                ACE_Time_Value const &cur_time)
{
  ACE_TRACE ("timeout");
  handler->on_timeout (cur_time, arg);
  return 0;
}

int
Custom_Handler_Upcall::postinvoke (TTimerQueue &,
                                   Custom_Handler *,
                                   void const *,
                                   int,
                                   ACE_Time_Value const &,
                                   void const *)
{
  ACE_TRACE ("postinvoke");
  return 0;
}

int
Custom_Handler_Upcall::cancel_type (TTimerQueue &,
                                    Custom_Handler *,
                                    int,
                                    int &)
{
  ACE_TRACE ("cancel_type");
  return 0;
}

int
Custom_Handler_Upcall::cancel_timer (TTimerQueue &,
                                     Custom_Handler *handler,
                                     int,
                                     int)
{
  ACE_TRACE ("cancel_timer");
  delete handler;
  return 0;
}

int
Custom_Handler_Upcall::deletion (TTimerQueue &,
                                 Custom_Handler *handler,
                                 void const *)
{
  ACE_TRACE ("deletion");
  delete handler;
  return 0;
}

