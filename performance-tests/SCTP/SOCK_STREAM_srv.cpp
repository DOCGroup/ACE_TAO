// $Id$

#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Log_Msg.h"
#include "ace/Thread_Manager.h"
#include "ace/Handle_Set.h"
#include "ace/CDR_Stream.h"
#include "ace/streams.h"

// make sure that the code compiles cleanly even if SCTP is not
// available. If SCTP is not installed, program will exit early in
// main() with an error message
#ifdef ACE_HAS_SCTP
extern "C" {
#include <netinet/sctp.h>
};
#else
#ifndef IPPROTO_SCTP
#define IPPROTO_SCTP 132 /* always the same value on every platform */ 
#endif
#define SCTP_NODELAY -1
#endif

// class that manages setting of options
#include "Options_Manager.h"

// structure to bundle arguments for thread functions
struct ArgStruct {
  ACE_SOCK_Stream * stream;
  ACE_CDR::ULong numIters;
};

// thread function that serves the client for the UnMarshalled Octet
// test
static ACE_THR_FUNC_RETURN unmarshalledOctetServer (void *arg){

  // unbundle the arguments
  ArgStruct * args = ACE_reinterpret_cast(ArgStruct *,arg);
  ACE_SOCK_Stream * dataModeStream = args->stream;
  ACE_CDR::ULong numIterations = args->numIters;
  delete args;

  // serve the client for numIterations synchronous invocations
  do {

    // READ A MESSAGE FROM THE CLIENT

    size_t bt;
    ACE_CDR::ULong msgBufSize=0;
    // read the size of the buffer to follow
    if ((dataModeStream->recv_n(&msgBufSize, ACE_CDR::LONG_SIZE, 0, &bt)) == -1)
      ACE_ERROR_RETURN((LM_ERROR,
                        "%p\n",
                        "recv_n"),
                       0);
    msgBufSize = ACE_NTOHL(msgBufSize);

    // allocate the buffer for the message payload
    ACE_CDR::Octet * msgBuf=NULL;
    ACE_NEW_RETURN(msgBuf,
                   ACE_CDR::Octet[msgBufSize],
                   0);

    // read the buffer
    if ((dataModeStream->recv_n(msgBuf, msgBufSize, 0, &bt)) == -1)
      ACE_ERROR_RETURN((LM_ERROR,
                        "%p\n",
                        "recv_n"),
                       0);

    // clean up the allocated buffer
    delete[] msgBuf;

    // SEND A REPLY TO THE CLIENT

    // send back a 2 byte reply
    ACE_CDR::Short reply;
    if ((dataModeStream->send_n(&reply, ACE_CDR::SHORT_SIZE, 0, &bt)) == -1)
      ACE_ERROR_RETURN((LM_ERROR,
                        "%p\n",
                        "send_n"),
                       0);

  } while (--numIterations);

  // close and destroy the stream
  dataModeStream->close();
  delete dataModeStream;

  return NULL;
}

// sets up the dataModeSocket Stream, reads the test header infomation
// and launches a thread to handle the requested test.
static void run_server (ACE_HANDLE handle)
{
  ACE_INET_Addr cli_addr;
  // create a new stream and initialized with the handle returned by
  // accept
  ACE_SOCK_Stream * dataModeStream = new ACE_SOCK_Stream;
  dataModeStream->set_handle (handle);

  // Make sure we're not in non-blocking mode.
  if (dataModeStream->disable (ACE_NONBLOCK) == -1){
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "disable"));
    return;
  }
  else if (dataModeStream->get_remote_addr (cli_addr) == -1){
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "get_remote_addr"));
    return;
  }

  // explicity configure Nagling. Default is
  // Options_Manager::test_enable_nagle=0 so default configurations is
  // NO NAGLING
  ACE_CDR::Long nagle;
  if (Options_Manager::test_enable_nagle)
    nagle=0;
  else
    nagle=1;

  if (Options_Manager::test_transport_protocol == IPPROTO_SCTP){
    // default - sctp case
    if (-1 == dataModeStream->set_option(IPPROTO_SCTP, SCTP_NODELAY, &nagle, sizeof nagle)){
      ACE_ERROR((LM_ERROR,
                 "%p\n",
                 "set_option"));
      return;
    }
  } else {
    // tcp case
    if (-1 == dataModeStream->set_option(IPPROTO_TCP, TCP_NODELAY, &nagle, sizeof nagle)){
      ACE_ERROR ((LM_ERROR,
                  "%p\n",
                  "set_option"));
      return;
    }
  }

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) client %s connected from %d\n",
              cli_addr.get_host_name (),
              cli_addr.get_port_number ()));

  // hdr bufSize is hardcoded to 8 bytes
  // (4 for a CDR-encoded boolean and 4 for a CDR-encoded ULong)
  ACE_CDR::ULong hdrBufSize = 8;
  // allocate a raw buffer large enough to receive the header and be
  // properly aligned for the CDR decoding.
  ACE_CDR::Char * hdrBuf= new ACE_CDR::Char[hdrBufSize+ACE_CDR::MAX_ALIGNMENT];
  // align the raw buffer before reading data into it.
  char * hdrBuf_a = ACE_ptr_align_binary(hdrBuf, ACE_CDR::MAX_ALIGNMENT);

  size_t bt;
  // read the header
  if ((dataModeStream->recv_n(hdrBuf_a, hdrBufSize, 0, &bt)) == -1){
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "recv_n"));
    return;
  }

  // pass the CDR encoded data into an ACE_InputCDR class. hdrCDR does
  // NOT copy this data. Nor does it delete. It assumes the buffer
  // remains valid while it is in scope.
  ACE_InputCDR hdrCDR(hdrBuf_a, hdrBufSize);

  ACE_CDR::Boolean byteOrder;
  ACE_CDR::ULong numIterations;

  // extract the data
  hdrCDR >> ACE_InputCDR::to_boolean (byteOrder);
  hdrCDR.reset_byte_order(byteOrder);
  hdrCDR >> numIterations;

  // make sure the stream is good after the extractions
  if (!hdrCDR.good_bit()){
    ACE_ERROR((LM_ERROR,
               "%p\n",
               "hdrCDR"));

    return;
  }

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Test for %u iterations\n",
              numIterations));

  // deallocate the header buffer
  delete[] hdrBuf;

  // bundle up the arguments
  ArgStruct * args = new ArgStruct;
  args->stream = dataModeStream;
  args->numIters = numIterations;

