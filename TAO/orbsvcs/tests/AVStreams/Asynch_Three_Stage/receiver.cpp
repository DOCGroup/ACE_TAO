// $Id$

#include "receiver.h"
#include "ace/Get_Opt.h"

static FILE *output_file = 0;
// File into which the received data is written.

static const char *output_file_name = "output";
// File handle of the file into which data is written.

ACE_CString sender_device_name_;
// Sender device name.

ACE_CString receiver_device_name_;
// Receiver device name.

int
Receiver_StreamEndPoint::get_callback (const char *,
                                         TAO_AV_Callback *&callback)
{
  // Return the application callback to the AVStreams for further upcalls
  callback = &this->callback_;
  return 0;
}

int
Receiver_Callback::receive_frame (ACE_Message_Block *frame,
                                    TAO_AV_frame_info *,
                                    const ACE_Addr &)
{
  // Upcall from the AVStreams when there is data to be received from the
  // distributer.

  ACE_DEBUG ((LM_DEBUG,
              "Receiver_Callback::receive_frame\n"));

  while (frame != 0)
    {
      // Write the received data to the file.
      unsigned int result = ACE_OS::fwrite (frame->rd_ptr (),
                                            frame->length (),
                                            1,
                                            output_file);
      
      if (result == frame->length ())
        ACE_ERROR_RETURN ((LM_ERROR,
                           "FTP_Server_Flow_Handler::fwrite failed\n"),
                          -1);
      
      frame = frame->cont ();
    }
  return 0;
}

int
Receiver_Callback::handle_destroy (void)
{
  // Called when the distributer requests the stream to be shutdown.
  ACE_DEBUG ((LM_DEBUG,
              "Receiver_Callback::end_stream\n"));
  TAO_AV_CORE::instance ()->orb ()->shutdown ();
  return 0;
}


int
Receiver_Callback::handle_stop (void)
{
  // Called when the distributer requests the stream to be shutdown.
  ACE_DEBUG ((LM_DEBUG,
              "Receiver_Callback::end_stream\n"));
  TAO_AV_CORE::instance ()->orb ()->shutdown ();
  return 0;
}

Receiver::Receiver (void)
  : default_port_ (10000),
    protocol_ ("UDP")
{
}

Receiver::~Receiver (void)
{
}

int
Receiver::init (int,
                char **,
                CORBA::Environment &ACE_TRY_ENV)
{
  // Initialize the endpoint strategy with the orb and poa.
  int result =
    this->reactive_strategy_.init (TAO_AV_CORE::instance ()->orb (),
                                   TAO_AV_CORE::instance ()->poa ());
  
  if (result != 0)
    return result;

  // Initialize the helper.
  this->helper_.init (TAO_AV_CORE::instance ()->orb (), 
                      TAO_AV_CORE::instance ()->poa ());
  
  // Register the receiver mmdevice object with the ORB 
  ACE_NEW_RETURN (this->mmdevice_,
                  TAO_MMDevice (&this->reactive_strategy_),
                  -1);

  // Servant Reference Counting to manage lifetime
  PortableServer::ServantBase_var safe_mmdevice =
    this->mmdevice_;

  this->mmdevice_obj_ =
    this->mmdevice_->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN(-1);
  
  // Bind to sender
  result = this->helper_.bind_to_sender (&sender_device_name_,
                                         &receiver_device_name_,
                                         this->sender_mmdevice_.inout (),
                                         this->mmdevice_obj_.in (),
                                         ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);
  
  ACE_DEBUG ((LM_DEBUG,
              "Bound to senders\n"));
  
  if (result != 0)
    return result;

  ACE_CString flow_protocol_str ("");

  ACE_CString address ("");

  result = this->helper_.connect_to_sender (&sender_device_name_,
                                            &receiver_device_name_,
                                            &flow_protocol_str,
                                            &this->protocol_,
                                            &address,
                                            this->sender_mmdevice_.inout (),
                                            this->mmdevice_obj_.in (),
                                            this->streamctrl_map_,
                                            ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);
  
  ACE_DEBUG ((LM_DEBUG,
              "Connected to senders\n"));
  
  if (result != 0)
    return result;

  return 0;
}

int
parse_args (int argc,
            char **argv)
{
  // Parse the command line arguments
  ACE_Get_Opt opts (argc,
                    argv,
                    "f:s:r:");

  int c;
  while ((c = opts ()) != -1)
    {
      switch (c)
        {
        case 'f':
          output_file_name = opts.optarg;
          break;
        case 's':
          sender_device_name_ = opts.optarg;
          break;
        case 'r':
          receiver_device_name_ = opts.optarg;
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Usage: server -f filename"),
                            -1);
        }
    }

  return 0;
}

int
main (int argc,
      char **argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Initialize the ORB first.
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            0,
                                            ACE_TRY_ENV);
      ACE_TRY_CHECK;

      int result =
        parse_args (argc,
                    argv);

      if (result == -1)
        return -1;

      // Make sure we have a valid <output_file>
      output_file = ACE_OS::fopen (output_file_name,
                                   "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_DEBUG,
                           "Cannot open output file %s\n",
                           output_file_name),
                          -1);

      else ACE_DEBUG ((LM_DEBUG,
		       "File Opened Successfull\n"));

      CORBA::Object_var obj
        = orb->resolve_initial_references ("RootPOA",
                                           ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Get the POA_var object from Object_var.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in (),
                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var mgr
        = root_poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      mgr->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Initialize the AVStreams components.
      TAO_AV_CORE::instance ()->init (orb.in (),
                                      root_poa.in (),
                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Receiver receiver;
      result =
        receiver.init (argc,
                     argv,
                     ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      if (result != 0)
        return result;
      
      orb->run (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"receiver::init");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  ACE_OS::fclose (output_file);

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class TAO_AV_Endpoint_Reactive_Strategy_B
<Receiver_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>;
template class TAO_AV_Endpoint_Reactive_Strategy
<Receiver_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy_B
<Receiver_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy
<Receiver_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
