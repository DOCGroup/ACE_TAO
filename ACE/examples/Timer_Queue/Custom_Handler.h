/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Custom_Handler.h
 *
 *  This is a custom event handler to be used with the thread timer queue
 *  adapter, and its appropriate upcall.
 *
 *  @author Alon Diamant <diamant.alon@gmail.com>
 */
//=============================================================================


#ifndef _CUSTOM_HANDLER_H_
#define _CUSTOM_HANDLER_H_

#include "ace/Timer_Queue.h"
#include "ace/svc_export.h"
#include <atomic>
#include <memory>

/**
 * @class Custom_Handler
 *
 * @brief Custom event handler for the timer queue timeout events.
 *
 * The <on_timeout> hook method prints out the current time,
 * prints the time when this timer expired and deletes "this".
 */
class Custom_Handler
{
    public:
        Custom_Handler (
          ACE_Time_Value const &expiration_time,
          std::shared_ptr<std::atomic<long>> const &id);

        virtual ~Custom_Handler ();

        Custom_Handler (Custom_Handler const &) = delete;
        Custom_Handler (Custom_Handler &&) = delete;
        Custom_Handler &operator= (Custom_Handler const &) = delete;
        Custom_Handler &operator= (Custom_Handler &&) = delete;

        // Call back hook.
        virtual int on_timeout (ACE_Time_Value const &current_time,
                                void const *arg);

    private:
        // Store the expected time of expiration, it is used to print a nice
        // message saying how much delay was at the actual expiration time.
        ACE_Time_Value expires_ {};

        // Store an "id" for the Handler, which is only use to print better
        // messages.
        std::shared_ptr<std::atomic<long>> id_;
};


/// CWorkItemAndParamTupleUpcall
///
/// Implements the Upcall interface used by the ACE_Timer_Queue, specifically for the
/// IWorkItem interface.
class ACE_Svc_Export Custom_Handler_Upcall
{
    public:
        using TTimerQueue = ACE_Timer_Queue_T<Custom_Handler *, Custom_Handler_Upcall, ACE_Null_Mutex>;

        // Default constructor
        Custom_Handler_Upcall () = default;

        // Destructor.
        ~Custom_Handler_Upcall () = default;

        Custom_Handler_Upcall (Custom_Handler_Upcall const &) = delete;
        Custom_Handler_Upcall (Custom_Handler_Upcall &&) = delete;
        Custom_Handler_Upcall &operator= (Custom_Handler_Upcall const &) = delete;
        Custom_Handler_Upcall &operator= (Custom_Handler_Upcall &&) = delete;

        // This method is called when a timer is registered.
        int registration (TTimerQueue &timer_queue,
                          Custom_Handler *handler,
                          void const *arg);

        // This method is called before the timer expires.
        int preinvoke (TTimerQueue &timer_queue,
                      Custom_Handler *handler,
                      void const *arg,
                      int recurring_timer,
                      ACE_Time_Value const &cur_time,
                      void const *&upcall_act);

        // This method is called when the timer expires.
        int timeout (TTimerQueue &timer_queue,
                     Custom_Handler *handler,
                     void const *arg,
                     int recurring_timer,
                     ACE_Time_Value const &cur_time);

        // This method is called after the timer expires.
        int postinvoke (TTimerQueue &timer_queue,
                       Custom_Handler *handler,
                       void const *arg,
                       int recurring_timer,
                       ACE_Time_Value const &cur_time,
                       void const *upcall_act);

        // This method is called when a handler is canceled
        int cancel_type (TTimerQueue &timer_queue,
                        Custom_Handler *handler,
                        int dont_call,
                        int &requires_reference_counting);

        // This method is called when a timer is canceled
        int cancel_timer (TTimerQueue &timer_queue,
                         Custom_Handler *handler,
                         int dont_call,
                         int requires_reference_counting);

        // This method is called when the timer queue is destroyed and
        // the timer is still contained in it
        int deletion (TTimerQueue &timer_queue,
                     Custom_Handler *handler,
                     void const *arg);
};

#endif