#if defined (ACE_HAS_THREADS)
  // Spawn a new thread and run the new connection in that thread of
  // control using the <server> function as the entry point.
  if (ACE_Thread_Manager::instance ()->spawn (unmarshalledOctetServer,
                                              ACE_reinterpret_cast(void *,args),
                                              THR_DETACHED) == -1)
    ACE_ERROR ((LM_ERROR,
                "(%P|%t) %p\n",
                "spawn"));
#else
  (*unmarshalledOctetServer) (ACE_reinterpret_cast(void *, args));
#endif /* ACE_HAS_THREADS */

}

int main(int argc, char **argv){

  Options_Manager optsMgr(argc, argv, "server-opts");

  // show usage is requested
  if (optsMgr._usage) {
    optsMgr._show_usage(cerr, "server-opts");
    return 1;
  }

  // If SCTP is not installed then terminate the program, unless TCP
  // was specified.
#ifndef ACE_HAS_SCTP
  if (optsMgr.test_transport_protocol == IPPROTO_SCTP)
    ACE_ERROR_RETURN((LM_ERROR,
                      "SCTP was NOT installed when this binary was compiled.\n"
                      "SOCK_STREAM_srv may still be run using TCP "
                      "via the '-t tcp' option.\n"),
                     1);
#endif

  // check that valid options were specified
  if (optsMgr._error) {
    cerr << "ERROR: " << optsMgr._error_message << endl;
    return 1;
  }

  // this is the socket that the server will listen on
  ACE_SOCK_Acceptor acceptor_socket;

  // Create the address that we want to listen for connections on. If
  // server_accept_addr=INADDR_ANY (the default), SCTP will listen for
  // connections on all IP interfaces. If an address is specified,
  // SCTP will listen for connections on that address ONLY.
  ACE_INET_Addr serverAddr(optsMgr.server_port,
                           optsMgr.server_accept_addr);

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) Accepting connections on port %u on interface %s using %s\n",
             serverAddr.get_port_number(),
             (optsMgr.server_accept_addr == INADDR_ANY) ? "INADDR_ANY" : serverAddr.get_host_addr(),
             (optsMgr.test_transport_protocol == IPPROTO_SCTP) ? "IPPROTO_SCTP" : "IPPROTO_TCP"));

  // this operation creates a socket, binds the specified internet
  // address to it and calls listen. As this is a wrapper facade
  // approach, the ACE_OS::{socket,bind,listen} calls are invoked in
  // the implementation of open.
  if (acceptor_socket.open(serverAddr, 1,
                           AF_INET,
                           ACE_DEFAULT_BACKLOG,
                           optsMgr.test_transport_protocol) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open"),
                      1);

  // this function checks that the port that was actually bound was
  // the port we asked for. Apparently some operating systems will
  // automatically select new ports if the specified port is currently
  // used.
  else if (acceptor_socket.get_local_addr(serverAddr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "get_local_addr"),
                      1);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) starting server at port %d\n",
              serverAddr.get_port_number()));

  // this is the stream object that will associated with a completed
  // connection (aka the data mode socket). It will be set when accept
  // is called below.
  ACE_SOCK_Stream new_stream;

  // a file decriptor set
  ACE_Handle_Set handle_set;
  // add the acceptor socket to the file descriptor set.
  handle_set.set_bit(acceptor_socket.get_handle());

  for (;;){

    ACE_Time_Value timeout(ACE_DEFAULT_TIMEOUT);
    ACE_Handle_Set temp = handle_set;

    // wait for connect() call from client. In the original test there
    // were two different acceptor sockets for two different
    // services. So select was needed to wait on both sockets
    // simultaneously. In this test we could just call accept on the
    // one socket.
    int result = ACE_OS::select((int) (acceptor_socket.get_handle()) +1,
                                (fd_set *) temp,
                                0,
                                0,
                                timeout);

    // check that select did not end with an error.
    if (result == -1)
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) %p\n",
                  "select"));
    // check to see if select timed out.
    else if (result == 0){
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) select timed out\n"));

    }
    else { // case where a file descriptor was actually set
      if (!(temp.is_set(acceptor_socket.get_handle()))){
        // CANNOT BE REACHED
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) %p\n",
                    "select: NO ERROR BUT NO FD SET"));
      } else {
        // call accept to set up the new stream.
        if (acceptor_socket.accept(new_stream) == -1) {
          ACE_ERROR ((LM_ERROR,
                      "%p\n",
                      "accept"));
          continue;
        }
        else{
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) spawning server\n"));

        }
        // Run the server.
        run_server (new_stream.get_handle ());
      }

    }

  }
  return 0;
}
