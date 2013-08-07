/*
 * ACE reactor demonstration
 *
 * $Id$
 * Date: 26-Jan-2006
 */

#include "common.h"
#include "ReadHandler.h"

#include <ace/streams.h>
#include <ace/Time_Value.h>
#include <ace/Log_Msg.h>

/**
 * This macro is used to increase the invocation counter by one when entering
 * handle_input(). It also checks wether the counter is greater than zero
 * indicating, that handle_input() has been called before.
 */
#define INVOCATION_ENTER() do { if (mInvocationCounter > 0) \
        ACE_ERROR((LM_ERROR, ACE_TEXT("Multiple invocations detected.\n"))); \
        mInvocationCounter++; } while (0)

/**
 * THis macro is the counter part to INVOCATION_ENTER(). It decreases the
 * invocation counter and then returns the given value. This macro is
 * here for convenience to decrease the invocation counter also when returning
 * due to errors.
 */
#define INVOCATION_RETURN(retval) do { mInvocationCounter--; \
                                    return retval; } while(0)

ReadHandler::ReadHandler() : ACE_Event_Handler(), mStream(), mDataSize(0),
        mData(0), mCallCounter(0), mInvocationCounter(0) {
    ACE_TRACE("ReadHandler::ReadHandler()");
}

ReadHandler::~ReadHandler() {
    ACE_TRACE("ReadHandler::~ReadHandler()");

    if (mStream.close() == -1)
      ACE_ERROR((LM_ERROR, ACE_TEXT("%N:%l: Failed to close socket. ")
                 ACE_TEXT ("(errno = %i: %m)\n"), ACE_ERRNO_GET));

    delete[] mData;
}

ACE_SOCK_Stream &ReadHandler::getStream(void) {
    ACE_TRACE("ReadHandler::getStream(void)");
    return mStream;
}

ACE_HANDLE ReadHandler::get_handle(void) const {
    ACE_TRACE("ReadHandler::get_handle(void)");
    return mStream.get_handle();
}

int ReadHandler::handle_input(ACE_HANDLE) {
    ACE_TRACE("ReadHandler::handle_input(ACE_HANDLE)");

    INVOCATION_ENTER();

    // the response sent to the client
    char response = 0;

    if (mCallCounter == 0) {

        /*
         * This is the first request from the client.
         */

        // increase the call counter so the next client request goes to else-if
        mCallCounter++;

        // get the desired size from the client
        // Note: only use the sizeof and pointer to int on compatible
        //       platforms (i.e. little-endian/big-endian, data type size)
        if (mStream.recv_n(&mDataSize, sizeof(mDataSize),
                &connTimeout) != (ssize_t) sizeof(mDataSize)) {
          ACE_ERROR((LM_ERROR, ACE_TEXT("%N:%l: Failed to receive ")
                     ACE_TEXT ("request. (errno = %i: %m)\n"), ACE_ERRNO_GET));
            INVOCATION_RETURN(-1);
        }

        // The verbose debug output is replaced with some unintrusive dots.
        // This increases visibility of the desired effect.
        // ACE_DEBUG((LM_DEBUG, ACE_TEXT("%@: Data size: %i\n"), this, mDataSize));
        ACE_DEBUG((LM_DEBUG, ACE_TEXT(".")));

        // check mDataSize for plausability then allocate memory
        if (mDataSize > 0) {
            mData = new (std::nothrow) char[mDataSize];
            if (mData == 0)
              ACE_DEBUG((LM_DEBUG, ACE_TEXT("%N:%l: Failed to allocate ")
                         ACE_TEXT ("data buffer.\n")));
            else
                response = 'K';
        }

        // send the response to the client (which is still 0, if the
        // allocation did not succeed)
        if (mStream.send_n(&response, sizeof(response), &connTimeout) != 1) {
          ACE_ERROR((LM_ERROR, ACE_TEXT("%N:%l: Failed to send ")
                     ACE_TEXT ("response. (errno = %i: %m)\n"), ACE_ERRNO_GET));
            INVOCATION_RETURN(-1);
        }

        if (response == 'K')
            INVOCATION_RETURN(0); // get another request from the same client
        else
            INVOCATION_RETURN(-1); // the client will not send data if response != 'K'

    } else if (mCallCounter == 1) {

        /*
         * This is the second request from the client.
         */

        // increase the call counter, this read handler should not be called
        // again
        mCallCounter++;

        // receive the data from the client
        if (mStream.recv_n(mData, mDataSize, &connTimeout) != mDataSize) {
          ACE_ERROR((LM_ERROR, ACE_TEXT("%N:%l: Failed to receive data.")
                     ACE_TEXT ("(errno = %i: %m)\n"), ACE_ERRNO_GET));
            INVOCATION_RETURN(-1);
        }

        response = 'K';

        if (mStream.send_n(&response, 1, &connTimeout) != 1) {
          ACE_ERROR((LM_ERROR, ACE_TEXT("%N:%l: Failed to send ")
                     ACE_TEXT ("confirmation. (errno = %i: %m)\n"), ACE_ERRNO_GET));
            INVOCATION_RETURN(-1);
        }

        INVOCATION_RETURN(-1); // ask for removal, since client does not send any more data
    }

    // this is to find strange actions with the call counter
    ACE_ERROR((LM_ERROR, ACE_TEXT("%N:%l: We should not get here.")));
    INVOCATION_RETURN(-1);
}

int ReadHandler::handle_close(ACE_HANDLE, ACE_Reactor_Mask) {
  ACE_TRACE("ReadHandler::handle_close(ACE_HANDLE, ACE_Reactor_Mask)");

    delete this;
    return 0;
}

