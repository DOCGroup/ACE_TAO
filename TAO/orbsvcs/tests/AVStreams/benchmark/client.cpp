// $Id$

#include "client.h"


ACE_RCSID(benchmark, client, "$Id$")

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
  //  return 1;
  the_spec.length (0);
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) handle_preconnect called\n"));
  return 0;
}

// called by the A/V framework after calling connect. Passes the
// server streamendpoints' flowspec which we use to connect our
// datagram socket.
CORBA::Boolean
Client_StreamEndPoint::handle_postconnect (AVStreams::flowSpec& server_spec)
{
  //  return 1;
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

ttcp_Acceptor::ttcp_Acceptor (ttcp_Client_StreamEndPoint *endpoint)
  :endpoint_ (endpoint)
{
}

int
ttcp_Acceptor::make_svc_handler (ttcp_Client_StreamEndPoint *&sh)
{
  sh = this->endpoint_;
  return 0;
}

//------------------------------------------------------------

ttcp_Client_StreamEndPoint::ttcp_Client_StreamEndPoint (Client *client)
  :acceptor_ (this),
   client_ (client)
{
}

CORBA::Boolean
ttcp_Client_StreamEndPoint::handle_preconnect (AVStreams::flowSpec &the_spec)
{
  // listen for the tcp socket.
  
  ACE_INET_Addr tcp_addr;

  //  tcp_addr.set (TCP_PORT,"mambo-atm.cs.wustl.edu");
  tcp_addr.set (TCP_PORT);

  if (this->acceptor_.open (tcp_addr,
                            TAO_ORB_Core_instance ()->reactor ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,"%p\n","open"),-1);
  ACE_INET_Addr local_addr;

  if (this->acceptor_.acceptor ().get_local_addr (local_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t)acceptor get local addr failed %p"),-1);

  char client_address_string [BUFSIZ];
  ::sprintf (client_address_string,
             "%s:%d",
             local_addr.get_host_name (),
             //             "mambo-atm.cs.wustl.edu",
             local_addr.get_port_number ());
  the_spec.length (1);
  the_spec [0] = CORBA::string_dup (client_address_string);
  
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) client flow spec is %s\n",
              client_address_string));

  return 1;
}

CORBA::Boolean
ttcp_Client_StreamEndPoint::handle_postconnect (AVStreams::flowSpec& server_spec)
{
  ACE_DEBUG ((LM_DEBUG,"ttcp_Client_StreamEndPoint::handle_postconnect \n"));
  this->client_->set_stream (this->peer ());
  return 1;
}

int
ttcp_Client_StreamEndPoint::open (void *)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) ttcp_Client_StreamEndPoint::open () called\n"));
  return 0;
}

Client::Client (int argc, char **argv,int task_id)
  : reactive_strategy_ (&orb_manager_),
    ttcp_reactive_strategy_ (&orb_manager_,this),
    client_mmdevice_ (0),
    argc_ (argc),
    argv_ (argv),
    task_id_ (task_id)
{
}

void
Client::set_stream (ACE_SOCK_Stream & control)
{
  this->stream_ = control;
}

Globals::Globals (void)
  :ready_cond_ (ready_mtx_),
   hostname_ (""),
   port_ (0)
{
}

int
Globals::parse_args (int argc,
                     char **argv)
{
  ACE_Get_Opt opts (argc,argv,"b:n:h:p:ts");

  int c;
  this->strategy_ = TTCP_REACTIVE;
  this->block_size_ = 1;
  this->number_ = 10;
  while ((c = opts ()) != -1)
    switch (c)
      {
      case 'b':
        this->block_size_ = ACE_OS::atoi (opts.optarg);
        break;
      case 'n':
        this->number_ = ACE_OS::atoi (opts.optarg);
        break;
      case 'h':
        this->hostname_ = ACE_OS::strdup (opts.optarg);
        break;
      case 'p':
        this->port_ = ACE_OS::atoi (opts.optarg);
        break;
      case 't':
        this->thread_count_ = ACE_OS::atoi (opts.optarg);
        break;
      case 's':
        // use ttcp strategy.        
        this->strategy_ = TTCP_REACTIVE;
        break;
      case '?':
        ACE_DEBUG ((LM_DEBUG,"Usage %s [-b block_size] [-n number_of times]"
                    "[-h hostname] [-p port_number] [-t]",
                    argv[0]));
        break;
      }
  return 0;
}

