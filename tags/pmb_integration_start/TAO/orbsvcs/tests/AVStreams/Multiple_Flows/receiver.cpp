// $Id$

#include "receiver.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"

int endstream = 0;

typedef ACE_Singleton<Receiver,ACE_Null_Mutex> RECEIVER;
//Create a singleton instance of the receiver.

int
Receiver_StreamEndPoint::get_callback (const char *flow_name,
                                       TAO_AV_Callback *&callback)
{
  Receiver_Callback *callback_;
  ACE_NEW_RETURN (callback_,
                  Receiver_Callback,
                  -1);

  // Return the receiver application callback to the AVStreams for
  // future upcalls.
  callback = callback_;
  callback_->flowname (flow_name);
  return 0;
}


int
Receiver_StreamEndPoint::set_protocol_object (const char * flowname,
                                              TAO_AV_Protocol_Object *object)
{
  // Set the sender protocol object corresponding to the transport
  // protocol selected.
  if (ACE_OS::strcmp (flowname, "Data_Receiver1") == 0)
    RECEIVER::instance ()->protocol_object (object);
  return 0;
}

Receiver_Callback::Receiver_Callback (void)
  : frame_count_ (1),
      mb_ (BUFSIZ)
{
  ACE_DEBUG ((LM_DEBUG,
              "Receiver_Callback::Receiver_Callback\n"));
}

void
Receiver_Callback::flowname (const char* flow_name)
{
  this->flowname_ = flow_name;

  // Make sure we have a valid <output_file>
  this->output_file_ = ACE_OS::fopen (this->flowname_.c_str (),
                                      "w");
  if (this->output_file_ == 0)
    ACE_ERROR ((LM_DEBUG,
                "Cannot open output file %s\n",
                this->flowname_.c_str ()));

  else
    ACE_DEBUG ((LM_DEBUG,
                "%s File Opened Successfully\n",
                this->flowname_.c_str ()));


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
              "Receiver_Callback::receive_frame for frame %d for flow %s\n",
              this->frame_count_++,
              this->flowname_.c_str ()));

  while (frame != 0)
    {
      // Write the received data to the file.
      size_t result =
        ACE_OS::fwrite (frame->rd_ptr (),
                        frame->length (),
                        1,
                        this->output_file_);

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
              "Receiver_Callback::handle_destroy\n"));

  endstream++;

  return 0;
}

Receiver::Receiver (void)
  : mmdevice_ (0),
    frame_rate_ (30),
    frame_count_ (0),
    filename_ ("input"),
    mb_ (BUFSIZ)
{
}

Receiver::~Receiver (void)
{
}

void
Receiver::protocol_object (TAO_AV_Protocol_Object *object)
{
  // Set the sender protocol object corresponding to the transport
  // protocol selected.
  this->protocol_object_ = object;
}

int
Receiver::parse_args (int argc,
                      char **argv)
{
  // Parse command line arguments
  ACE_Get_Opt opts (argc, argv, "f:r:d");

  int c;
  while ((c= opts ()) != -1)
    {
      switch (c)
        {
        case 'f':
          this->filename_ = opts.opt_arg ();
          break;
        case 'r':
          this->frame_rate_ = ACE_OS::atoi (opts.opt_arg ());
          break;
        case 'd':
          TAO_debug_level++;
          break;
        default:
          ACE_DEBUG ((LM_DEBUG, "Unknown Option\n"));
          return -1;
        }
    }
  return 0;
}

int
Receiver::init (int argc,
                char ** argv
                ACE_ENV_ARG_DECL)
{
  // Initialize the endpoint strategy with the orb and poa.
  int result =
    this->reactive_strategy_.init (TAO_AV_CORE::instance ()->orb (),
                                   TAO_AV_CORE::instance ()->poa ());
  if (result != 0)
    return result;

    // Parse the command line arguments
  result =
    this->parse_args (argc,
                      argv);
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

TAO_AV_Protocol_Object*
Receiver::protocol_object (void)
{
  return this->protocol_object_;
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
        RECEIVER::instance ()->init (argc,
                                     argv
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (result != 0)
        return result;

      while (endstream != 2)
        {
          orb->perform_work (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

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
