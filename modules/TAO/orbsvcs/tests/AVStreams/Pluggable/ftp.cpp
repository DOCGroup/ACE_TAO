// $Id$

#include "ftp.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"

ACE_High_Res_Timer last_frame_sent_time;
// The time taken for sending a frmae and preparing for the next frame

ACE_Time_Value inter_frame_time;
// The time that should lapse between two consecutive frames sent.

FTP_Client_Callback::FTP_Client_Callback (void)
{
}

FTP_Client_StreamEndPoint::FTP_Client_StreamEndPoint (void)
{
}

int
FTP_Client_StreamEndPoint::get_callback (const char *,
                                         TAO_AV_Callback *&callback)
{
  // Create and return the clienmt application callback and return to the AVStreams
  // for further upcalls.
  callback = &this->callback_;
  return 0;
}

int
FTP_Client_StreamEndPoint::set_protocol_object (const char *,
                                                TAO_AV_Protocol_Object *object)
{
  // Set the client protocol object corresponding to the transport protocol selected.
  CLIENT::instance ()->set_protocol_object (object);
  return 0;
}

Client::Client (void)
  :client_mmdevice_ (&endpoint_strategy_),
   count_ (0),
   address_ (0),
   peer_addr_str_ (0),
   fp_ (0),
   protocol_ (ACE_OS::strdup ("UDP")),
   frame_rate_ (30)
{
  this->mb.size (BUFSIZ);
}

void
Client::set_protocol_object (TAO_AV_Protocol_Object *object)
{
  // Set the client protocol object corresponding to the transport protocol selected.
  this->protocol_object_ = object;
}

int
Client::parse_args (int argc,
                    ACE_TCHAR *argv[])
{
  // Parse command line arguments
  ACE_Get_Opt opts (argc, argv, ACE_TEXT("f:l:a:p:r:sd"));

  this->use_sfp_ = 0;

  int c;
  while ((c= opts ()) != -1)
    {
      switch (c)
        {
        case 'f':
          this->filename_ = ACE_OS::strdup (ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ()));
          break;
        case 'l':
          this->address_ = ACE_OS::strdup (ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ()));
          break;
        case 'a':
          this->peer_addr_str_ = ACE_OS::strdup (ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ()));
          break;
        case 'p':
          this->protocol_ = ACE_OS::strdup (ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ()));
          break;
        case 'r':
          this->frame_rate_ = ACE_OS::atoi (opts.opt_arg ());
          break;
        case 's':
          this->use_sfp_ = 1;
          break;
        case 'd':
          TAO_debug_level++;
          break;
        default:
          ACE_DEBUG ((LM_DEBUG,"Unknown Option\n"));
          return -1;
        }
    }
  return 0;
}

FILE *
Client::file (void)
{
  return this->fp_;
}

char*
Client::flowname (void)
{
  return this->flowname_;
}

TAO_StreamCtrl*
Client::streamctrl (void)
{
  return &this->streamctrl_;
}


int
Client::frame_rate (void)
{
  return this->frame_rate_;
}


// Method to get the object reference of the server
int
Client::bind_to_server (void)
{
  // Initialize the naming services
  if (my_naming_client_.init (TAO_AV_CORE::instance ()->orb ()) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Unable to initialize "
                       "the TAO_Naming_Client.\n"),
                      -1);

  CosNaming::Name server_mmdevice_name (1);
  server_mmdevice_name.length (1);
  server_mmdevice_name [0].id = CORBA::string_dup ("Server_MMDevice");

  // Resolve the server object reference from the Naming Service
  CORBA::Object_var server_mmdevice_obj =
    my_naming_client_->resolve (server_mmdevice_name);

  this->server_mmdevice_ =
    AVStreams::MMDevice::_narrow (server_mmdevice_obj.in ());

  if (CORBA::is_nil (this->server_mmdevice_.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Could not resolve Server_MMdevice in Naming service <%s>\n"),
                      -1);

  return 0;
}

int
Client::init (int argc,
              ACE_TCHAR *argv[])
{
  this->argc_ = argc;
  this->argv_ = argv;

  CORBA::String_var ior;

  // Initialize the endpoint strategy with the orb and poa.
  this->endpoint_strategy_.init(TAO_AV_CORE::instance ()->orb (),
                                TAO_AV_CORE::instance ()->poa ());

  // Parse the command line arguments
  int result = this->parse_args (argc,
                                 argv);

  if (result != 0)
    return result;

  // Open file to read.
  this->fp_ = ACE_OS::fopen (this->filename_,
                             "r");
  if (this->fp_ == 0)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       "Cannot open input file %s\n",
                       this->filename_),
                      -1);

  result
    = this->bind_to_server ();

  // Resolve the object reference of the server from the Naming Service.
  if (result != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Error binding to the naming service\n"),
                      -1);

  // Create the Flow protocol name
  char flow_protocol_str [BUFSIZ];
  if (this->use_sfp_)
    ACE_OS::strcpy (flow_protocol_str,
                    "sfp:1.0");
  else
    ACE_OS::strcpy (flow_protocol_str,
                    "");

  // Initialize the  QoS
  AVStreams::streamQoS_var the_qos (new AVStreams::streamQoS);

  // Set the address of the ftp client.
  ACE_INET_Addr* addr;
  if (this->address_ != 0)
    ACE_NEW_RETURN (addr,
                    ACE_INET_Addr (this->address_),
                    -1);
  else
    {
      char buf [BUFSIZ];
      ACE_OS::hostname (buf,
                        BUFSIZ);
      ACE_NEW_RETURN (addr,
                      ACE_INET_Addr ("5000",
                                     buf),
                      -1);
    }

  // Initialize the flowname
  ACE_NEW_RETURN (this->flowname_,
                  char [BUFSIZ],
                  0);

  ACE_OS::sprintf (this->flowname_,
                   "Data_%s",
                   this->protocol_);

  // Create the forward flow specification to describe the flow.
  TAO_Forward_FlowSpec_Entry entry (this->flowname_,
                                    "IN",
                                    "USER_DEFINED",
                                    flow_protocol_str,
                                    this->protocol_,
                                    addr);

  ACE_INET_Addr* peer_addr;
  if (this->peer_addr_str_ != 0)
    ACE_NEW_RETURN (peer_addr,
                    ACE_INET_Addr (this->peer_addr_str_),
                    -1);
  else
    {
      char buf [BUFSIZ];
      ACE_OS::hostname (buf,
                        BUFSIZ);
      ACE_NEW_RETURN (peer_addr,
                      ACE_INET_Addr ("5050",
                                     buf),
                      -1);
    }

  entry.set_peer_addr (peer_addr);

  AVStreams::flowSpec flow_spec (1);
  flow_spec.length (1);
  flow_spec [0] = CORBA::string_dup (entry.entry_to_string ());

  AVStreams::MMDevice_var client_mmdevice =
    this->client_mmdevice_._this ();

  // Bind/Connect  the client and server MMDevices.
  CORBA::Boolean bind_result =
    this->streamctrl_.bind_devs (client_mmdevice.in (),
                                 this->server_mmdevice_.in (),
                                 the_qos.inout (),
                                 flow_spec);

  if (bind_result == 0)
    ACE_ERROR_RETURN ((LM_ERROR,"streamctrl::bind_devs failed\n"),-1);

  return 0;
}

