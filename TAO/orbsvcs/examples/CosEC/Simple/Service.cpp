#include "orbsvcs/CosEvent/CEC_EventChannel.h"
#include "orbsvcs/CosEvent/CEC_Default_Factory.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include <memory>

int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const *&ior_output_file)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s -o <iorfile>\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  TAO_CEC_Default_Factory::init_svcs ();

  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      ACE_TCHAR const *ior_output_file = ACE_TEXT ("ec.ior");
      if (parse_args (argc, argv, ior_output_file) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Usage: Service [-o IOR_file_name]\n")));
          return 1;
        }

      CORBA::Object_var const object =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const poa =
        PortableServer::POA::_narrow (object.in ());
      PortableServer::POAManager_var const poa_manager =
        poa->the_POAManager ();
      poa_manager->activate ();

      TAO_CEC_EventChannel_Attributes attributes (poa.in (),
                                                  poa.in ());

      TAO_CEC_EventChannel ec_impl (attributes);
      ec_impl.activate ();

      CosEventChannelAdmin::EventChannel_var event_channel =
        ec_impl._this ();

      CORBA::String_var const ior =
        orb->object_to_string (event_channel.in ());

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Activated as <%C>\n"), ior.in ()));

      std::unique_ptr<FILE, int (*) (FILE *)> const output_file (
        ACE_OS::fopen (ior_output_file, ACE_TEXT ("w")), ACE_OS::fclose);
      if (!output_file)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                           ior_output_file),
                          1);
      ACE_OS::fprintf (output_file.get (), "%s", ior.in ());

      // Wait for events, using work_pending()/perform_work() may help
      // or using another thread, this example is too simple for that.
      orb->run ();

      // We don't do any cleanup, it is hard to do it after shutdown,
      // and would complicate the example; plus it is almost
      // impossible to do cleanup after ORB->run() because the POA is
      // in the holding state.  Applications should use
      // work_pending()/perform_work() to do more interesting stuff.
      // Check the supplier for the proper way to do cleanup.
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (ACE_TEXT ("Service"));
      return 1;
    }
  return 0;
}
