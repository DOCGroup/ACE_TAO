// $Id$

#include "ace/Get_Opt.h"
#include "Test_i.h"
#include "ace/OS_NS_stdio.h"
#include "orbsvcs/Shutdown_Utilities.h"

const ACE_TCHAR *ior_output_file = 0;

class Service_Shutdown_Functor : public Shutdown_Functor
{
public:
  Service_Shutdown_Functor (CORBA::ORB_ptr orb)
    : orb_(CORBA::ORB::_duplicate (orb))
  {
  }

  void operator() (int which_signal)
  {
    ACE_DEBUG ((LM_DEBUG,
                "shutting down on signal %d\n", which_signal));
    (void) this->orb_->shutdown ();
  }

private:
  CORBA::ORB_var orb_;
};

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "SERVER (%P): usage:  %s "
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      Service_Shutdown_Functor killer (orb.in ());
      Service_Shutdown kill_contractor (killer);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "SERVER (%P): Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        return 1;

      Test_i server_impl (orb.in ());

      PortableServer::ObjectId_var tmp =
        root_poa->activate_object (&server_impl);


      CORBA::Object_var server = server_impl._this();

      CORBA::String_var ior =
        orb->object_to_string (server.in ());

      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != 0)
        {
          FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "SERVER (%P): Cannot open output file "
                               "for writing IOR: %s",
                               ior_output_file),
                              1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);

          ACE_DEBUG ((LM_DEBUG,
                      "SERVER (%P): Activated as file://%s\n",
                      ior_output_file));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "SERVER (%P): Activated as <%s>\n",
                      ior.in ()));
        }

      poa_manager->activate();

      orb->run ();

      root_poa->destroy (1, 1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("SERVER (%P): Caught exception:");
      return 1;
    }

  return 0;
}
