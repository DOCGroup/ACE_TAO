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
   address_ (ACE_OS::strdup ("224.9.9.2:12345")),
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
                    char **argv)
{
  // Parse command line arguments
  ACE_Get_Opt opts (argc,argv,"f:a:p:r:sd");

  this->use_sfp_ = 0;

  int c;
  while ((c= opts ()) != -1)
    {
      switch (c)
        {
        case 'f':
          this->filename_ = ACE_OS::strdup (opts.opt_arg ());
          break;
        case 'a':
          this->address_ = ACE_OS::strdup (opts.opt_arg ());
          break;
        case 'p':
          this->protocol_ = ACE_OS::strdup (opts.opt_arg ());
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
Client::bind_to_server (TAO_ENV_SINGLE_ARG_DECL)
{
  // Initialize the naming services
  if (my_naming_client_.init (TAO_AV_CORE::instance ()->orb ()) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Unable to initialize "
                       "the TAO_Naming_Client. \n"),
                      -1);

  CosNaming::Name server_mmdevice_name (1);
  server_mmdevice_name.length (1);
  server_mmdevice_name [0].id = CORBA::string_dup ("Server_MMDevice");

  // Resolve the server object reference from the Naming Service
  CORBA::Object_var server_mmdevice_obj =
    my_naming_client_->resolve (server_mmdevice_name
                                TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->server_mmdevice_ =
    AVStreams::MMDevice::_narrow (server_mmdevice_obj.in ()
                                  TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (this->server_mmdevice_.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Could not resolve Server_MMdevice in Naming service <%s>\n"),
                      -1);

  return 0;
}

int
Client::init (int argc,
              char **argv
              TAO_ENV_ARG_DECL)
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
                       "Cannot open output file %s\n",
                       this->filename_),
                      -1);

  result
    = this->bind_to_server (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

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
  ACE_INET_Addr addr (this->address_);

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
                                    &addr);

  AVStreams::flowSpec flow_spec (1);
  flow_spec [0] = CORBA::string_dup (entry.entry_to_string ());
  flow_spec.length (1);

  AVStreams::MMDevice_var client_mmdevice =
    this->client_mmdevice_._this (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Bind/Connect  the client and server MMDevices.
  CORBA::Boolean bind_result =
    this->streamctrl_.bind_devs (client_mmdevice.in (),
                                 this->server_mmdevice_.in (),
                                 the_qos.inout (),
                                 flow_spec
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (bind_result == 0)
    ACE_ERROR_RETURN ((LM_ERROR,"streamctrl::bind_devs failed\n"),-1);

  return 0;
}

// Method to send data at the specified rate
int
Client::pace_data (TAO_ENV_SINGLE_ARG_DECL)
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

  ACE_TRY
    {

      // Continue to send data till the file is read to the end.
      while (1)
        {
          // Count the frames sent.
          count_++;

          // Read from the file into a message block.
          int n = ACE_OS::fread(this->mb.rd_ptr (),
                                1,
                                this->mb.size (),
                                CLIENT::instance ()->file ());

          if (n < 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "FTP_Client_Flow_Handler::fread end of file\n"),
                              -1);

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
                  TAO_AV_CORE::instance ()->orb ()->run (wait_time
                                                         TAO_ENV_ARG_PARAMETER);
                  ACE_TRY_CHECK;
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
      CLIENT::instance ()->streamctrl ()->destroy (stop_spec
                                                   TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Shut the orb down.
      TAO_AV_CORE::instance ()->orb ()->shutdown (0);
      ACE_TRY_CHECK;

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Client::pace_data Failed");
      return -1;
    }
  ACE_ENDTRY;
  return 0;
}

int
main (int argc,
      char **argv)
{
  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            0
                                            TAO_ENV_ARG_PARAMETER);
      CORBA::Object_var obj
        = orb->resolve_initial_references ("RootPOA"
                                           TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;


      //Get the POA_var object from Object_var
      PortableServer::POA_var root_poa
        = PortableServer::POA::_narrow (obj.in ()
                                        TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var mgr
        = root_poa->the_POAManager (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      mgr->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Initialize the AV STream components.
      TAO_AV_CORE::instance ()->init (orb.in (),
                                      root_poa.in ()
                                      TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // INitialize the Client.
      int result = 0;
      result = CLIENT::instance ()->init (argc,
                                          argv
                                          TAO_ENV_ARG_PARAMETER);
      if (result < 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "client::init failed\n"),1);

      // Start sending data.
      result = CLIENT::instance ()->pace_data (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
    }
  ACE_CATCHANY

    {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"Client Failed\n");
    return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton <Client,ACE_Null_Mutex>;
template class
TAO_AV_Endpoint_Reactive_Strategy_A<FTP_Client_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>;
template class
TAO_AV_Endpoint_Reactive_Strategy<FTP_Client_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton <Client,ACE_Null_Mutex>
#pragma instantiate
TAO_AV_Endpoint_Reactive_Strategy_A<FTP_Client_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>
#pragma instantiate
TAO_AV_Endpoint_Reactive_Strategy<FTP_Client_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