// Method to send data at the specified rate
int
Client::pace_data (void)
{

  // Rate at which frames of data need to be sent.
  this->frame_rate_ = CLIENT::instance ()->frame_rate ();

  // Time within which a frame should be sent.
  double frame_time = 1/ (double) this->frame_rate_;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Frame Time ONE = %f\n Frame Rate = %d\n",
                frame_time,
                this->frame_rate_));

  // The time between two consecutive frames.
  inter_frame_time.set (frame_time);

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Inter Frame Time = %d\n",
                inter_frame_time.msec ()));

  try
    {

      // Continue to send data till the file is read to the end.
      while (1)
        {
          // Count the frames sent.
          count_++;

          // Read from the file into a message block.
          size_t n = ACE_OS::fread(this->mb.rd_ptr (),
                                   1,
                                   this->mb.size (),
                                   CLIENT::instance ()->file ());

          if (n == 0)
            {
              if (feof (CLIENT::instance ()->file ()))
                {
                  // At end of file break the loop and end the client.
                  if (TAO_debug_level > 0)
                    ACE_DEBUG ((LM_DEBUG,"Handle_Start:End of file\n"));
                  break;
                }

            }

          this->mb.wr_ptr (n);

          if ( this->count_ > 1)
            {
              // Greater than the first frame.

              // Stop the timer that was started just before the previous frame was sent.
              last_frame_sent_time.stop ();

              // Get the time elapsed after sending the previous frame.
              ACE_Time_Value tv;
              last_frame_sent_time.elapsed_time (tv);

              if (TAO_debug_level > 0)
                ACE_DEBUG ((LM_DEBUG,
                            "Elapsed Time = %d\n",
                            tv.msec ()));

              // Check to see if the inter frame time has elapsed.
              if (tv < inter_frame_time)
                {
                  // Inter frame time has not elapsed.

                  // Claculate the time to wait before the next frame needs to be sent.
                  ACE_Time_Value wait_time (inter_frame_time - tv);

                  if (TAO_debug_level > 0)
                    ACE_DEBUG ((LM_DEBUG,
                                "Wait Time = %d\n",
                                wait_time.msec ()));

                  // run the orb for the wait time so the client can continue other orb requests.
                  TAO_AV_CORE::instance ()->orb ()->run (wait_time);
                }
            }

          // Start timer before sending the frame.
          last_frame_sent_time.start ();

          // Send frame.
          int result = this->protocol_object_->send_frame (&this->mb);
          if (result < 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "send failed:%p","FTP_Client_Flow_Handler::send\n"),
                              -1);
          ACE_DEBUG ((LM_DEBUG,"Client::pace_data buffer sent succesfully\n"));

          // Reset the mb.
          this->mb.reset ();

        } // end while

      // Since the file is read stop the stream.
      AVStreams::flowSpec stop_spec (1);
      CLIENT::instance ()->streamctrl ()->destroy (stop_spec);

      // Shut the orb down.
      TAO_AV_CORE::instance ()->orb ()->shutdown (0);

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Client::pace_data Failed");
      return -1;
    }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var obj
        = orb->resolve_initial_references ("RootPOA");


      //Get the POA_var object from Object_var
      PortableServer::POA_var root_poa
        = PortableServer::POA::_narrow (obj.in ());

      PortableServer::POAManager_var mgr
        = root_poa->the_POAManager ();

      mgr->activate ();

      // Initialize the AV STream components.
      TAO_AV_CORE::instance ()->init (orb.in (),
                                      root_poa.in ());

      // INitialize the Client.
      int result = 0;
      result = CLIENT::instance ()->init (argc,
                                          argv);

      if (result < 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "client::init failed\n"),1);

      // Start sending data.
      result = CLIENT::instance ()->pace_data ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)

    {
    ex._tao_print_exception ("Client Failed\n");
    return -1;
    }

  CLIENT::close ();

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Unmanaged_Singleton<Client, ACE_Null_Mutex> *ACE_Unmanaged_Singleton<Client, ACE_Null_Mutex>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */
