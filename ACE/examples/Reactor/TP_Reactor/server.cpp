/*
 * ACE reactor demonstration
 *
 * $Id$
 * Date: 26-Jan-2006
 */

#include <ace/Event_Handler.h>
#include <ace/Log_Msg.h>
#include <ace/Reactor.h>
#include <ace/Signal.h>
#include <ace/streams.h>
#include <ace/Thread_Manager.h>
#include <ace/TP_Reactor.h>

#include "AcceptHandler.h"

/**
 * This is the function run by all threads in the thread pool.
 *
 * @param arg is expected to be of type (ACE_Reactor *)
 */
ACE_THR_FUNC_RETURN threadFunc(void *arg) {
    ACE_TRACE("threadFunc(void *)");

    ACE_Reactor *reactor = (ACE_Reactor *) arg;
    reactor->run_reactor_event_loop();

    return 0;
}

/**
 * The main function sets up the TP reactor. The code is basically taken from
 * the solution to exercise 4c of the ACE course.
 */
int ACE_TMAIN(int, ACE_TCHAR **) {

    // create a reactor from a TP reactor
    ACE_TP_Reactor tpReactor;
    ACE_Reactor reactor(&tpReactor);

    // create a new accept handler using that reactor
    AcceptHandler *acceptHandler = 0;
    ACE_NEW_NORETURN (acceptHandler, AcceptHandler(&reactor));
    if (acceptHandler == 0)
      ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("%N:%l: Failed to allocate ")
                        ACE_TEXT ("accept handler. (errno = %i: %m)\n"), ACE_ERRNO_GET), -1);

    // open the accept handler
    if (acceptHandler->open() == -1) {
        delete acceptHandler;
        ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("%N:%l: Failed to open accept ")
                          ACE_TEXT ("handler. Exiting.\n")), -1);
    }

    // spawn some threads which run the reactor event loop(s)
    ACE_Thread_Manager::instance()->spawn_n(9, threadFunc, &reactor);

    // let the thread manager wait for all threads
    ACE_Thread_Manager::instance()->wait();

    ACE_DEBUG((LM_DEBUG, ACE_TEXT("Bye. Bye.\n")));
    return 0;
}

