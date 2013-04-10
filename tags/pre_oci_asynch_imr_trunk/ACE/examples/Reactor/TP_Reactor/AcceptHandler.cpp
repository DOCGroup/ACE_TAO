/*
 * $Id$
 *
 * ACE reactor demonstration
 *
 * Date: 26-Jan-2006
 */

#include "common.h"
#include "AcceptHandler.h"
#include "ReadHandler.h"

#include <ace/Auto_Ptr.h>
#include <ace/INET_Addr.h>
#include <ace/Log_Msg.h>


AcceptHandler:: AcceptHandler(ACE_Reactor *reactor) :
        ACE_Event_Handler(),
        mReactor(reactor == 0 ? ACE_Reactor::instance() : reactor),
        mAcceptor() {
    ACE_TRACE("AcceptHandler:: AcceptHandler(ACE_Reactor *)");
}

AcceptHandler::~AcceptHandler() {
    ACE_TRACE("AcceptHandler::~AcceptHandler()");
}

int AcceptHandler::open(void) {
    ACE_TRACE("AcceptHandler::open(void)");

    // create the local address used for the service (PORT is from common.h)
    ACE_INET_Addr addr(PORT);

    // open a port using the acceptor; reuse the address later
    if (mAcceptor.open(addr, 1) == -1)
      ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("%N:%l: Failed to open ")
                        ACE_TEXT ("listening socket. (errno = %i: %m)\n"), ACE_ERRNO_GET), -1);

    // register the handler with the reactor
    if (mReactor->register_handler(this,
            ACE_Event_Handler::ACCEPT_MASK) == -1) {
      ACE_ERROR((LM_ERROR, ACE_TEXT("%N:%l: Failed to register accept ")
                 ACE_TEXT ("handler. (errno = %i: %m)\n"), ACE_ERRNO_GET));

        // don't leave the acceptor open
        if (mAcceptor.close() == -1)
          ACE_ERROR((LM_ERROR, ACE_TEXT("%N:%l: Failed to close the socket ")
                     ACE_TEXT ("after previous error. (errno = %i: %m)\n"),
                        ACE_ERRNO_GET));
        return -1;
    }

    return 0;
}

ACE_HANDLE AcceptHandler::get_handle(void) const {
    ACE_TRACE("AcceptHandler::get_handle(void)");
    return mAcceptor.get_handle();
}

int AcceptHandler::handle_input(ACE_HANDLE) {
    ACE_TRACE("AcceptHandler::handle_input(ACE_HANDLE)");

    ACE_INET_Addr clientAddr;

    // create a new ReadHandler
    ReadHandler *reader = 0;
    ACE_NEW_NORETURN (reader, ReadHandler());
    if (reader == 0)
      ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("%N:%l: Failed to allocate ")
                        ACE_TEXT ("reader. (errno = %i: %m)\n"), ACE_ERRNO_GET), -1);

    // put reader in an auto pointer so we can use ACE_ERROR_RETURN safely
    auto_ptr<ReadHandler> pReader(reader);

    // accept the connection using the reader's stream
    if (mAcceptor.accept(reader->getStream(), &clientAddr) == -1)
      ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("%N:%l: Failed to accept ")
                    ACE_TEXT ("client connection. (errno = %i: %m)\n"), ACE_ERRNO_GET), -1);

    // register the reader with the reactor
    if (mReactor->register_handler(reader,
            ACE_Event_Handler::READ_MASK) == -1)
      ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("%N:%l: Failed to register ")
                        ACE_TEXT ("read handler. (errno = %i: %m)\n"), ACE_ERRNO_GET), -1);

    // from now on the read handler takes care of itself
    pReader.release();

    return 0; // keep going
}

int AcceptHandler::handle_close(ACE_HANDLE, ACE_Reactor_Mask) {
    ACE_TRACE("AcceptHandler::handle_close(ACE_HANDLE, ACE_Reactor_Mask)");

    // close the listening socket
    if (mAcceptor.close() == -1)
      ACE_ERROR((LM_ERROR, ACE_TEXT("%N:%l: Failed to close the ")
                 ACE_TEXT ("socket. (errno = %i: %m)\n"), ACE_ERRNO_GET));

    // no need to distinguish between error during close and normal close
    // since ACE does not evaluate the return value of handle_close()

    delete this;
    return 0;
}

