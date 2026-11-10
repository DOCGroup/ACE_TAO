#include "receiver.h"
#include "ace/Get_Opt.h"
#include <cstddef>
#include <cstdio>
#include <memory>

// Borrowed by the callback; ACE_TMAIN owns the file for the whole ORB run.
static FILE *output_file {};

static bool done {};

int
Receiver_StreamEndPoint::get_callback (char const *,
                                       TAO_AV_Callback *&callback)
{
  // Return the receiver application callback to the AVStreams for
  // future upcalls.
  callback = &this->callback_;
  return 0;
}

int
Receiver_Callback::receive_frame (ACE_Message_Block *frame,
                                  TAO_AV_frame_info *,
                                  ACE_Addr const &)
{
  //
  // Upcall from the AVStreams when there is data to be received from
  // the sender.
  //
  ACE_DEBUG ((LM_DEBUG,
              "Receiver_Callback::receive_frame for frame %d\n",
              this->frame_count_++));

  while (frame != nullptr)
    {
      // Write the received data to the file.
      std::size_t const result = ACE_OS::fwrite (frame->rd_ptr (),
                                                 frame->length (),
                                                 1,
                                                 output_file);

      if (result != 1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Receiver_Callback::fwrite failed\n"),
                          -1);

      frame = frame->cont ();
    }

  return 0;
}

int
Receiver_Callback::handle_destroy ()
{
  // Called when the distributer requests the stream to be shutdown.
  ACE_DEBUG ((LM_DEBUG,
              "Receiver_Callback::end_stream\n"));

  done = true;
  return 0;
}

int
Receiver::init ()
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

  this->connection_manager_.load_ep_addr (
    ACE_TEXT_ALWAYS_CHAR (this->options_.addr_file.c_str ()));

  // Register the receiver mmdevice object with the ORB
  TAO_MMDevice *mmdevice_servant {};
  ACE_NEW_RETURN (mmdevice_servant,
                  TAO_MMDevice (&this->reactive_strategy_),
                  -1);

  // Servant Reference Counting to manage lifetime
  PortableServer::ServantBase_var safe_mmdevice =
    mmdevice_servant;

  AVStreams::MMDevice_var mmdevice =
    mmdevice_servant->_this ();

  // Bind to sender.
  this->connection_manager_.bind_to_sender (this->options_.sender_name,
                                            this->options_.receiver_name,
                                            mmdevice.in ());

  // Connect to the sender.
  this->connection_manager_.connect_to_sender ();

  return 0;
}

int
Receiver::parse_args (int argc, ACE_TCHAR *argv[])
{
  // Parse the command line arguments
  ACE_Get_Opt opts (argc, argv, ACE_TEXT ("f:s:r:a:"));

  int c {};
  while ((c = opts ()) != -1)
    {
      switch (c)
        {
        case 'a':
          this->options_.addr_file = opts.opt_arg ();
          break;
        case 'f':
          this->options_.output_file_name = opts.opt_arg ();
          break;
        case 's':
          this->options_.sender_name = ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ());
          break;
        case 'r':
          this->options_.receiver_name = ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ());
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Usage: receiver -f filename"),
                            -1);
        }
    }

  return 0;
}

ACE_TString const &
Receiver::output_file_name () const
{
  return this->options_.output_file_name;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  using File_Ptr = std::unique_ptr<FILE, int (*) (FILE *)>;
  File_Ptr output_file_owner (nullptr, ACE_OS::fclose);

  try
    {
      // Initialize the ORB first.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var obj
        = orb->resolve_initial_references ("RootPOA");

      // Get the POA_var object from Object_var.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      PortableServer::POAManager_var mgr
        = root_poa->the_POAManager ();

      mgr->activate ();

      // Initialize the AVStreams components.
      TAO_AV_CORE::instance ()->init (orb.in (),
                                      root_poa.in ());

      Receiver receiver;
      int result =
        receiver.parse_args (argc, argv);
      if (result == -1)
        return -1;

      // Make sure we have a valid <output_file>
      output_file_owner.reset (
        ACE_OS::fopen (receiver.output_file_name ().c_str (), "w"));
      output_file = output_file_owner.get ();
      if (!output_file_owner)
        ACE_ERROR_RETURN ((LM_DEBUG,
                           "Cannot open output file %s\n",
                           receiver.output_file_name ().c_str () ),
                          -1);

      else
        ACE_DEBUG ((LM_DEBUG,
                    "File Opened Successfully\n"));

      result = receiver.init ();

      if (result != 0)
        return result;

      ACE_Time_Value tv (0, 10000);
      while (!done)
        orb->run (tv);

      // Hack for now....
      ACE_OS::sleep (1);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("receiver::init");
      return -1;
    }

  return 0;
}
