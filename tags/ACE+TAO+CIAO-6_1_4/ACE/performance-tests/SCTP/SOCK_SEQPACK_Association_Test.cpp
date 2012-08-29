
//=============================================================================
/**
 *  @file    SOCK_SEQPACK_Association_Test.cpp
 *
 *  $Id$
 *
 *
 * Tests the methods get_local_addrs and get_remote_addrs of class
 * ACE_SOCK_SEQPACK_Association.
 *
 * This is not an automated "one-button" test.  Rather, it prints
 * some output to a log file, so that an interested human can
 * inspect the output and get a vague notion of whether or not
 * the methods are working properly.
 *
 *
 *  @author Edward Mulholland (emulholl@atl.lmco.com)
 */
//=============================================================================


#include "ace/SOCK_SEQPACK_Association.h"
#include "ace/SOCK_SEQPACK_Connector.h"
#include "ace/INET_Addr.h"
#include "ace/Log_Msg.h"

void dump_names(const ACE_SOCK_SEQPACK_Association& assoc);

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int status = 0;     // Innocent until proven guilty

  // object that manages the connection to the server
  ACE_SOCK_SEQPACK_Connector connector;

  // object that manages the data xfer between the client and server
  ACE_SOCK_SEQPACK_Association dataStream;

  // object that represents the server's IP address and port
  ACE_INET_Addr serverAddr;

  if (argc < 2) {

    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Usage: SOCK_SEQPACK_Association_Test hostname:port\n")));
    status = 1;

  } else if (serverAddr.set(argv[1])) {

    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("ACE_INET_Addr::set")));
    status = 1;

  } else if (connector.connect (dataStream, serverAddr)) {

    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("ACE_SOCK_SEQPACK_Connector::connect")));
    status = 1;

  } else {

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Connected to server at %s\n"),
                argv[1]));

    dump_names(dataStream);
  }

  dataStream.close();

  return status;
}

void dump_names(const ACE_SOCK_SEQPACK_Association& assoc)
{
  // Pre-declare for-loop index
  size_t i = 0;

  size_t in_out_size = 100;
  ACE_INET_Addr in_out[100];

  // Output char buffer
  const size_t outbuf_size = 1024;
  ACE_TCHAR outbuf[outbuf_size];

  // Get local addresses of the association
  if (assoc.get_local_addrs(in_out, in_out_size)) {

    ACE_ERROR((LM_ERROR,
               "%p\n",
               "get_local_addrs"));
    return;
  }

  ACE_DEBUG((LM_DEBUG, "Called get_local_addrs\n"));

  // Print individual results of get_local_addrs
  for (i = 0; i < in_out_size; ++i) {

    if (in_out[i].addr_to_string(outbuf, outbuf_size)) {

      ACE_ERROR((LM_ERROR,
                 "%p\n",
                 "addr_to_string"));
      return;
    }

    ACE_DEBUG((LM_DEBUG,
               "get_local_addrs[%i] = %s\n",
               i,
               outbuf));
  }

  // Reset in_out_size
  in_out_size = 100;

  // Get remote addresses of the association
  if (assoc.get_remote_addrs(in_out, in_out_size)) {

    ACE_ERROR((LM_ERROR,
               "%p\n",
               "get_remote_addrs"));
    return;
  }

  ACE_DEBUG((LM_DEBUG, "Called get_remote_addrs\n"));

  // Print individual results of get_remote_addrs
  for (i = 0; i < in_out_size; ++i) {

    if (in_out[i].addr_to_string(outbuf, outbuf_size)) {

      ACE_ERROR((LM_ERROR,
                 "%p\n",
                 "addr_to_string"));
      return;
    }

    ACE_DEBUG((LM_DEBUG,
               "get_remote_addrs[%i] = %s\n",
               i,
               outbuf));
  }
}


