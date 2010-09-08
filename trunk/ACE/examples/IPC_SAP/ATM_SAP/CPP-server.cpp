// $Id$

#include "ace/OS_main.h"
#include "ace/ATM_Acceptor.h"
#include "ace/ATM_Addr.h"
#include "ace/High_Res_Timer.h"
#include "ace/Log_Msg.h"



#if defined (ACE_HAS_ATM)
// ACE_ATM Server

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Time_Value timeout (ACE_DEFAULT_TIMEOUT);

  //unsigned char selector = ACE_ATM_Addr::DEFAULT_SELECTOR;
  //int selector_specified = 0;

  if (argc > 2)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Usage: %s [selector]\n",
                       argv[0]),
                      1);

  // Create a server address.
  ACE_ATM_Addr addr;
  //if (selector_specified)
  unsigned char selector = ( argc == 2 ) ? ACE_OS::atoi( argv[ 1 ]) : ACE_ATM_Addr::DEFAULT_SELECTOR;
  addr.set_selector( selector );
  ACE_OS::printf( "ATM_Server: selector changed to %d\n", addr.get_selector());


  // Create a server, reuse the addr.
  ACE_ATM_Acceptor  peer_acceptor;
  ACE_ATM_Params    params;

  // Not sure why but reuse_addr set to true/1 causes problems for
  // FORE/XTI/ATM - this is now handled in ACE_ATM_Acceptor::open()

  ACE_HANDLE ret = peer_acceptor.open (addr, 5, params);
  if ( ret == ACE_INVALID_HANDLE )
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open"),
                      -1);

  ACE_ATM_Stream new_stream;
  ACE_ATM_Addr local_addr;

  local_addr.set_selector( selector );
  peer_acceptor.get_local_addr( local_addr );

  ACE_DEBUG ((LM_DEBUG,
              "starting server at address %s\n",
              local_addr.addr_to_string ()));

  // Performs the iterative server activities
  char buf[BUFSIZ];
  ACE_High_Res_Timer timer;
  int total;
  ACE_Time_Value tv;
  double real_time;
  double actual_rate;

  for (;;)
  {
    // Create a new ACE_ATM_Stream endpoint (note automatic restart
    // if errno == EINTR).
    ACE_OS::printf( "ATM_Server: expecting clients\n" );

    if (peer_acceptor.accept (new_stream,
                              &addr,
                              &timeout) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "%p\n",
                  "accept"));
      continue;
    }

    ACE_OS::printf( "ATM_Server: got a connection\n" );

    ACE_UINT16 vpi, vci;
    vpi = vci = 0;
    // This has problem on PMP connections on NT
    //new_stream.get_vpi_vci(vpi, vci);
    ACE_DEBUG ((LM_DEBUG,
                "connected to VPI %d VCI %d\n",
                vpi, vci));

    ACE_OS::printf( "ATM_Server: connection accepted\n" );

    ACE_DEBUG ((LM_DEBUG,
                "client %s connected\n",
                addr.addr_to_string ()));
    ACE_DEBUG ((LM_DEBUG,
                "client %s connected to host\n",
                new_stream.get_peer_name ()));

    // Read data from client (terminate on error).

    int recvd = 0;

    for ( ;; )
    {
      total = 0;
      timer.start_incr();

      for (int r_bytes;
           (r_bytes = new_stream.recv (buf, sizeof buf, 0)) > 0; )
        {
//        ACE_OS::printf( "ATM_Server: received %dB\n", r_bytes );

//        if (ACE_OS::write (ACE_STDOUT,
//                           buf,
//                           r_bytes) != r_bytes)
//          ACE_ERROR ((LM_ERROR,
//                      "%p\n",
//                      "ACE::send_n"));
          total += r_bytes;

          if ( total > 10000000 )
            break;
        }

      timer.stop_incr();
      timer.elapsed_time_incr( tv );
      real_time = tv.sec() * ACE_ONE_SECOND_IN_USECS + tv.usec();
      recvd += total;
      actual_rate = ( double )recvd * ( double )8 / real_time;

      ACE_DEBUG ((LM_DEBUG,
                 ACE_TEXT ("(%t) bytes = %d, usec = %f, rate = %0.00f Mbps\n"),
                 recvd,
                 real_time,
                 actual_rate < 0 ? 0 : actual_rate ));
    }

    // Close new endpoint (listening endpoint stays open).
    if (new_stream.close () == -1)
      ACE_ERROR ((LM_ERROR,
                  "%p\n",
                  "close"));
  }

  /* NOTREACHED */
  return 0;
}
#else
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "your platform isn't configured to support ATM\n"),
                    1);
}
#endif /* ACE_HAS_ATM */
