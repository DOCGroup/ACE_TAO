// $Id$


#include "child.h"
// Bench_Server_StreamEndPoint methods.

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
    ACE_DEBUG ((LM_DEBUG,"(%P|%t) Bench_Server_StreamEndPoint::handle_connection_requested:() \n"));
    
    ACE_INET_Addr client_addr (the_spec [0]);
    
    if (this->connector_.connect (this->tcp_stream_,
                                  client_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Connection to server failed: %p\n",
                       "connect"),
                      CORBA::B_FALSE);
    return CORBA::B_TRUE;
}

// --------------------------------------------------------------------------------

int
main (int argc, char **argv)
{
  TAO_AV_Child_Process_B<Bench_Server_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl> bench_child;

  if (bench_child.init (argc,argv) == -1)
    return 1;
  if (bench_child.run () == -1)
    return 2;
  
  return 0;
}