int
Client::svc (void)
{
  // Now start pumping data.
  ACE_High_Res_Timer timer;
  ACE_Time_Value tv1,tv2;
  
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Thread created\n"));

  if (GLOBALS::instance ()->parse_args (this->argc_,this->argv_) == -1)
    return -1;
  TAO_TRY
    {
      this->orb_manager_.init (this->argc_,
                               this->argv_,
                               TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (this->task_id_ == 0)
        {
          ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ready_mon, GLOBALS::instance ()->ready_mtx_, 1));
          if (GLOBALS::instance ()->parse_args (this->argc_,
                                                this->argv_) == -1)
            return -1;

          ACE_NEW_RETURN (GLOBALS::instance ()->barrier_,
                          ACE_Barrier (GLOBALS::instance ()->thread_count_),
                          -1);
          GLOBALS::instance ()->ready_ = 1;
          GLOBALS::instance ()->ready_cond_.broadcast ();
        }
      else
        {
          ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ready_mon, GLOBALS::instance ()->ready_mtx_, 1));
          GLOBALS::instance ()->ready_cond_.wait ();
        }

      switch (GLOBALS::instance ()->strategy_)
        {
        case Globals::TTCP_REACTIVE:
          ACE_NEW_RETURN  (this->client_mmdevice_,
                           TAO_MMDevice (&ttcp_reactive_strategy_),
                           -1);
          break;
        case Globals::DGRAM_REACTIVE:
          ACE_NEW_RETURN (this->client_mmdevice_,
                          TAO_MMDevice (&reactive_strategy_),
                          -1);
          break;
        }

      // activate the client MMDevice with the ORB
      this->orb_manager_.activate (this->client_mmdevice_,
                                   TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (this->bind_to_server () == -1)
        ACE_ERROR_RETURN ((LM_ERROR, 
                           "(%P|%t) Error binding to the naming service\n"),
                          -1);
      
      // wait for the other clients to finish binding
      GLOBALS::instance ()->barrier_->wait ();
      
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) All threads finished, starting tests.\n"));

      ACE_Time_Value tv (0);
      this->orb_manager_.run (TAO_TRY_ENV,&tv);
      TAO_CHECK_ENV;
      AVStreams::streamQoS_var the_qos (new AVStreams::streamQoS);
      AVStreams::flowSpec_var the_flows (new AVStreams::flowSpec);
      // Bind the client and server mmdevices.

      timer.start ();
      this->streamctrl_.bind_devs
        (this->client_mmdevice_->_this (TAO_TRY_ENV),
         this->server_mmdevice_.in (),
         the_qos.inout (),
         the_flows.in (),
         TAO_TRY_ENV);
      TAO_CHECK_ENV;

      timer.stop ();
      timer.elapsed_time (tv1);
      long time_taken = tv1.sec () + tv1.usec () /1000000;
      tv1.dump ();
      ACE_DEBUG ((LM_DEBUG,"(%P|%t)time taken for stream setup is %ld \n",
                  time_taken ));
      
#if !defined (ACE_LACKS_SOCKET_BUFSIZ)
      int sndbufsize = ACE_DEFAULT_MAX_SOCKET_BUFSIZ;
      int rcvbufsize = ACE_DEFAULT_MAX_SOCKET_BUFSIZ;
      
      int result;
      result = this->stream_.set_option (SOL_SOCKET,
					 SO_SNDBUF,
					 (void *) &sndbufsize,
					 sizeof (sndbufsize));
      if ((result == -1) && (errno != ENOTSUP))
        ACE_ERROR_RETURN ((LM_ERROR,"set_option failed for sndbufsize:%p\n",""),-1);
      result = this->stream_.set_option (SOL_SOCKET,
                                         SO_RCVBUF,
                                         (void *) &rcvbufsize,
                                         sizeof (rcvbufsize));
      if ((result == -1)&& (errno != ENOTSUP))
        ACE_ERROR_RETURN ((LM_ERROR,"set_option failed for rcvbufsize:%p\n",""),-1);
#endif /* ACE_LACKS_SOCKET_BUFSIZ */
#if defined (TCP_NODELAY)
      int one = 1;
      result = this->stream_.set_option (SOL_SOCKET,
					 TCP_NODELAY,
					 (char *)& one,
					 sizeof (one));
      if (result == -1)
        ACE_ERROR_RETURN ((LM_ERROR,"set_option failed TCP_NODELAY:%p\n",""),-1);
