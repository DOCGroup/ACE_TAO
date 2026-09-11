#include "OnewayIdle_i.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#include "tao/ORB_Core.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Transport_Cache_Manager_T.h"

static const ACE_TCHAR *ior_output_file = ACE_TEXT ("test.ior");

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int c;
  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'o':
          ior_output_file = get_opts.opt_arg ();
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
  TAO_ORB_Core *core = orb->orb_core ();
  return core->lane_resources ().transport_cache ().current_size ();
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());
      PortableServer::POAManager_var manager = root_poa->the_POAManager ();

      OnewayIdle_i *impl = nullptr;
      ACE_NEW_RETURN (impl, OnewayIdle_i (orb.in ()), 1);
      PortableServer::ServantBase_var owner (impl);

      PortableServer::ObjectId_var oid = root_poa->activate_object (impl);
      CORBA::Object_var object = root_poa->id_to_reference (oid.in ());
      Test::OnewayIdle_var test = Test::OnewayIdle::_narrow (object.in ());

      CORBA::String_var ior = orb->object_to_string (test.in ());
      FILE *file = ACE_OS::fopen (ior_output_file, ACE_TEXT ("w"));
      if (file == nullptr)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) Cannot open output file '%s'\n"),
                           ior_output_file),
                          1);
      ACE_OS::fprintf (file, "%s", ior.in ());
      ACE_OS::fclose (file);

      manager->activate ();

      ACE_Time_Value run_time (2);
      orb->run (run_time);

      size_t const size = cache_size (orb.in ());
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("(%P|%t) server transport cache size after ORB run = %B\n"),
                  size));

      if (impl->test_failed () || size != 1)
        {
          if (size != 1)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) ERROR: expected server transport cache size 1, got %B\n"),
                        size));
          return 1;
        }

      root_poa->destroy (true, true);
      orb->destroy ();
    }
  catch (const CORBA::Exception &ex)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) server caught CORBA exception %C\n"),
                  ex._name ()));
      return 1;
    }

  return 0;
}
