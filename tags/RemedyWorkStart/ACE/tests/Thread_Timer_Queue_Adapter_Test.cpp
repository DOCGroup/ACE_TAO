// $Id$

/**
 *  @file    Thread_Timer_Queue_Adapter_Test.cpp
 *
 *  @author Alon Diamant <diamant.alon@gmail.com>
 *
 * This test verifies the functionality of the ACE_Timer_Queue_Thread_Adapter.
 * It also shows the usage of custom event handlers.
 *
 */
//=============================================================================

#include "ace/Timer_Wheel.h"
#include "ace/Timer_Queue_Adapters.h"
#include "test_config.h"

#if defined (ACE_HAS_THREADS)

/// ICustomEventHandler
///
/// Used to demonstrate the usage of custom event handlers. This is called by the
/// timer queue thread adapter.
class ICustomEventHandler
{
    public:

        /// Default constructor.
        ///
        /// @return
        ICustomEventHandler()
        {
        }

        /// Default destructor.
        ///
        /// @return
        virtual ~ICustomEventHandler()
        {
        }

        /// Main functor method.
        ///
        /// @return int
        /// @param p_vParameter
        virtual int operator() (void* p_vParameter) = 0;
};

/// CCustomEventHandlerUpcall
///
/// Implements the Upcall interface used by the ACE_Timer_Queue, specifically for the
/// ICustomEventHandler interface.
class CCustomEventHandlerUpcall
{
    public:

        typedef ACE_Timer_Queue_T<ICustomEventHandler*,
                                  CCustomEventHandlerUpcall,
                                  ACE_Null_Mutex> TTimerQueue;

        /// Default constructor
        CCustomEventHandlerUpcall()
        {
        }

        /// Destructor.
        ~CCustomEventHandlerUpcall()
        {
        }

        /// This method is called when the timer expires.
        int timeout (TTimerQueue&,
                     ICustomEventHandler* p_Handler,
                     const void* p_vParameter,
                     int /*recurring_timer*/,
                     const ACE_Time_Value& /*cur_time*/)
        {
            ACE_TRACE("timeout");

            return (*p_Handler)(const_cast<void*> (p_vParameter));
        }

        /// This method is called when a timer is registered.
        int registration(TTimerQueue&, ICustomEventHandler*, const void*) { return 0; }

        /// This method is called before the timer expires.
        int preinvoke(TTimerQueue&, ICustomEventHandler*, const void*,
            int, const ACE_Time_Value&, const void*&) { return 0; }

        /// This method is called after the timer expires.
        int postinvoke(TTimerQueue&, ICustomEventHandler*, const void*,
            int, const ACE_Time_Value&, const void*) { return 0; }

        /// This method is called when a handler is canceled
        int cancel_type(TTimerQueue&, ICustomEventHandler*, int, int&) { return 0; }

        /// This method is called when a timer is canceled
        int cancel_timer(TTimerQueue&, ICustomEventHandler* p_Handler, int, int)
        {
            ACE_TRACE("cancel_timer");
            delete p_Handler;
            return 0;
        }

        /// This method is called when the timer queue is destroyed and
        /// the timer is still contained in it
        int deletion(TTimerQueue&, ICustomEventHandler* p_Handler, const void*)
        {
            ACE_TRACE("deletion");
            delete p_Handler;
            return 0;
        }
};

/// ICustomEventHandler
///
/// Used to demonstrate the usage of custom event handlers. This is called by the
/// timer queue thread adapter.
class CTestEventHandler : public ICustomEventHandler
{
    public:

        /// Default constructor.
        ///
        /// @return
        CTestEventHandler(long* p_iCallCount)
        : m_p_iCallCount(p_iCallCount)
        {
            ACE_DEBUG((LM_DEBUG, ACE_TEXT("%I(%t) Initializing test event handler.\n")));
        }

        /// Default destructor.
        ///
        /// @return
        virtual ~CTestEventHandler()
        {
            ACE_DEBUG((LM_DEBUG, ACE_TEXT("%I(%t) Destroying test event handler.\n")));
        }

        /// Main functor method.
        ///
        /// @return int
        /// @param p_vParameter
        virtual int operator() (void* p_vParameter)
        {
            long iParameter = (long) p_vParameter;

            ACE_DEBUG((LM_DEBUG,
                       ACE_TEXT("%I(%t) Incrementing test event handler call count by %d.\n"),
                       iParameter));

            m_Mutex.acquire();
            *m_p_iCallCount += iParameter;
            m_Mutex.release();

            // Success
            return 0;
        }

    private:

        long* m_p_iCallCount;
        ACE_Thread_Mutex m_Mutex;
};

// Used for the actual timer queue thread adapter
typedef ACE_Timer_Wheel_T <ICustomEventHandler*,
                           CCustomEventHandlerUpcall,
                           ACE_Null_Mutex> TTimerWheel;
typedef ACE_Timer_Wheel_Iterator_T <ICustomEventHandler*,
                                    CCustomEventHandlerUpcall,
                                    ACE_Null_Mutex> TTimerWheelIterator;
