// $Id$

#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Connector.h"
#include "ace/Log_Msg.h"
#include "ace/CDR_Stream.h"
#include "ace/High_Res_Timer.h"
#include "ace/OS_Memory.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

#include "ace/os_include/netinet/os_tcp.h"
#include "ace/os_include/arpa/os_inet.h"

// make sure that the code compiles cleanly even if SCTP is not
// available. If SCTP is not installed, program will exit early in
// main() with an error message
#ifdef ACE_HAS_SCTP
extern "C" {
#include <netinet/sctp.h>
};
#else
#ifndef IPPROTO_SCTP
#define IPPROTO_SCTP 132
#endif
#define SCTP_NODELAY -1
#endif

// class that manages setting of options
#include "Options_Manager.h"

// histogram code
#include "hist.h"

// global constants
ACE_CDR::UShort const primerIterations = 100;
// ace/High_Res_Timer.h describes the need for the following.
ACE_UINT32 const microsec_clock_scale_factor = ACE_High_Res_Timer::global_scale_factor();

// forward declations of functions. Bodies follow main() to improve
// file readability.
ACE_SCTP::HIST runTest(ACE_SOCK_Stream &);

int ACE_TMAIN (int argc, ACE_TCHAR **argv){

  // Initialize the options manager
  Options_Manager optsMgr(argc, argv, ACE_TEXT ("client-opts"));

  // show usage if requested
  if (optsMgr._usage) {
    optsMgr._show_usage(stderr, ACE_TEXT ("client-opts"));
    return 1;
  }

  // If SCTP is not installed then terminate the program, unless TCP
  // was specified.
#ifndef ACE_HAS_SCTP
  if (optsMgr.test_transport_protocol == IPPROTO_SCTP)
    ACE_ERROR_RETURN((LM_ERROR,
                      ACE_TEXT ("SCTP was NOT installed when this binary was compiled.\nSOCK_STREAM_clt may still be run using TCP via the '-t tcp' option.\n")),
                     1);
#endif

  // check that valid options were specified
  if (optsMgr._error) {
    ACE_OS::fprintf (stderr, "ERROR: %s\n", ACE_TEXT_ALWAYS_CHAR (optsMgr._error_message));
    return 1;
  }


  // Create the address that we want the client to connect to
  ACE_INET_Addr serverAddr(Options_Manager::server_port,
                           Options_Manager::server_host);

  // Create the address that we want the client to connect FROM
  ACE_INET_Addr clientAddr(Options_Manager::client_port,
                           Options_Manager::client_connect_addr);

  // object that manages the connection to the server
  ACE_SOCK_Connector connector;

  // object that manages the data xfer between the client and server
  ACE_SOCK_Stream dataStream;

  // connect to the server
  if (connector.connect (dataStream,
                         serverAddr,
                         0,clientAddr, 0, 0, 0, // ALL DEFAULT ARGUMENTS
                         Options_Manager::test_transport_protocol) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) %p\n"),
                       ACE_TEXT ("connection failed")),
                      1);

  // run the test
  ACE_SCTP::HIST testResultsHistogram = 0;
  // connection is closed by runTest* functions
  testResultsHistogram = runTest(dataStream);

  // report out the test statistics.
  // all histograms created are placed onto a linked list that report()
  // can access. So the histogram created in one of the *_test() functions
  // will be reported out by the report() call
  if (testResultsHistogram)
    ACE_SCTP::report();

  return 0;
}

  // create a histogram to store test results
  ACE_SCTP::HIST createHistogram(ACE_CDR::ULong messageSize){

  // The histogram created below lives beyond the scope of this
  // function. So the memory allocated here cannot be cleaned up when
  // this function goes out of scope. Unfortunately the histogram
  // implementation does not clean it up either. So it is a small
  // memory leak.
  char * histName = (char *) malloc(200);

  ACE_OS::sprintf(histName, "%s Unmarshalled Msg Synchronous Latency Test\n\t\t\t\t\t(Message Size %u, Message Type octet)",
                  "ACE", messageSize);

  // actually create the histogram
  ACE_SCTP::HIST createdHist = ACE_SCTP::histogram(histName,
                               Options_Manager::histogram_bin_count,
                               Options_Manager::histogram_min_bin,
                               Options_Manager::histogram_max_bin);

  // set the maximum number of outliers to maintain in the histogram
  ACE_SCTP::set_outer(Options_Manager::histogram_num_outliers, createdHist);

  return (createdHist);
}

// send the test header (only contains number of iterations)
int sendHeader(ACE_SOCK_Stream & stream) {

  // create an ACE CDR output stream and place the header information
  // into it
  ACE_OutputCDR hdrCDR;
    hdrCDR << ACE_OutputCDR::from_boolean (ACE_CDR_BYTE_ORDER);
    hdrCDR << ACE_CDR::ULong(Options_Manager::test_iterations+primerIterations);
  if (!hdrCDR.good_bit())
    return (0);

  // send the header to the server (HEADER IS 8 BYTES LONG)
  size_t bt;
  if (stream.send_n(hdrCDR.begin(), 0,  &bt) == -1)
    return 0;

  return 1;
}

