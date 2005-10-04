// $Id$

#include "receiver.h"
#include "ace/Get_Opt.h"

// File handle of the file into which received data is written.
static FILE *output_file = 0;

// Flag to tell us if we are done or not.
static int done=0;

int
Receiver_StreamEndPoint::get_callback (const char *,
                                       TAO_AV_Callback *&callback)
{
  // Return the receiver application callback to the AVStreams for
  // future upcalls.
  callback = &this->callback_;
  return 0;
}

Receiver_Callback::Receiver_Callback (void)
  : frame_count_ (1)
{
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
              this->frame_count_++));

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
      done=1;
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
  : mmdevice_ (0),
    output_file_name_ ("output"),
    addr_file_ ("addr_file"),
    sender_name_ ("distributer"),
    receiver_name_ ("receiver")
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

  // Initialize the connection manager.
  result =
    this->connection_manager_.init (TAO_AV_CORE::instance ()->orb ());
  if (result != 0)
    return result;

  this->connection_manager_.load_ep_addr (this->addr_file_.c_str ());

  // Register the receiver mmdevice object with the ORB
  ACE_NEW_RETURN (this->mmdevice_,
                  TAO_MMDevice (&this->reactive_strategy_),
                  -1);

  // Servant Reference Counting to manage lifetime
  PortableServer::ServantBase_var safe_mmdevice =
    this->mmdevice_;

  AVStreams::MMDevice_var mmdevice =
    this->mmdevice_->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Bind to sender.
  this->connection_manager_.bind_to_sender (this->sender_name_,
                                            this->receiver_name_,
                                            mmdevice.in ()
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Connect to the sender.
  this->connection_manager_.connect_to_sender (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
Receiver::parse_args (int argc,
                      char **argv)
{
  // Parse the command line arguments
  ACE_Get_Opt opts (argc,
                    argv,
                    "f:s:r:a:");

  int c;
  while ((c = opts ()) != -1)
    {
      switch (c)
        {
	case 'a':
	  this->addr_file_ = opts.opt_arg ();
	  break;
        case 'f':
          this->output_file_name_ = opts.opt_arg ();
          break;
        case 's':
          this->sender_name_ = opts.opt_arg ();
          break;
        case 'r':
          this->receiver_name_ = opts.opt_arg ();
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Usage: receiver -f filename"),
                            -1);
        }
    }

  return 0;
}

ACE_CString
Receiver::output_file_name (void)
{
  return this->output_file_name_;
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

      Receiver receiver;
      int result =
        receiver.parse_args (argc,
                             argv);
      if (result == -1)
        return -1;

      // Make sure we have a valid <output_file>
      output_file =
        ACE_OS::fopen (receiver.output_file_name ().c_str (),
                       "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_DEBUG,
                           "Cannot open output file %s\n",
                           receiver.output_file_name ().c_str ()),
                          -1);

      else
        ACE_DEBUG ((LM_DEBUG,
                    "File Opened Successfully\n"));

      result =
        receiver.init (argc,
                       argv
                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (result != 0)
        return result;

      ACE_Time_Value tv(0, 10000);
      while(!done)
      {
         orb->run (tv ACE_ENV_ARG_PARAMETER);
         ACE_TRY_CHECK;
      }

      // Hack for now....
      ACE_OS::sleep (1);

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
