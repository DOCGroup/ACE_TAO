#include "SSL_Connector.h"

ACE_RCSID (ACE_SSL,
           SSL_Connector,
           "$Id$")


// Shorthand names.
#define SVH SVC_HANDLER
#define PR_CO_1 ACE_PEER_CONNECTOR_1
#define PR_CO_2 ACE_PEER_CONNECTOR_2
#define PR_AD ACE_PEER_CONNECTOR_ADDR


template <class SVH, PR_CO_1>
ACE_SSL_Connector<SVH, PR_CO_2>::ACE_SSL_Connector (ACE_Reactor *r,
                                                    int flags)
  : ACE_Connector<SVH, PR_CO_2> (r, flags)
{
}

template <class SVH, PR_CO_1> int
ACE_SSL_Connector<SVH, PR_CO_2>::handle_input (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_SSL_Connector<SVH, PR_CO_2>::handle_input");

  // The TCP connection has either failed or it was previously
  // completed.  If it was previously completed then complete the SSL
  // connection/handshake.

  // @@ It doesn't look like we'll ever get here since the
  //    ACE_SSL_SOCK_Connector::ssl_connect() method doesn't return
  //    until the SSL connection/handshake is completed.  In case its
  //    implementation ever changes, this method may be needed.

  int result = 0;
  ACE_Connector<SVH, PR_CO_2>::AST * ast = 0;

  if (this->handler_map_.find (handle, ast) == 0)
    {
      ACE_SSL_SOCK_Stream & new_stream = ast->svc_handler ()->peer ();

      PR_AD raddr;

      // Check to see if the TCP connection was previously completed.
      if (new_stream.peer ().get_remote_addr (raddr) == -1)
        {
          ACE_Timer_Queue * tq = this->reactor ()->timer_queue ();

          ACE_Time_Value timer_buf (0);
          ACE_Time_Value * tv = tq->calculate_timeout (0, &timer_buf);

          if (this->connector ().complete (new_stream, 0, tv) == 0)
            return this->ACE_Connector<SVH, PR_CO_2>::handle_output (handle);
        }

      if (errno != EWOULDBLOCK)
        // Connection failed.  Cleanup resources.
        return this->ACE_Connector<SVH, PR_CO_2>::handle_input (handle);
    }

  return result;
}

template <class SVH, PR_CO_1> int
ACE_SSL_Connector<SVH, PR_CO_2>::handle_output (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_SSL_Connector<SVH, PR_CO_2>::handle_output");

  // The TCP connection has completed.  Now complete the SSL
  // connection/handshake.

  ACE_Connector<SVH, PR_CO_2>::AST * ast = 0;
  if (this->handler_map_.find (handle, ast) == 0)
    {
      ACE_SSL_SOCK_Stream & new_stream = ast->svc_handler ()->peer ();

      ACE_Timer_Queue * tq = this->reactor ()->timer_queue ();

      ACE_Time_Value timer_buf (0);
      ACE_Time_Value * tv = tq->calculate_timeout (0, &timer_buf);

      // Delegate completion to the underlying connector.
      if (this->connector ().complete (new_stream, 0, tv) == 0)
        return this->ACE_Connector<SVH, PR_CO_2>::handle_output (handle);
      else if (errno != EWOULDBLOCK)
        // Connection failed.  Cleanup resources.
        return this->ACE_Connector<SVH, PR_CO_2>::handle_input (handle);
    }

  return 0;
}

// ------------------------------------------------------------

template <class SVH, PR_CO_1>
ACE_SSL_Strategy_Connector<SVH, PR_CO_2>::ACE_SSL_Strategy_Connector
  (ACE_Reactor *reactor,
   ACE_Creation_Strategy<SVC_HANDLER> *cre_s,
   ACE_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2> *conn_s,
   ACE_Concurrency_Strategy<SVC_HANDLER> *con_s,
   int flags)
  : ACE_Strategy_Connector<SVH, PR_CO_2> (reactor,
                                          cre_s,
                                          conn_s,
                                          con_s,
                                          flags)
{
  ACE_TRACE ("ACE_SSL_Strategy_Connector<SVH, PR_CO_2>::ACE_SSL_Strategy_Connector");
}

template <class SVH, PR_CO_1> int
ACE_SSL_Strategy_Connector<SVH, PR_CO_2>::handle_input (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_SSL_Strategy_Connector<SVH, PR_CO_2>::handle_input");


  // @@ Mucho code duplication here, but I couldn't get around the
  //    multiple inheritance issues introduced when inheriting from
  //    both ACE_Strategy_Connector and ACE_SSL_Connector.
  //      -Ossama


  // The TCP connection has either failed or it was previously
  // completed.  If it was previously completed then complete the SSL
  // connection/handshake.

  // @@ It doesn't look like we'll ever get here since the
  //    ACE_SSL_SOCK_Connector::ssl_connect() method doesn't return
  //    until the SSL connection/handshake is completed.  In case its
  //    implementation ever changes, this method may be needed.

  int result = 0;
  ACE_Connector<SVH, PR_CO_2>::AST * ast = 0;

  if (this->handler_map_.find (handle, ast) == 0)
    {
      ACE_SSL_SOCK_Stream & new_stream = ast->svc_handler ()->peer ();

      PR_AD raddr;

      // Check to see if the TCP connection was previously completed.
      if (new_stream.peer ().get_remote_addr (raddr) == -1)
        {
          ACE_Timer_Queue * tq = this->reactor ()->timer_queue ();

          ACE_Time_Value timer_buf (0);
          ACE_Time_Value * tv = tq->calculate_timeout (0, &timer_buf);

          if (this->connector ().complete (new_stream, 0, tv) == 0)
            return this->ACE_Connector<SVH, PR_CO_2>::handle_output (handle);
        }

      if (errno != EWOULDBLOCK)
        // Connection failed.  Cleanup resources.
        return this->ACE_Connector<SVH, PR_CO_2>::handle_input (handle);
    }

  return result;
}

template <class SVH, PR_CO_1> int
ACE_SSL_Strategy_Connector<SVH, PR_CO_2>::handle_output (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_SSL_Strategy_Connector<SVH, PR_CO_2>::handle_output");


  // @@ Mucho code duplication here, but I couldn't get around the
  //    multiple inheritance issues introduced when inheriting from
  //    both ACE_Strategy_Connector and ACE_SSL_Connector.
  //      -Ossama


  // The TCP connection has completed.  Now complete the SSL
  // connection/handshake.

  ACE_Connector<SVH, PR_CO_2>::AST * ast = 0;
  if (this->handler_map_.find (handle, ast) == 0)
    {
      ACE_SSL_SOCK_Stream & new_stream = ast->svc_handler ()->peer ();

      ACE_Timer_Queue * tq = this->reactor ()->timer_queue ();

      ACE_Time_Value timer_buf (0);
      ACE_Time_Value * tv = tq->calculate_timeout (0, &timer_buf);

      // Delegate completion to the underlying connector.
      if (this->connector ().complete (new_stream, 0, tv) == 0)
        return this->ACE_Connector<SVH, PR_CO_2>::handle_output (handle);
      else if (errno != EWOULDBLOCK)
        // Connection failed.  Cleanup resources.
        return this->ACE_Connector<SVH, PR_CO_2>::handle_input (handle);
    }

  return 0;
}
