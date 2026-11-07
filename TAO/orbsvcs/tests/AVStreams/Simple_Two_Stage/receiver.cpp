#include "receiver.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include <array>
#include <memory>

static FILE *output_file {};
// File handle of the file into which received data is written.

static ACE_TString output_file_name = ACE_TEXT ("output");
// File name of the file into which received data is written.

static ACE_TString stats_file_name = ACE_TEXT ("Stats.dat");

static constexpr std::size_t max_stats_samples {1000010};
static std::array<int, max_stats_samples> stats {};
static std::size_t stats_index {};

static bool start {true};
static ACE_Time_Value start_time {};

int
Receiver_StreamEndPoint::get_callback (const char *,
                                       TAO_AV_Callback *&callback)
{
  // Return the receiver application callback to the AVStreams for
  // future upcalls.
  callback = &this->callback_;
  return 0;
}

Receiver_Callback::Receiver_Callback ()
  : frame_count_ (0)
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

  if (start)
    {
      start_time = ACE_OS::gettimeofday ();
      start = false;
    }
  else
    {
      ACE_Time_Value elapsed_time = ACE_OS::gettimeofday () - start_time;
      if (stats_index == stats.size ())
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Receiver statistics capacity exceeded\n"),
                          -1);
      stats [stats_index++] = elapsed_time.msec ();
      start_time = ACE_OS::gettimeofday ();
    }


  while (frame != nullptr)
    {
      // Write the received data to the file.
      std::size_t const result =
        ACE_OS::fwrite (frame->rd_ptr (),
                        1,
                        frame->length (),
                        output_file);

      if (result != frame->length ())
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Receiver_Callback::fwrite failed\n"),
                          -1);

      frame = frame->cont ();
    }

  return 0;
}

int
Receiver_Callback::dump_samples (ACE_TCHAR const* file)
{
  ACE_DEBUG ((LM_DEBUG,
              "Dumping Stats.....\n"));

  std::unique_ptr<FILE, int (*) (FILE *)> stats_file (
    ACE_OS::fopen (file, "wb"), ACE_OS::fclose);

  if (!stats_file)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Stats file cannot be opened\n")),
                      -1);

  for (std::size_t i = 0; i < stats_index; ++i)
    this->stats_.sample (static_cast<ACE_UINT64> (stats [i]));

  this->stats_.dump_results (
    ACE_TEXT ("Inter Frame Arrival Time Statistics "), stats_file.get (), 1);

  for (std::size_t i = 0; i < stats_index; ++i)
    ACE_OS::fprintf (stats_file.get (), "%d\n", stats [i]);

  ACE_DEBUG ((LM_DEBUG,
              "Done\n"));
  return 0;
}

int
Receiver_Callback::handle_destroy ()
{
  // Called when the distributer requests the stream to be shutdown.
  ACE_DEBUG ((LM_DEBUG,
              "Receiver_Callback::end_stream\n"));

  if (this->dump_samples (stats_file_name.c_str ()) != 0)
    return -1;

  try
    {
      TAO_AV_CORE::instance ()->orb ()->shutdown (0);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Receiver_Callback::handle_destroy Failed\n");
      return -1;
    }

  return 0;
}

int
Receiver::init (int,
                ACE_TCHAR *[])
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
    this->mmdevice_->_this ();

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


int
parse_args (int argc,
            ACE_TCHAR *argv[])
{
  // Parse the command line arguments
  ACE_Get_Opt opts (argc,
                    argv,
                    "f:s:");

  int c;
  while ((c = opts ()) != -1)
    {
      switch (c)
        {
        case 'f':
          output_file_name = opts.opt_arg ();
          break;
        case 's':
          stats_file_name = opts.opt_arg ();
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
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
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

      int result =
        parse_args (argc,
                    argv);

      if (result == -1)
        return -1;

      // Make sure we have a valid <output_file>
      std::unique_ptr<FILE, int (*) (FILE *)> output_file_owner (
        ACE_OS::fopen (output_file_name.c_str (), "w"), ACE_OS::fclose);
      output_file = output_file_owner.get ();
      if (output_file == nullptr)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot open output file %s\n"),
                           output_file_name.c_str ()),
                          -1);

      else
        ACE_DEBUG ((LM_DEBUG,
                    "File Opened Successfully\n"));

      Receiver receiver;
      result =
        receiver.init (argc,
                       argv);

      if (result != 0)
        return result;

      orb->run ();

      // Hack for now....
      ACE_OS::sleep (1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("receiver::init");
      return -1;
    }

  return 0;
}
