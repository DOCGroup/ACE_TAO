// $Id$

#include "receiver.h"
#include "ace/Get_Opt.h"

typedef ACE_Singleton<Receiver, ACE_Null_Mutex> RECEIVER;
// Create a singleton instance of the Sender.

static FILE *output_file = 0;
// File handle of the file into which received data is written.

static const char *output_file_name = "output";
// File name of the file into which received data is written.

int
Receiver_StreamEndPoint::get_callback (const char *,
                                       TAO_AV_Callback *&callback)
{
  // Return the receiver application callback to the AVStreams for
  // future upcalls.
  callback = &this->callback_;


  // Get the stream controller for this stream.
  ACE_TRY_NEW_ENV
    {
      CORBA::Any_ptr streamctrl_any =
        this->get_property_value ("Related_StreamCtrl"
                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      AVStreams::StreamCtrl_ptr streamctrl;
      *streamctrl_any >>= streamctrl;

      // Store the stream control for the stream with the callback.
      this->callback_.streamctrl (streamctrl);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Sender_StreamEndPoint::get_callback failed");

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

Receiver_Callback::Receiver_Callback (void)
  : frame_count_ (0)
{
}

void
Receiver_Callback::streamctrl (AVStreams::StreamCtrl_ptr streamctrl)
{
  // Set the sender protocol object corresponding to the transport
  // protocol selected.
  this->streamctrl_ = streamctrl;
}


int
Receiver_Callback::receive_frame (ACE_Message_Block *frame,
                                  TAO_AV_frame_info *,
                                  const ACE_Addr &)
{
  //
  // Upcall from the AVStreams when there is data to be received from
  // the sender.
  //
  ACE_DEBUG ((LM_DEBUG,
              "Receiver_Callback::receive_frame for frame %d\n",
              ++this->frame_count_));


  if (this->streamctrl_ != 0)
    {
      // Modify QoS after receiving 20 frames
      if (frame_count_ == 20)
        {
          // Specify the flow for which the qos needs to be changed.
          TAO_Forward_FlowSpec_Entry entry ("Data_Receiver",
                                            "IN",
                                            "",
                                            "",
                                            0);

          AVStreams::flowSpec flow_spec (1);
          flow_spec.length (1);
          flow_spec [0] = CORBA::string_dup (entry.entry_to_string ());

          // Initialize the qos parameter(s) that need to be changed
          // with the corresponding value.
          AVStreams::streamQoS qos;
          qos.length (1);

          // The QoS Type that needs to be changed.
          qos [0].QoSType =  CORBA::string_dup ("video_qos");

          qos [0].QoSParams.length (1);
          qos [0].QoSParams [0].property_name = CORBA::string_dup ("video_frame_rate");
          qos [0].QoSParams [0].property_value <<= (CORBA::Short) 30;

          ACE_DECLARE_NEW_CORBA_ENV;

          // Initiate the modifying of the qos for the flows.
          this->streamctrl_->modify_QoS (qos, flow_spec ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          ACE_DEBUG ((LM_DEBUG,
                      "Stream Ctrl available....Modify QoS called\n"));
        }
    }
  else ACE_DEBUG ((LM_DEBUG,
                   "No Stream Ctrl\n"));

  while (frame != 0)
    {
      // Write the received data to the file.
      size_t result =
        ACE_OS::fwrite (frame->rd_ptr (),
                        frame->length (),
                        1,
                        output_file);

      if (result == frame->length ())
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Receiver_Callback::fwrite failed\n"),
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

  ACE_TRY_NEW_ENV
    {
      TAO_AV_CORE::instance ()->orb ()->shutdown (0
                                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Receiver_Callback::handle_destroy Failed\n");
      return -1;

    }
  ACE_ENDTRY;

  return 0;
}

Receiver::Receiver (void)
  : mmdevice_ (0)
{
}

Receiver::~Receiver (void)
{
}



int
Receiver::init (int,
                char **
                ACE_ENV_ARG_DECL)
{
  // Initialize the endpoint strategy with the orb and poa.
  int result =
    this->reactive_strategy_.init (TAO_AV_CORE::instance ()->orb (),
                                   TAO_AV_CORE::instance ()->poa ());
  if (result != 0)
    return result;

  // Register the receiver mmdevice object with the ORB
  ACE_NEW_RETURN (this->mmdevice_,
                  TAO_MMDevice (&this->reactive_strategy_),
                  -1);

  // Servant Reference Counting to manage lifetime
  PortableServer::ServantBase_var safe_mmdevice =
    this->mmdevice_;

  CORBA::Object_var mmdevice =
    this->mmdevice_->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Register the mmdevice with the naming service.
  CosNaming::Name name (1);
  name.length (1);
  name [0].id =
    CORBA::string_dup ("Receiver");

  // Initialize the naming services
  if (this->naming_client_.init (TAO_AV_CORE::instance ()->orb ()) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to initialize "
                       "the TAO_Naming_Client\n"),
                      -1);

  // Register the receiver object with the naming server.
  this->naming_client_->rebind (name,
                                mmdevice.in ()
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
parse_args (int argc,
            char **argv)
{
  // Parse the command line arguments
  ACE_Get_Opt opts (argc,
                    argv,
                    "f:");

  int c;
  while ((c = opts ()) != -1)
    {
      switch (c)
        {
        case 'f':
          output_file_name = opts.opt_arg ();
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Usage: receiver -f filename"),
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
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         0
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var obj
        = orb->resolve_initial_references ("RootPOA"
                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get the POA_var object from Object_var.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var mgr
        = root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      mgr->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Initialize the AVStreams components.
      TAO_AV_CORE::instance ()->init (orb.in (),
                                      root_poa.in ()
                                      ACE_ENV_ARG_PARAMETER);
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

      else
        ACE_DEBUG ((LM_DEBUG,
                    "File Opened Successfully\n"));

      Receiver receiver;
      result =
        receiver.init (argc,
                       argv
                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (result != 0)
        return result;

      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Hack for now....
      ACE_OS::sleep (1);

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
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
template class ACE_Singleton <Receiver,ACE_Null_Mutex>;
template class TAO_AV_Endpoint_Reactive_Strategy_B<Receiver_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>;
template class TAO_AV_Endpoint_Reactive_Strategy<Receiver_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton <Receiver,ACE_Null_Mutex>
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy_B<Receiver_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy<Receiver_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
