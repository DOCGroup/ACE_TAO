
// $Id$

#include "client.h"

Client_StreamEndPoint::Client_StreamEndPoint (void)
{
}
 

int
Client_StreamEndPoint::handle_open (void)
{
  return 0;
}

int
Client_StreamEndPoint::handle_close (void)
{
  return 0;
}

// called by the framework before calling connect. Here we create our
// flow spec which is nothing but hostname::port_number of the
// datagram.
CORBA::Boolean
Client_StreamEndPoint::handle_preconnect (AVStreams::flowSpec &the_spec)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) handle_preconnect called\n"));
  return 0;
}

// called by the A/V framework after calling connect. Passes the
// server streamendpoints' flowspec which we use to connect our
// datagram socket.
CORBA::Boolean
Client_StreamEndPoint::handle_postconnect (AVStreams::flowSpec& server_spec)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) handle_postconnect called \n"));
  return 0;
}

int
Client_StreamEndPoint::handle_start (const AVStreams::flowSpec &the_spec,
                                           CORBA::Environment &env) 

{
  return -1;
}

int
Client_StreamEndPoint::handle_stop (const AVStreams::flowSpec &the_spec,
                                          CORBA::Environment &env) 

{
  return -1;
}

int
Client_StreamEndPoint::handle_destroy (const AVStreams::flowSpec &the_spec,
                                             CORBA::Environment &env) 

{
  return -1;
}

// ----------------------------------------------------------------------
Client::Client (int argc, char **argv, ACE_Barrier *barrier)
  : reactive_strategy_ (&orb_manager_),
    client_mmdevice_ (&reactive_strategy_),
    argc_ (argc),
    argv_ (argv),
    barrier_ (barrier)
{
}

int
Client::svc (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Thread created\n"));
  TAO_TRY
    {
      this->orb_manager_.init (this->argc_,
                               this->argv_,
                               TAO_TRY_ENV);
      TAO_CHECK_ENV;
      // activate the client MMDevice with the ORB
      this->orb_manager_.activate (&this->client_mmdevice_,
                                   TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (this->bind_to_server () == -1)
        ACE_ERROR_RETURN ((LM_ERROR, 
                           "(%P|%t) Error binding to the naming service\n"),
                          -1);
      
      // wait for the other clients to finish binding
      this->barrier_->wait ();
      
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) All threads finished, starting tests.\n"));
      
      AVStreams::streamQoS_var the_qos (new AVStreams::streamQoS);
      AVStreams::flowSpec_var the_flows (new AVStreams::flowSpec);
      // Bind the client and server mmdevices.
      
      this->streamctrl_.bind_devs
        (this->client_mmdevice_._this (TAO_TRY_ENV),
         this->server_mmdevice_.in (),
         the_qos.inout (),
         the_flows.in (),
         TAO_TRY_ENV);
      
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("streamctrl.bind_devs:");
      return -1;
    }
  TAO_ENDTRY;
  
  return 0;
}

int
Client::bind_to_server (void)
{
  TAO_TRY
    {
      CORBA::Object_var naming_obj =
        this->orb_manager_.orb ()->resolve_initial_references ("NameService");
      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to resolve the Name Service.\n"),
                          -1);
      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in (),
                                           TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CosNaming::Name server_mmdevice_name (1);

      server_mmdevice_name.length (1);
      server_mmdevice_name [0].id = CORBA::string_dup ("Bench_Server_MMDevice");
      CORBA::Object_var server_mmdevice_obj =
        naming_context->resolve (server_mmdevice_name,
                                 TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->server_mmdevice_ =
        AVStreams::MMDevice::_narrow (server_mmdevice_obj.in (),
                                      TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (CORBA::is_nil (this->server_mmdevice_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " could not resolve Server_Mmdevice in Naming service <%s>\n"),
                          -1);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Command_Handler::resolve_reference");
      return -1;
    }
  TAO_ENDTRY;

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) MMDevice successfully resolved.\n"));
  return 0;
}

int
Client::establish_stream (void)
{

  AVStreams::streamQoS_var the_qos (new AVStreams::streamQoS);
  AVStreams::flowSpec_var the_flows (new AVStreams::flowSpec);
  // Bind the client and server mmdevices.

  TAO_TRY
    {
      this->streamctrl_.bind_devs
        (this->client_mmdevice_._this (TAO_TRY_ENV),
         this->server_mmdevice_.in (),
         the_qos.inout (),
         the_flows.in (),
         TAO_TRY_ENV);
      
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("streamctrl.bind_devs:");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

// ----------------------------------------------------------------------

int
main (int argc, char **argv)
{
  ACE_Get_Opt opts (argc, argv, "T:");
  int thread_count = 0;
  
  int c;
  while ((c = opts ()) != -1)
    switch (c)
      {
      case 'T':
      thread_count = (u_int) ACE_OS::atoi (opts.optarg);
      continue;
      default:
//         ACE_DEBUG ((LM_DEBUG, 
//                     "Usage: %s -t number_of_threads\n",
//                     argv [0]));
        break;
      }

  ACE_Barrier *barrier;
  ACE_NEW_RETURN (barrier,
                  ACE_Barrier (thread_count + 1),
                  -1);
  for (int i = 0; i < thread_count; i++)
    {
      Client *client;
      ACE_NEW_RETURN (client,
                      Client (argc,
                              argv,
                              barrier),
                      -1);

      if (client->activate (THR_BOUND) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, 
                           "(%P|%t) Error in activate: %p",
                           "activate"),
                          -1);
    }

  // wait for all the threads to finish starting up
  barrier->wait ();
  ACE_DEBUG ((LM_DEBUG,
              "(%t) All threads started, waiting for test completion\n"));
  
  ACE_Thread_Manager::instance ()->wait ();

}
