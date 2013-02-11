// $Id$

#include "Hello.h"
#include "tao/Dynamic_TP/DTP_POA_Strategy.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Thread_Manager.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT ("server.ior");
int sleep_sec = 1;

// The parms below must be in a particular order for this test.
// min_threads_ = -1;
// init_threads_ = 1;
// max_threads_ = -1;
// stack_size_ = 0;
// timeout_ = 60;
// queue_depth_ = 0;

const ACE_TCHAR *parms = "{-1,1,-1,0,60,0}";

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:s:p:"));
  int c;

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
        sleep_sec = ACE_OS::strtol(get_opts.opt_arg (),NULL,0);
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
next_parm (const ACE_TCHAR **pstart)
{
  ACE_TCHAR *sep = 0;
  long rtn = ACE_OS::strtol (*pstart, &sep, 10);
  *pstart = sep + 1;
  if (!(**pstart == '-' || (**pstart >= '0' && **pstart <= '9')))
    ++(*pstart);
  return rtn;
}

void
set_parms ( TAO_DTP_Definition * this_config)
{
  // Get past the brace
  const ACE_TCHAR *pstart = parms + 1;

  // min_threads_ = -1;
  this_config->min_threads_ = next_parm (&pstart);
  // init_threads_ = 1;
  this_config->init_threads_ = next_parm (&pstart);
  // max_threads_ = 1;
  this_config->max_threads_ = next_parm (&pstart);
  // stack_size_ = 0;
  this_config->stack_size_ = next_parm (&pstart);
  // timeout_ = 60;
  this_config->timeout_.set (next_parm (&pstart),0);
  // queue_depth_ = 0;
  this_config->queue_depth_ = next_parm (&pstart);
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        return 1;


      TAO_DTP_Definition dtp_config;

      set_parms(&dtp_config);

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
    new TAO_DTP_POA_Strategy(&dtp_config, false);

  // Tell the strategy to apply itself to the child poa.
    if (csd_strategy->apply_to(root_poa.in()) == false)
      {
        ACE_ERROR((LM_ERROR,
                   "Failed to apply CSD strategy to root poa.\n"));
        return -1;
      }

      Hello *hello_impl;
      ACE_NEW_RETURN (hello_impl,
                      Hello (orb.in ()),
                      1);
      PortableServer::ServantBase_var owner_transfer(hello_impl);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (hello_impl);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      Test::Hello_var hello =
        Test::Hello::_narrow (object.in ());

      CORBA::String_var ior =
        orb->object_to_string (hello.in ());

      ACE_DEBUG ((LM_DEBUG,"Server calling poa_manager::activate()\n"));
      poa_manager->activate ();

      // Output the IOR to the <ior_output_file>
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file %s for writing IOR: %C",
                           ior_output_file,
                           ior.in ()),
                              1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);


      ACE_DEBUG ((LM_DEBUG,"Server calling hello->sleep_sec()\n"));
      // Set the sleep time for the Hello object
      hello->sleep_sec(sleep_sec);

      ACE_DEBUG ((LM_DEBUG,"Server calling orb::run()\n"));
      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));
      // Wait for all CSD task threads exit.
      ACE_Thread_Manager::instance ()->wait ();

      root_poa->destroy (1, 1);

      orb->destroy ();


    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
