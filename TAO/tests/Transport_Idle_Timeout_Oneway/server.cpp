#include "OnewayIdle_i.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#include "tao/ORB_Core.h"
#include "tao/Resource_Factory.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Transport_Cache_Manager_T.h"
#include <chrono>
#include <memory>
#include <string>

struct Server_Options
{
  std::basic_string<ACE_TCHAR> ior_output_file {ACE_TEXT ("test.ior")};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int option {};

  while ((option = get_opts ()) != -1)
    {
      switch (option)
        {
        case 'o':
          options.ior_output_file = get_opts.opt_arg ();
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%P|%t) Usage: server [-o <ior_file>]\n")),
                            -1);
        }
    }
  return 0;
}

static size_t
cache_size (CORBA::ORB_ptr orb)
{
  TAO_ORB_Core * const core = orb->orb_core ();
  return core->lane_resources ().transport_cache ().current_size ();
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Server_Options options;
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());
      PortableServer::POAManager_var manager = root_poa->the_POAManager ();

      OnewayIdle_i *impl {};
      ACE_NEW_RETURN (impl, OnewayIdle_i (orb.in ()), 1);
      PortableServer::ServantBase_var owner (impl);

      PortableServer::ObjectId_var oid = root_poa->activate_object (impl);
      CORBA::Object_var object = root_poa->id_to_reference (oid.in ());
      Test::OnewayIdle_var test = Test::OnewayIdle::_narrow (object.in ());

      CORBA::String_var ior = orb->object_to_string (test.in ());
      {
        std::unique_ptr<FILE, int (*) (FILE *)> file (
          ACE_OS::fopen (options.ior_output_file.c_str (), ACE_TEXT ("w")), ACE_OS::fclose);
        if (!file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%P|%t) Cannot open output file '%s'\n"),
                             options.ior_output_file.c_str ()),
                            1);
        ACE_OS::fprintf (file.get (), "%s", ior.in ());
      }

      manager->activate ();

      // Do not start the idle observation window until the oneway has run.
      // Also fail if no request arrives; an initially empty cache is not a pass.
      std::chrono::steady_clock::time_point const deadline =
        std::chrono::steady_clock::now () + std::chrono::seconds (30);
      while (!impl->request_received () && std::chrono::steady_clock::now () < deadline)
        {
          ACE_Time_Value slice (0, 50000);
          orb->perform_work (slice);
        }
      if (!impl->request_received ())
        {
          ACE_ERROR_RETURN ((LM_ERROR,
            ACE_TEXT ("(%P|%t) ERROR: oneway request was not received\n")), 1);
        }

      TAO_Resource_Factory *factory = orb->orb_core ()->resource_factory ();
      ACE_Time_Value run_time (factory->transport_idle_timeout ()
                               + factory->transport_idle_scan_interval () + 1);
      orb->run (run_time);

      size_t const size = cache_size (orb.in ());
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("(%P|%t) server transport cache size after ORB run = %B\n"),
                  size));

      if (impl->test_failed () || size != 0)
        {
          if (size != 0)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) ERROR: expected server transport cache size 0, got %B\n"),
                        size));
          return 1;
        }

      root_poa->destroy (true, true);
      orb->destroy ();
    }
  catch (CORBA::Exception const & ex)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) server caught CORBA exception %C\n"),
                  ex._name ()));
      return 1;
    }

  return 0;
}
