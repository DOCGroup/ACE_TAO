#include "receiver.h"
#include "ace/Get_Opt.h"
#include <cstddef>
#include <cstring>
#include <memory>

// Borrowed by the callback; ACE_TMAIN owns the file for its duration.
static FILE *output_file {};

static ACE_TCHAR const output_file_name[] = ACE_TEXT ("output");

using RECEIVER = ACE_Unmanaged_Singleton<Receiver, ACE_Null_Mutex>;

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
Receiver_StreamEndPoint::set_protocol_object (char const *flowname,
                                              TAO_AV_Protocol_Object *object)
{
  // Set the sender protocol object corresponding to the transport
  // protocol selected.
  if (std::strcmp (flowname, "Data_Receiver1") == 0)
    RECEIVER::instance ()->protocol_object (object);
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
                                                 1,
                                                 frame->length (),
                                                 output_file);

      if (result != frame->length ())
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Receiver_Callback::fwrite failed\n"),
                          -1);

      frame = frame->cont ();
    }

  // Read from the file into a message block.
  std::size_t const n = ACE_OS::fread (this->mb_.wr_ptr (),
                                       1,
                                       this->mb_.size (),
                                       RECEIVER::instance ()->input_file ());

  if (n == 0)
    {
      // No more data is available to send back.
      ACE_DEBUG ((LM_DEBUG,"End of file\n"));
    }
  else
    {
      this->mb_.wr_ptr (n);

      // Send frame.
      int const result =
        RECEIVER::instance ()->protocol_object ()->send_frame (&this->mb_);

      if (result < 0)
        ACE_DEBUG ((LM_DEBUG,
                    "Send Frame Failed\n"));

      // Reset the message block.
      this->mb_.reset ();
    }
  return 0;
}

int
Receiver_Callback::handle_destroy ()
{
  // Called when the distributer requests the stream to be shutdown.
  ACE_DEBUG ((LM_DEBUG,
              "Receiver_Callback::handle_destroy\n"));

  static int count {};
  ++count;

  if (count < 2)
    {
      try
        {
          TAO_AV_CORE::instance ()->orb ()->shutdown (0);
        }
      catch (CORBA::Exception const &ex)
        {
          ex._tao_print_exception (
            "Receiver_Callback::handle_destroy Failed\n");
          return -1;
        }
    }

  return 0;
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
                      ACE_TCHAR *argv[])
{
  // Parse command line arguments
  ACE_Get_Opt opts (argc, argv, ACE_TEXT("f:d"));

  int c;
  while ((c = opts ()) != -1)
    {
      switch (c)
        {
        case 'f':
          this->options_.filename = ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ());
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
                ACE_TCHAR *argv[])
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

  // Open file to read.
  this->input_file_.reset (ACE_OS::fopen (this->options_.filename.c_str (), "rb"));

  if (!this->input_file_)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       "Cannot open input file %C\n",
                       this->options_.filename.c_str ()),
                      -1);
  else
    ACE_DEBUG ((LM_DEBUG,
                "Input File opened successfully\n"));

  // Register the receiver mmdevice object with the ORB
  TAO_MMDevice *mmdevice_servant {};
  ACE_NEW_RETURN (mmdevice_servant,
                  TAO_MMDevice (&this->reactive_strategy_),
                  -1);

  // Servant Reference Counting to manage lifetime
  PortableServer::ServantBase_var safe_mmdevice =
    mmdevice_servant;

  CORBA::Object_var mmdevice =
    mmdevice_servant->_this ();

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
                                mmdevice.in ());

  return 0;
}

TAO_AV_Protocol_Object *
Receiver::protocol_object () const
{
  return this->protocol_object_;
}

FILE *
Receiver::input_file () const
{
  return this->input_file_.get ();
}

int
ACE_TMAIN (int argc,
           ACE_TCHAR *argv[])
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
      TAO_AV_CORE::instance ()->init (orb.in (), root_poa.in ());

      // Make sure we have a valid <output_file>
      output_file_owner.reset (ACE_OS::fopen (output_file_name, "wb"));
      output_file = output_file_owner.get ();
      if (!output_file_owner)
        ACE_ERROR_RETURN ((LM_DEBUG,
                           "Cannot open output file %s\n",
                           output_file_name),
                          -1);

      else
        ACE_DEBUG ((LM_DEBUG,
                    "Output File Opened Successfully\n"));

      int const result = RECEIVER::instance ()->init (argc, argv);

      if (result != 0)
        return result;

      orb->run ();

      ACE_OS::sleep (1);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("receiver::init");
      return -1;
    }

  RECEIVER::close ();  // Explicitly finalize the Unmanaged_Singleton.

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Unmanaged_Singleton<Receiver, ACE_Null_Mutex> *
ACE_Unmanaged_Singleton<Receiver, ACE_Null_Mutex>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */
