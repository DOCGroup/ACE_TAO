/*
 * ACE reactor demonstration
 *
 * $Id$
 * Date: 26-Jan-2006
 */

#ifndef __ACCEPTHANDLER_H__
#define __ACCEPTHANDLER_H__

#include <ace/Event_Handler.h>
#include <ace/Reactor.h>
#include <ace/SOCK_Acceptor.h>

/**
 * This accept handler is based on the provided solution from the ACE course.
 */
class AcceptHandler : public ACE_Event_Handler {

    private:

        /**
         * The reactor to which the accept handler belongs.
         */
        ACE_Reactor *mReactor;

        /**
         * The socket used for incoming conections.
         */
        ACE_SOCK_Acceptor mAcceptor;

    public:

        /**
         * @param reactor The reactor which will use this accept handler.
         */
        AcceptHandler(ACE_Reactor *reactor = 0);

        /**
         * The destructor exists for tracing purposes.
         */
        virtual ~AcceptHandler();

        /**
         * Open the listening socket and register the handler with the reactor.
         *
         * @return 0 on success, -1 on failure
         */
        int open(void);

        /**
         * @name Overridden methods from the ACE_Event_Handler
         */
        // @{

        /**
         * Provides the handle of mAcceptor.
         */
        virtual ACE_HANDLE get_handle(void) const;

        /**
         * Create a read handler for the new connection and register that
         * handler with the reactor.
         */
        virtual int handle_input(ACE_HANDLE = ACE_INVALID_HANDLE);

        /**
         * Close the listening socket.
         */
        virtual int handle_close(ACE_HANDLE, ACE_Reactor_Mask);
        // @}
};

#endif /* __ACCEPTHANDLER_H__ */

