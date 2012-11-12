// $Id$

#include "ace/OS_main.h"
#include "ace/ATM_Connector.h"
#include "ace/ATM_Addr.h"
#include "ace/High_Res_Timer.h"
#include "ace/Log_Msg.h"



#if defined (ACE_HAS_ATM)

#define MAX_LEAVES  32

/* ACE_ATM Client */

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if ( argc < 2 )
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Usage: %s <rate> <PDU> <session> <host> <selector> [ host sel ] ...\n"
                       "\tUse 0 for default values\n",
                       argv[0]),
                      1);

  int rate = ACE_OS::atoi( argv[ 1 ]);
  rate = ( rate != 0 ) ? rate : 170000;
  int pdu_size = ACE_OS::atoi( argv[ 2 ]) * 1024;
  pdu_size = ( pdu_size != 0 ) ? pdu_size : 8192;
  int session = ACE_OS::atoi( argv[ 3 ]);
  session = ( session != 0 ) ? session : 100;

  ACE_OS::printf( "ATM_Client: rate: %d c/s, PDU: %dB, session: %d pkts\n",
    rate, pdu_size, session );

  // Record all hosts/selectors
  ACE_ATM_Addr hosts[ MAX_LEAVES ];
  int num_leaves = argc / 2 - 2;

  ACE_OS::printf( "ATM_Client: Connecting to ...\n" );
  for ( int i = 0; i < num_leaves; i++ )
  {
    hosts[ i ].set( argv[ i*2 + 4 ],
                    ( argv[ i*2 + 5 ] != 0 )
                      ? ACE_OS::atoi( argv[ i*2 + 5 ]) : ACE_ATM_Addr::DEFAULT_SELECTOR );
    ACE_OS::printf( "ATM_Client: leaf: %s (%s), sel: %d\n",
                    argv[ i*2 + 4 ],
                    hosts[ i ].addr_to_string(),
                    hosts[ i ].get_selector());
  }

  // The timeout really gets ignored since FORE's drivers don't work when
  //  ioctl or fcntl calls are made on the transport id/file descriptor
  int timeout = ACE_DEFAULT_TIMEOUT;
  char buf[BUFSIZ];
  ACE_ATM_Stream atm_stream;

  char hostname[ MAXNAMELEN ];
  ACE_OS::hostname( hostname, MAXNAMELEN );
  ACE_ATM_Addr local_addr( hostname, hosts[ 0 ].get_selector());

  ACE_OS::printf( "ATM_Client: local host: %s(%s)\n",
    hostname, local_addr.addr_to_string());

  // In order to construct connections options the file handle is
  // needed.  Therefore, we need to open the ATM_Stream before we
  // construct the options.
  ACE_OS::printf( "ATM_Client: to open a stream\n" );
  if (atm_stream.open () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open failed"),
                      1);

  ACE_DEBUG ((LM_DEBUG,
              "ATM_Client: starting non-blocking connection\n"));

  // Initiate timed, non-blocking connection with server.
  ACE_ATM_Connector con;

  // Construct QoS options - currently FORE only supports bandwidth
  ACE_OS::printf( "ATM_Client: specify cell rate at %d c/s\n", rate );
  ACE_ATM_QoS qos;
  qos.set_rate(atm_stream.get_handle (),
               rate,
               ACE_ATM_QoS::OPT_FLAGS_CPID);

  if ( num_leaves == 1 )
  {
    // Point-to-point connection
    // Not sure why but reuse_addr set to true/1 causes problems for
    // FORE/XTI/ATM - this is now handled in ACE_ATM_Connector::connect()
    ACE_OS::printf( "ATM_Client: to open a connection\n" );
    ACE_ATM_Params params = ACE_ATM_Params();
    if (con.connect (atm_stream,
                     hosts[ 0 ],
                     params,
                     qos,
                     (ACE_Time_Value *) &ACE_Time_Value::zero,
                     local_addr,
                     0,
                     0 ) == -1)
    {
      if (errno != EWOULDBLOCK)
        ACE_ERROR_RETURN ((LM_ERROR,
                            "%p\n",
                            "ATM_Client: connection failed"),
                            1);

      ACE_DEBUG ((LM_DEBUG,
                    "ATM_Client: starting timed connection\n"));

      // Check if non-blocking connection is in progress, and wait up
      // to timeout seconds for it to complete.
      ACE_Time_Value tv (timeout);

      ACE_OS::printf( "ATM_Client: connection completed\n" );
      if (con.complete (atm_stream,
                        &hosts[ 0 ],
                        &tv) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "ATM_Client: connection failed"),
                           1);
      else
        ACE_DEBUG ((LM_DEBUG,
                    "ATM_Client: connected to %s\n",
                    hosts[ 0 ].addr_to_string()));
    }
  } else {
    // Point-to-multipoint connection
    for ( int i = 0; i < num_leaves; i++ ) {
      con.add_leaf( atm_stream,
                    hosts[ i ],
                    i,
                    0 );

    }
  } /* if num_leaves == 1 */

  ACE_UINT16 vpi, vci;
  atm_stream.get_vpi_vci(vpi, vci);
  ACE_DEBUG ((LM_DEBUG,
              "ATM_Client: connected to VPI %d VCI %d\n",
              vpi, vci));

  // Send data to server (correctly handles "incomplete writes").

  int s_bytes;
  int total;
  int xmit = 0;
  ACE_High_Res_Timer timer;
  ACE_Time_Value elapsed;
  double real_time;
  double actual_rate;

  for ( ;; ) {
    total = 0;

    timer.start_incr();

    for ( ;; ) {
      s_bytes = atm_stream.send_n( buf, BUFSIZ, 0 );
      if ( s_bytes == -1 )
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "send_n"),
                           1);

      total += s_bytes;

      if ( total >= session * pdu_size )
        break;
    }

    timer.stop_incr();
    timer.elapsed_time_incr( elapsed );
    real_time = elapsed.sec() * ACE_ONE_SECOND_IN_USECS + elapsed.usec();
    xmit += total;
    actual_rate = ( double )xmit * ( double )8 / real_time;

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) bytes = %d, usec = %f, rate = %0.00f Mbps\n"),
                xmit,
                real_time,
                actual_rate < 0 ? 0 : actual_rate ));
  }

  // Explicitly close the connection.
  ACE_OS::printf( "ATM_Client: close connection\n" );
  if (atm_stream.close () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "close"),
                       -1);
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
