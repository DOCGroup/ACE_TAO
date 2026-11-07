#include "Hello.h"
#include "tao/Dynamic_TP/DTP_POA_Strategy.h"
#include "tao/Dynamic_TP/DTP_POA_Loader.h"
#include "tao/CSD_Framework/CSD_Framework_Loader.h"
#include "tao/CSD_Framework/CSD_ORBInitializer.h"
#include "tao/CSD_ThreadPool/CSD_ThreadPool.h"

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Thread_Manager.h"
#include <memory>

ACE_TCHAR const default_parms[] = ACE_TEXT ("{-1,1,-1,0,60,0}");

int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TString &ior_output_file,
            ACE_TString &parms, int &sleep_sec)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:s:p:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case 'p':
        parms = get_opts.opt_arg ();
        break;
      case 's':
        sleep_sec = static_cast<int> (ACE_OS::strtol (get_opts.opt_arg (), nullptr, 0));
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "-p (parm array)"
                           "-s (# sec to sleep)"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

long
next_parm (ACE_TCHAR const *&pstart)
{
  ACE_TCHAR *sep = nullptr;
  long const rtn = ACE_OS::strtol (pstart, &sep, 10);
  pstart = sep + 1;
  if (!(*pstart == '-' || (*pstart >= '0' && *pstart <= '9')))
    ++pstart;
  return rtn;
}

void
set_parms (TAO_DTP_Definition &this_config, ACE_TString const &parms)
{
  // Get past the brace
  ACE_TCHAR const *pstart = parms.c_str () + 1;

  // min_threads_ = -1;
  this_config.min_threads_ = next_parm (pstart);
  // init_threads_ = 1;
  this_config.init_threads_ = next_parm (pstart);
  // max_threads_ = 1;
  this_config.max_threads_ = next_parm (pstart);
  // stack_size_ = 0;
  this_config.stack_size_ = next_parm (pstart);
  // timeout_ = 60;
  this_config.timeout_.set (next_parm (pstart), 0);
  // queue_depth_ = 0;
  this_config.queue_depth_ = next_parm (pstart);
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      ACE_TString ior_output_file (ACE_TEXT ("server.ior"));
      ACE_TString parms (default_parms);
      int sleep_sec {1};
      if (parse_args (argc, argv, ior_output_file, parms, sleep_sec) != 0)
        return 1;

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      TAO_DTP_Definition dtp_config;

      set_parms (dtp_config, parms);

      //dtp_config.min_threads_ = -1;
      //dtp_config.init_threads_ = 5;
      //dtp_config.max_threads_ = -1;
      //dtp_config.stack_size_ = (40 * 1024);
      //dtp_config.timeout_.set(10,0);
      //dtp_config.queue_depth_ = 10;

      ACE_DEBUG((LM_INFO,ACE_TEXT("TAO (%P|%t) - Server : min_threads_ = %d\n"),dtp_config.min_threads_));
      ACE_DEBUG((LM_INFO,ACE_TEXT("TAO (%P|%t) - Server : init_threads_ = %d\n"),dtp_config.init_threads_));
      ACE_DEBUG((LM_INFO,ACE_TEXT("TAO (%P|%t) - Server : max_threads_ = %d\n"),dtp_config.max_threads_));
      ACE_DEBUG((LM_INFO,ACE_TEXT("TAO (%P|%t) - Server : stack_size_ = %d\n"),dtp_config.stack_size_));
      ACE_DEBUG((LM_INFO,ACE_TEXT("TAO (%P|%t) - Server : timeout_ = %d\n"),dtp_config.timeout_.sec()));
      ACE_DEBUG((LM_INFO,ACE_TEXT("TAO (%P|%t) - Server : queue_depth_ = %d\n"),dtp_config.queue_depth_));
      ACE_DEBUG((LM_INFO,ACE_TEXT("TAO (%P|%t) - Server : sleep_sec = %d\n"),sleep_sec));

  // Create the thread pool servant dispatching strategy object, and
  // hold it in a (local) smart pointer variable.
      TAO_Intrusive_Ref_Count_Handle<TAO_DTP_POA_Strategy> csd_strategy =
        new TAO_DTP_POA_Strategy (&dtp_config, false);

  // Tell the strategy to apply itself to the child poa.
      if (!csd_strategy->apply_to (root_poa.in ()))
      {
        ACE_ERROR((LM_ERROR,
                   "Failed to apply CSD strategy to root poa.\n"));
        return -1;
      }

      std::unique_ptr<Hello> hello_impl = std::make_unique<Hello> (orb.in ());
      PortableServer::ServantBase_var owner_transfer (hello_impl.release ());

      PortableServer::ObjectId_var id =
        root_poa->activate_object (owner_transfer.in ());

      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

      Test::Hello_var const hello =
        Test::Hello::_narrow (object.in ());

      CORBA::String_var const ior =
        orb->object_to_string (hello.in ());

      ACE_DEBUG ((LM_DEBUG,"Server calling poa_manager::activate()\n"));
      poa_manager->activate ();

      // Output the IOR to the <ior_output_file>
      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (ior_output_file.c_str (), "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file %s for writing IOR: %C",
                             ior_output_file.c_str (),
                             ior.in ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }


      ACE_DEBUG ((LM_DEBUG,"Server calling hello->sleep_sec()\n"));
      // Set the sleep time for the Hello object
      hello->sleep_sec (sleep_sec);

      ACE_DEBUG ((LM_DEBUG,"Server calling orb::run()\n"));
      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));
      // Wait for all CSD task threads exit.
      ACE_Thread_Manager::instance ()->wait ();

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