// conduct the UnMarshalled Octet performance test using separate
// send_n calls with Nagle's algorithm disabled
ACE_SCTP::HIST runUnmarshalledOctetTest(ACE_CDR::Octet *buf, size_t seqLen, ACE_SOCK_Stream & stream){

  ACE_CDR::ULong const testIterations = Options_Manager::test_iterations;

  size_t bt;
  ACE_CDR::ULong cnt = 0;
  // variables for the timing measurements
  ACE_hrtime_t startTime, endTime;
  ACE_CDR::Double messageLatency_usec = 0.0;
  ACE_CDR::ULong msgLen = seqLen*ACE_CDR::OCTET_SIZE;

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
    if (-1 == stream.set_option(IPPROTO_SCTP, SCTP_NODELAY, &nagle, sizeof nagle))
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p\n",
                         "set_option"),
                        0);
  } else {
    // tcp case
    if (-1 == stream.set_option(IPPROTO_TCP, TCP_NODELAY, &nagle, sizeof nagle))
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p\n",
                         "set_option"),
                        0);
  }

  // prime the client and server before starting the test
  for(cnt=0;cnt<primerIterations;++cnt){

    // send message size
    // TODO : The message length should be CDR encoded
    ACE_CDR::ULong msgLenExpressed = ACE_HTONL(msgLen);
    if (-1 == stream.send_n (&msgLenExpressed, ACE_CDR::LONG_SIZE, 0, &bt))
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p\n",
                         "send_n"),
                        0);

    // send a message
    if (-1 == stream.send_n (buf, msgLen, 0, &bt))
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p\n",
                         "send_n"),
                        0);

    // block for a Short reply
    ACE_CDR::Short reply;
    if ((stream.recv_n(&reply, ACE_CDR::SHORT_SIZE, 0, &bt)) == -1)
      ACE_ERROR_RETURN((LM_ERROR,
                        "%p\n",
                        "recv_n"),
                       0);
  }

  // AFTER PRIMING THE PUMP CREATE THE HISTOGRAM
  ACE_SCTP::HIST aceStream_hist = 0;
  aceStream_hist = createHistogram(msgLen);
  if (0 == aceStream_hist)
    ACE_ERROR_RETURN((LM_ERROR,
                      "%p\n",
                      "histogram create failed"),
                     0);

  iovec iov[2];
  // PERFORMANCE TEST LOOP
  for (cnt = 0; cnt < testIterations; ++cnt){

    // get the start time
    startTime = ACE_OS::gethrtime();
    if (!startTime)
      ACE_ERROR_RETURN((LM_ERROR,
                        "%p\n",
                        "ACE_OS::gethrtime()"),
                       0);


    ACE_CDR::ULong msgLenExpressed = ACE_HTONL(msgLen);
    iov[0].iov_base = reinterpret_cast<char *> (&msgLenExpressed);
    iov[0].iov_len = ACE_CDR::LONG_SIZE;
    iov[1].iov_base = reinterpret_cast<char *> (buf);
    iov[1].iov_len = msgLen;

    if (-1 == stream.sendv_n (iov, 2))
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p\n",
                         "send_n"),
                        0);

    // block for a Short reply
    ACE_CDR::Short reply;
    if ((stream.recv_n(&reply, ACE_CDR::SHORT_SIZE, 0, &bt)) == -1)
      ACE_ERROR_RETURN((LM_ERROR,
                        "%p\n",
                        "recv_n"),
                       0);

    // get the end time
    endTime = ACE_OS::gethrtime();
    if (!endTime)
      ACE_ERROR_RETURN((LM_ERROR,
                        "%p\n",
                        "ACE_OS::gethrtime()"),
                       0);

    // compute the message latency in micro-seconds
    messageLatency_usec =

      (static_cast<double> (ACE_UINT64_DBLCAST_ADAPTER(endTime)) -
       static_cast<double> (ACE_UINT64_DBLCAST_ADAPTER(startTime)))

      / microsec_clock_scale_factor;

    // record the message latency in the histogram
    ACE_SCTP::record(messageLatency_usec, aceStream_hist);
  }

  // THE HEADER MESSAGE SENT TO THE SERVER CONTAINED THE NUMBER OF
  // PRIMER AND TEST MESSAGES TO BE SENT AFTER WHICH THE SERVER WILL
  // CLOSE THE STREAM SO ONCE WE REACH THIS POINT THE STREAM IS NO
  // LONGER VALID AND WE CLOSE IT.
  stream.close();

  // allocated by runTest
  delete[] buf;
  return aceStream_hist;
}

// sends the test information to the server and calls the correct test
// function.
ACE_SCTP::HIST runTest(ACE_SOCK_Stream & stream)
{

  size_t msgLen = 1;
  for (int i=1; i <= Options_Manager::payload_size_power_of_2; i++)
    msgLen *= 2;

  // send a header to the server that contains test parameters
  if (sendHeader(stream) < 0)
    ACE_ERROR_RETURN((LM_ERROR,
                      "Could NOT Send CDR Encoded Header"),
                     0);

  // create the forward message buffer

  ACE_CDR::Octet *o_arr;
  ACE_CDR::ULong seqLen = msgLen/ACE_CDR::OCTET_SIZE;
  ACE_NEW_RETURN(o_arr,
                 ACE_CDR::Octet[seqLen],
                 0);
  // o_arr is deleted in run_octet_test
  return(runUnmarshalledOctetTest(o_arr, seqLen, stream));
}
