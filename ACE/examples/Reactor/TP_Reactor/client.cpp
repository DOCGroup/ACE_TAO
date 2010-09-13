/*
 * ACE reactor demonstration
 *
 * $Id$
 * Date: 26-Jan-2006
 */

#include <ace/Auto_Ptr.h>
#include <ace/INET_Addr.h>
#include <ace/Log_Msg.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/SOCK_Connector.h>
#include <ace/SOCK_Stream.h>
#include <ace/streams.h>

#include "common.h"

/**
 * Print usage information for the client.
 *
 * @param arg The progams name (argv[0]).
 */
int printUsage(ACE_TCHAR *arg) {
    cerr << "Usage: " << arg << " size [count]" << endl;
    cerr << "\tSends <size> MiB to the server and optionally repeats that "
         << "<count> times." << endl;
    cerr << "\tAll arguments must be positive numbers. If no <count> is "
         << "given, the\n\tclient runs until interrupted." << endl;
    return -1;
}

int ACE_TMAIN(int argc, ACE_TCHAR **argv) {

    // size and count for transmissions
    int size = 0, count = -1;

    // the server's answer is a single byte
    char answer;

    // parse the <size> argument
    if ((argc < 2) || (((size = ACE_OS::strtol(argv[1], 0, 10)) < 1) ||
            (errno == EINVAL)))
        return printUsage(argv[0]);

    // take size as the number of MiB and create appropriate buffer
    size *= BASE;
    char *someData = new (std::nothrow) char[size];

    if (someData == 0)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%N:%l: Failed to allocate ")
                         ACE_TEXT ("data buffer.\n")), -1);

    // put someData in an auto_ptr so it gets deleted automatically
    auto_ptr<char> pSomeData(someData);

    // parse the <count> argument if available
    if ((argc == 3) && (((count = ACE_OS::strtol(argv[2], 0, 10)) < 1) ||
            (errno == EINVAL)))
        return printUsage(argv[0]);

    // the server listens on localhost on default port (from common.h)
    ACE_INET_Addr serverAddr(PORT, "localhost");

    ACE_SOCK_Stream stream;
    ACE_SOCK_Connector connector;

    // -1 is running indefinitely
    while ((count == -1) || (count-- != 0)) {

        // some output, that we know something is happening
        //ACE_DEBUG((LM_DEBUG, ACE_TEXT("%N:%l: Passes left: %i\n"), count));
        ACE_DEBUG((LM_DEBUG, ACE_TEXT(".")));

        // connect to the server and get the stream
        if (connector.connect(stream, serverAddr) == -1) {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT("%N:%l: Failed to connect to ")
                     ACE_TEXT ("server. (errno = %i: %m)\n"), ACE_ERRNO_GET));
            break;
        }

        try {

            // send the request to the server (number of MiB in the next call)
            // Note: only use the sizeof and pointer to int on compatible
            //       platforms (i.e. little-endian/big-endian, data type size)
            if (stream.send_n(&size, sizeof(size), &connTimeout) != sizeof(size)) {
                ACE_ERROR((LM_ERROR, ACE_TEXT("%N:%l: Failed to  send ")
                           ACE_TEXT ("request. (errno = %i: %m)\n"), ACE_ERRNO_GET));
                throw 1;
            }

            // receive the answer
            if (stream.recv_n(&answer, sizeof(answer), &connTimeout) != 1) {
              ACE_ERROR((LM_ERROR, ACE_TEXT("%N: %l: Failed to receive ")
                         ACE_TEXT ("1st response. (errno = %i: %m)\n"), ACE_ERRNO_GET));
                throw 1;
            }

            // server answer, 'K" indicates a positive answer
            if (answer == 'K') {

                // send a huge message to the server
                if (stream.send_n(someData, size, &connTimeout) != size) {
                  ACE_ERROR((LM_ERROR, ACE_TEXT("%N:%l: Failed to send ")
                             ACE_TEXT ("someData. (errno = %i: %m)\n"), ACE_ERRNO_GET));
                    throw 1;
                }

                // get an answer
                if (stream.recv_n(&answer, sizeof(answer), &connTimeout) != 1) {
                  ACE_ERROR((LM_ERROR, ACE_TEXT("%N: %l: Failed to receive ")
                             ACE_TEXT ("2nd response. (errno = %i: %m)\n"), ACE_ERRNO_GET));
                    throw 1;
                }

                // check the answer
                if (answer != 'K') {
                    cout << "The server was unable to process the data."
                         << endl;
                }
            }
        } catch (...) {
            // ok we know an error occurred, we need to close the socket.
            // The we'll try again.
        }

        // close the current stream
        if (stream.close() == -1) {
          ACE_ERROR((LM_ERROR, ACE_TEXT("%N:%l: Failed to close ")
                     ACE_TEXT ("socket. (errno = %i: %m)\n"), ACE_ERRNO_GET));
            break;
        }
    } // while

    cout << "Bye. Bye" << endl;
    return 0;
}

