// $Id$

#include "child.h"

ACE_RCSID(benchmark, child, "$Id$")

// Bench_Server_StreamEndPoint methods.

Bench_Server_StreamEndPoint::Bench_Server_StreamEndPoint (void)
{
}

int
Bench_Server_StreamEndPoint::handle_open (void) 
{
  return 0;
}

int
Bench_Server_StreamEndPoint::handle_close (void) 
{
  // called when streamendpoint is being destructed
  return 0;
}

int
Bench_Server_StreamEndPoint::handle_stop (const AVStreams::flowSpec &the_spec,
                                          CORBA::Environment &env) 
{
  return 0;
}
  
int
Bench_Server_StreamEndPoint::handle_start (const AVStreams::flowSpec &the_spec,  
                                           CORBA::Environment &env) 
{
 return 0;
}
  
int
Bench_Server_StreamEndPoint::handle_destroy (const AVStreams::flowSpec &the_spec,  
                                             CORBA::Environment &env) 
{
  return 0;
}

CORBA::Boolean 
Bench_Server_StreamEndPoint::handle_connection_requested (AVStreams::flowSpec &the_spec,  
                                                          CORBA::Environment &env) 
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) Bench_Server_StreamEndPoint::handle_connection_requested:() length =%d\n",
              the_spec.length ()));

  ACE_INET_Addr client_addr (the_spec [0]);
  u_short local_port = 0;
  ACE_INET_Addr local_addr (local_port);
  
  if (this->connector_.connect (this->tcp_stream_,
                                client_addr,
                                0,
                                local_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Connection to server failed: %p\n",
                       "connect"),
                      0);
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) tcp connect succeeded %d\n",this->tcp_stream_.get_handle ()));
  
  int sndbufsize = ACE_DEFAULT_MAX_SOCKET_BUFSIZ;
  int rcvbufsize = ACE_DEFAULT_MAX_SOCKET_BUFSIZ;
  
  if (this->tcp_stream_.set_option (SOL_SOCKET,
                                SO_SNDBUF,
                                (void *) &sndbufsize,
                                sizeof (sndbufsize)) == -1
      && errno != ENOTSUP)
    return -1;
  else if (this->tcp_stream_.set_option (SOL_SOCKET,
                                         SO_RCVBUF,
                                     (void *) &rcvbufsize,
                                     sizeof (rcvbufsize)) == -1
           && errno != ENOTSUP)
    return -1;
  
  int one = 1;
  if (this->tcp_stream_.set_option (SOL_SOCKET,
                                    TCP_NODELAY,
                                (char *)& one,
                                sizeof (one)) == -1 )
    return -1;

  ACE_NEW_RETURN (this->stream_handler_,
                  ttcp_Stream_Handler (this->tcp_stream_.get_handle ()),
                  0);
  int result;

  result = TAO_ORB_Core_instance ()->reactor ()->register_handler (this->stream_handler_,
                                                                   ACE_Event_Handler::READ_MASK);
  if (result < 0)
    return 0;

  ACE_DEBUG ((LM_DEBUG,"(%P|%t) register handler for tcp_Stream success\n"));
  return 1;
}

ttcp_Stream_Handler::ttcp_Stream_Handler (ACE_HANDLE control_fd)
  : control_handle_ (control_fd)
{
}

// Called by the reactor to extract the handle associated with this handler.
ACE_HANDLE
ttcp_Stream_Handler::get_handle (void) const
{
  return this->control_handle_ ;
}

// Called by the Reactor when data is ready to be read from the
// video control handle, which indicates a control message from the client.
int
ttcp_Stream_Handler::handle_input (ACE_HANDLE handle)
{
  //  ACE_DEBUG ((LM_DEBUG,"(%P|%t) Bench_Server_StreamEndPoint::handle_input ()\n"));

  char buf[BUFSIZ];
  int result =ACE_OS::read (this->control_handle_,buf,BUFSIZ);
  if (result < 0 )
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Bench_Server_StreamEndPoint::handle_input ()\n"),
                      -1);

  if (result == 0)
    return 0;
  return 0;
}