typedef ACE_Thread_Timer_Queue_Adapter<TTimerWheel,
                                       ICustomEventHandler*> TTimerWheelThreadAdapter;

#endif /* ACE_HAS_THREADS */

int
run_main (int, ACE_TCHAR *[])
{
    ACE_START_TEST (ACE_TEXT ("Thread_Timer_Queue_Adapter_Test"));

#if defined (ACE_HAS_THREADS)

    // Start the thread adapter
    TTimerWheelThreadAdapter TimerWheelThreadAdapter;
    TimerWheelThreadAdapter.activate();

    // Single timer
    {
        // Create a test event handler
        long iCallCount = 0;
        CTestEventHandler* p_TestEventHandler = 0;
        ACE_NEW_RETURN(p_TestEventHandler, CTestEventHandler(&iCallCount), -1);

        ACE_DEBUG((LM_DEBUG,
            ACE_TEXT("%I(%t) Scheduling timer...\n")));

        TimerWheelThreadAdapter.schedule(p_TestEventHandler,
            (void*) 1,
            ACE_OS::gettimeofday() + ACE_Time_Value(1, 0));

        ACE_OS::sleep(ACE_Time_Value(1, 100 * 1000));
        ACE_TEST_ASSERT(iCallCount == 1);

        delete p_TestEventHandler;

        ACE_DEBUG((LM_DEBUG, ACE_TEXT("%I(%t) Success in Single timer test.\n")));
    }

    // Single timer with cancellation
    {
        // Create a test event handler
        long iCallCount = 0;
        CTestEventHandler* p_TestEventHandler = 0;
        ACE_NEW_RETURN(p_TestEventHandler, CTestEventHandler(&iCallCount), -1);

        ACE_DEBUG((LM_DEBUG,
            ACE_TEXT("%I(%t) Scheduling timer...\n")));

        long lTimerHandle =
            TimerWheelThreadAdapter.schedule(p_TestEventHandler,
                (void*) 1,
                ACE_OS::gettimeofday() + ACE_Time_Value(1, 0));

        // Cancel the repeating timer
        TimerWheelThreadAdapter.cancel(lTimerHandle);

        ACE_OS::sleep(ACE_Time_Value(1, 100 * 1000));

        ACE_TEST_ASSERT(iCallCount == 0);

        // Test event handler was deleted by the timer.

        ACE_DEBUG((LM_DEBUG, ACE_TEXT("%I(%t) Success in Single timer with cancellation test.\n")));
    }

    // Repeating timer with cancellation
    {
        // Create a test event handler
        long iCallCount = 0;
        CTestEventHandler* p_TestEventHandler = 0;
        ACE_NEW_RETURN(p_TestEventHandler, CTestEventHandler(&iCallCount), -1);

        ACE_DEBUG((LM_DEBUG,
                   ACE_TEXT("%I(%t) Scheduling timer...\n")));

        long lTimerHandle =
            TimerWheelThreadAdapter.schedule
                (p_TestEventHandler,
                 (void*) 1,
                 ACE_OS::gettimeofday() + ACE_Time_Value(1, 0),
                 ACE_Time_Value(1, 0));

        ACE_OS::sleep(ACE_Time_Value(3, 500 * 1000));
        ACE_TEST_ASSERT(iCallCount == 3);

        // Cancel the repeating timer
        TimerWheelThreadAdapter.cancel(lTimerHandle);

        ACE_TEST_ASSERT(iCallCount == 3);

        ACE_DEBUG((LM_DEBUG,
                   ACE_TEXT("%I(%t) Success in Repeating timer with cancellation test.\n")));
    }

    // Multiple timers
    {
        // Create a test event handler
        long iCallCount = 0;
        CTestEventHandler* p_TestEventHandler = 0;
        ACE_NEW_RETURN(p_TestEventHandler, CTestEventHandler(&iCallCount), -1);

        ACE_DEBUG((LM_DEBUG,
                   ACE_TEXT("%I(%t) Scheduling timer...\n")));

        TimerWheelThreadAdapter.schedule
                (p_TestEventHandler,
                 (void*) 1,
                 ACE_OS::gettimeofday() + ACE_Time_Value(1, 0));

        TimerWheelThreadAdapter.schedule
                (p_TestEventHandler,
                 (void*) 1,
                 ACE_OS::gettimeofday() + ACE_Time_Value(1, 0));

        TimerWheelThreadAdapter.schedule
                (p_TestEventHandler,
                 (void*) 1,
                 ACE_OS::gettimeofday() + ACE_Time_Value(2, 0));

        ACE_OS::sleep(ACE_Time_Value(3, 0));
        ACE_TEST_ASSERT(iCallCount == 3);

        delete p_TestEventHandler;

        ACE_DEBUG((LM_DEBUG, ACE_TEXT("%I(%t) Success in Multiple timers test.\n")));
    }

#else
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */
    ACE_END_TEST;

    return 0;
}