#endif

      char *buffer;
      long buffer_siz = GLOBALS::instance ()->block_size_*1024;
      
      ACE_NEW_RETURN (buffer,
                      char [buffer_siz],
                      -1);
      long number = 64 *1024/(GLOBALS::instance ()->block_size_);
      timer.start ();
      for (int i=0;i<number;i++)
	{
	  cerr << i << " ";
	  if (i == 136)
	    {
	      	  this->stream_.send_n (buffer,buffer_siz);
	    }
	  else
	    this->stream_.send_n (buffer,buffer_siz);
	}
      timer.stop ();
      timer.elapsed_time (tv2);
      double total_time = tv2.sec ()+tv2.usec ()/1000000.0;
      double total_data = 64*1024*1024*8.0;
      ACE_DEBUG ((LM_DEBUG,"Total data = %f , Total time = %f \n",
                  total_data,total_time));
      ACE_DEBUG ((LM_DEBUG,"(%P|%t) Thruput for block size is:%d\t%f Mb/s \n",
                  buffer_siz,total_data/(total_time*1024.0*1024.0)));
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
      /*
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
      */

      // Initialize the naming services
      if (my_name_client_.init (this->orb_manager_.orb ()) != 0)
	ACE_ERROR_RETURN ((LM_ERROR,
			   " (%P|%t) Unable to initialize "
			   "the TAO_Naming_Client. \n"),
			  -1);
      
      CosNaming::Name server_mmdevice_name (1);

      server_mmdevice_name.length (1);
      server_mmdevice_name [0].id = CORBA::string_dup ("Bench_Server_MMDevice");
      CORBA::Object_var server_mmdevice_obj =
        my_name_client_->resolve (server_mmdevice_name,
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
        (this->client_mmdevice_->_this (TAO_TRY_ENV),
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


// -----------------------------------------------------------
// Video_Endpoint_Reactive_Strategy_A methods

ttcp_Endpoint_Reactive_Strategy_A::ttcp_Endpoint_Reactive_Strategy_A (TAO_ORB_Manager *orb_manager,
                                                                        Client *client)
  : TAO_AV_Endpoint_Reactive_Strategy_A<ttcp_Client_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>  (orb_manager),
    client_ (client)
{
}

int
ttcp_Endpoint_Reactive_Strategy_A::make_stream_endpoint (ttcp_Client_StreamEndPoint *&endpoint)
{
  ACE_NEW_RETURN (endpoint,
                  ttcp_Client_StreamEndPoint (this->client_),
                  -1);

  return 0;
}

// ----------------------------------------------------------------------

int
main (int argc, char **argv)
{  

  GLOBALS::instance ()->thread_count_ = 1;
  // Preliminary argument processing.
  for (int i=0;i< argc;i++)
    {
      if (ACE_OS::strcmp (argv[i],"-t") == 0
          && (i - 1 < argc))
        GLOBALS::instance ()->thread_count_ =
          ACE_OS::atoi (argv[i+1]);
    }

  for (i = 0; i < GLOBALS::instance ()->thread_count_; i++)
    {
      Client *client;
      ACE_NEW_RETURN (client,
                      Client (argc,
                              argv,
                              i),
                      -1);

      if (client->activate (THR_BOUND) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, 
                           "(%P|%t) Error in activate: %p",
                           "activate"),
                          -1);
    }

  ACE_Thread_Manager::instance ()->wait ();
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class TAO_AV_Endpoint_Reactive_Strategy<ttcp_Client_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>;
template class TAO_AV_Endpoint_Reactive_Strategy<Client_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>;
template class TAO_AV_Endpoint_Reactive_Strategy_A<ttcp_Client_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>;
template class TAO_AV_Endpoint_Reactive_Strategy_A<Client_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>;
template class ACE_Acceptor <ttcp_Client_StreamEndPoint,ACE_SOCK_ACCEPTOR>;
template class ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
template class ACE_Task<ACE_SYNCH>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy<ttcp_Client_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy<Client_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy_A<ttcp_Client_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl> 
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy_A<Client_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>
#pragma instantiate ACE_Acceptor <ttcp_Client_StreamEndPoint,ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH>
#pragma instantiate ACE_Task<ACE_SYNCH>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
