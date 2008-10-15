/*
 * ACE reactor demonstration
 *
 * $Id$
 * Date: 26-Jan-2006
 */

#ifndef __READHANDLER_H__
#define __READHANDLER_H__

#include <ace/Event_Handler.h>
#include <ace/SOCK_Stream.h>

/**
 * This read handler is created by the accept handler and handles all the data
 * exchange between client and server. The client makes two requests to the
 * server. The first asks the server to create a buffer which will hold the
 * data sent in the second call.
 */
class ReadHandler : public ACE_Event_Handler {

    private:

        /**
         * The stream socket used for data exchange.
         */
        ACE_SOCK_Stream mStream;

        /**
         * The size of the data array.
         */
        int mDataSize;

        /**
         * The array containing the client's data.
         */
        char *mData;

        /**
         * The call counter to distinguish between first and second call.
         */
        int mCallCounter;

        /**
         * Count the numer of invocations of handle_*(). According to the
         * docs, there should be only one invocation at any given time.
         */
        int mInvocationCounter;

    public:

        /**
         * Initialization.
         */
        ReadHandler(void);

        /**
         * Clean up data.
         */
        virtual ~ReadHandler();

        /**
         * Provide access to the internal stream socket.
         */
        ACE_SOCK_Stream &getStream(void);

        /**
         * @name Overridden methods from the ACE_Event_Handler
         */
        // @{

        /**
         * Provides the handle of mStream;
         */
        virtual ACE_HANDLE get_handle(void) const;

        /**
         * Handles the data excahnge between client and server. On the first
         * invocation, mData is allocated to the requested size and on the
         * second invocation, that buffer is filled with the client's data.
         */
        virtual int handle_input(ACE_HANDLE = ACE_INVALID_HANDLE);

        /**
         * Deletes this instance of the read handler.
         */
        virtual int handle_close(ACE_HANDLE, ACE_Reactor_Mask);
        // @}
};

#endif /* __READHANDLER_H__ */

