// $Id$

#include "sender.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"

ACE_High_Res_Timer last_frame_sent_time;
// The time taken for sending a frmae and preparing for the next frame

ACE_Time_Value inter_frame_time;
// The time that should lapse between two consecutive frames sent.

int
Sender_Callback::handle_start (void)
{
  // Connection setup, start sending data.

  ACE_DECLARE_NEW_CORBA_ENV;
  SENDER::instance ()->pace_data (ACE_TRY_ENV);
  return 0;
}

Sender_StreamEndPoint::Sender_StreamEndPoint (void)
{
}

int
Sender_StreamEndPoint::get_callback (const char *,
                                     TAO_AV_Callback *&callback)
{
  // Create and return the client application callback and return to the AVStreams
  // for further upcalls.
  callback = &this->callback_;

  // Store reference to the sender stream endpoint
  SENDER::instance ()->set_endpoint (this);
  return 0;
}

int
Sender_StreamEndPoint::set_protocol_object (const char *,
                                            TAO_AV_Protocol_Object *object)
{
  // Set the client protocol object corresponding to the transport protocol selected.
  SENDER::instance ()->set_protocol_object (object);
  return 0;
}

Sender::Sender (void)
  :count_ (0),
   fp_ (0),
   frame_rate_ (30)
{
  this->mb.size (BUFSIZ);
}

void
Sender::set_protocol_object (TAO_AV_Protocol_Object *object)
{
  // Set the client protocol object corresponding to the transport protocol selected.
  this->protocol_object_ = object;
}

void
Sender::set_endpoint (Sender_StreamEndPoint* endpoint)
{
  // Store the sender stream endpoint
  this->endpoint_ = endpoint;
}

Sender_StreamEndPoint*
Sender::get_endpoint (void)
{
  // Return the sender stream endpoint reference.
  return this->endpoint_;
}

int
Sender::parse_args (int argc,
                    char **argv)
{
  // Parse command line arguments
  ACE_Get_Opt opts (argc,argv,"f:r:s");

  int c;
  while ((c= opts ()) != -1)
    {
      switch (c)
        {
        case 'f':
          this->filename_ = opts.optarg;
          if (TAO_debug_level > 0) 
            ACE_DEBUG ((LM_DEBUG,
                        "Input File Name %s\n",
                        this->filename_.c_str ()));                      
          break;
	case 'r':
	  this->frame_rate_ = ACE_OS::atoi (opts.optarg);
	  break;
        default:
          ACE_DEBUG ((LM_DEBUG,"Unknown Option\n"));
          return -1;
        }
    }
  return 0;
}

FILE *
Sender::file (void)
{
  return this->fp_;
}

TAO_StreamCtrl*
Sender::streamctrl (void)
{
  return &this->streamctrl_;
}


int
Sender::frame_rate (void)
{
  return this->frame_rate_;
}


// Method to bind the sender reference to the Naming Service.
int
Sender::register_sender (CORBA::Environment &ACE_TRY_ENV)
{
  // Initialize the naming services
  if (my_naming_client_.init (TAO_AV_CORE::instance ()->orb ()) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
		       " (%P|%t) Unable to initialize "
		       "the TAO_Naming_Client. \n"),
		      -1);
  
  // Register the sender mmdevice object with the ORB 
  ACE_NEW_RETURN (this->sender_mmdevice_,
                  TAO_MMDevice (&this->endpoint_strategy_),
                  -1);
  
  CosNaming::Name sender_mmdevice_name (1);
  sender_mmdevice_name.length (1);
  sender_mmdevice_name [0].id = CORBA::string_dup ("Sender");
  
  CORBA::Object_var mmdevice =
    this->sender_mmdevice_->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN(-1);
  
  // Register the server object with the naming server.
  this->my_naming_client_->rebind (sender_mmdevice_name,
                                   mmdevice.in (),
                                   ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
Sender::init (int argc,
	      char **argv,
	      CORBA::Environment& ACE_TRY_ENV)
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

  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
		       " (%P|%t) Error in Parse Args \n"),
		      -1);

  
  // Open file to read.
  this->fp_ = ACE_OS::fopen (this->filename_.c_str (),
			     "r");
  if (this->fp_ == 0)
    ACE_ERROR_RETURN ((LM_DEBUG,
		       "Cannot open output file %s\n",
		       this->filename_.c_str ()),
		      -1);
  else ACE_DEBUG ((LM_DEBUG,
		   "File opened successfully\n"));		  
 
  
  // Register the object reference with the Naming Service.
  if (this->register_sender (ACE_TRY_ENV) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Error binding to the naming service\n"),
                      -1);
  
  return 0;
}

// Method to send data at the specified rate
int
Sender::pace_data (CORBA::Environment& ACE_TRY_ENV)
{

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
          
          // Reset the message block.
          this->mb.reset ();
          
          // Read from the file into a message block.
          int n = ACE_OS::fread (this->mb.wr_ptr (),
                                 1,
                                 this->mb.size (),
                                 SENDER::instance ()->file ());
	  
          this->mb.wr_ptr (n);
          
          if (n < 0)
	    ACE_ERROR_RETURN ((LM_ERROR,
			       "FTP_Client_Flow_Handler::fread end of file\n"),
			      -1);
	  
	  if (n == 0)
	    {
	      if (feof (SENDER::instance ()->file ()))
		{
                  // At end of file break the loop and end the client.
                  if (TAO_debug_level > 0)
                    ACE_DEBUG ((LM_DEBUG,"Handle_Start:End of file\n"));
                  break;
                }
	      
	    }
          
  	  if (this->count_ > 1)
  	    {
              // Second frame and beyond
              
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
  		  TAO_AV_CORE::instance ()->orb ()->run (wait_time,
  							 ACE_TRY_ENV);
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
          
        } // end while


      AVStreams::flowSpec stop_spec;
      
      // Get the strem controoler for this stream.
      CORBA::Any_ptr streamctrl_any = SENDER::instance ()->get_endpoint ()->get_property_value ("Related_StreamCtrl",
                                                                                                ACE_TRY_ENV);
      
      AVStreams::StreamCtrl_ptr streamctrl;
      
      *streamctrl_any >>= streamctrl;

      // Destroy the stream
      streamctrl->destroy (stop_spec,
                           ACE_TRY_ENV);
      
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
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            0,
                                            ACE_TRY_ENV);
      CORBA::Object_var obj
        = orb->resolve_initial_references ("RootPOA",
					   ACE_TRY_ENV);
      ACE_TRY_CHECK;


      //Get the POA_var object from Object_var
      PortableServer::POA_var root_poa
        = PortableServer::POA::_narrow (obj.in (),
					ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var mgr
        = root_poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      mgr->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Initialize the AV Stream components.
      TAO_AV_CORE::instance ()->init (orb.in (),
                                      root_poa.in (),
                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Initialize the Client.
      int result = 0;
      result = SENDER::instance ()->init (argc,
					  argv,
					  ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (result < 0)
        ACE_ERROR_RETURN ((LM_ERROR,
			   "client::init failed\n"), -1);

      orb->run (ACE_TRY_ENV);
      ACE_TRY_CHECK;

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"Sender Failed\n");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton <Sender,ACE_Null_Mutex>;
template class
TAO_AV_Endpoint_Reactive_Strategy_B<Sender_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>;
template class
TAO_AV_Endpoint_Reactive_Strategy<Sender_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton <Client,ACE_Null_Mutex>
#pragma instantiate
TAO_AV_Endpoint_Reactive_Strategy_B<Sender_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>
#pragma instantiate
TAO_AV_Endpoint_Reactive_Strategy<Sender_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

