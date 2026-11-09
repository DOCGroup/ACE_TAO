#include "ace/Get_Opt.h"
#include "Test_i.h"
#include "ace/OS_NS_stdio.h"
#include "orbsvcs/Shutdown_Utilities.h"
#include <memory>

struct Server_Options
{
  ACE_TCHAR const *ior_output_file {};
};

class Service_Shutdown_Functor : public Shutdown_Functor
{
public:
  explicit Service_Shutdown_Functor (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
  {
  }

  Service_Shutdown_Functor (Service_Shutdown_Functor const&) = delete;
  Service_Shutdown_Functor (Service_Shutdown_Functor&&) = delete;
  Service_Shutdown_Functor& operator= (Service_Shutdown_Functor const&) = delete;
  Service_Shutdown_Functor& operator= (Service_Shutdown_Functor&&) = delete;

  void operator() (int which_signal) override
  {
    ACE_DEBUG ((LM_DEBUG,
                "shutting down on signal %d\n", which_signal));
    (void) this->orb_->shutdown ();
  }

private:
  CORBA::ORB_var orb_ {};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
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
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      Service_Shutdown_Functor killer (orb.in ());
      Service_Shutdown kill_contractor (killer);

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "SERVER (%P): Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      Server_Options options;
      if (parse_args (argc, argv, options) != 0)
        return 1;

      Test_i server_impl (orb.in ());

      PortableServer::ObjectId_var const tmp =
        root_poa->activate_object (&server_impl);

      CORBA::Object_var const server = server_impl._this ();

      CORBA::String_var const ior =
        orb->object_to_string (server.in ());

      // If the ior_output_file exists, output the ior to it
      if (options.ior_output_file != nullptr)
        {
          std::unique_ptr<FILE, int (*) (FILE *)> output_file (
            ACE_OS::fopen (options.ior_output_file, "w"), ACE_OS::fclose);
          if (!output_file)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "SERVER (%P): Cannot open output file "
                               "for writing IOR: %s",
                               ACE_TEXT_ALWAYS_CHAR (options.ior_output_file)),
                              1);
          ACE_OS::fprintf (output_file.get (), "%s", ior.in ());

          ACE_DEBUG ((LM_DEBUG,
                      "SERVER (%P): Activated as file://%s\n",
                      ACE_TEXT_ALWAYS_CHAR (options.ior_output_file)));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "SERVER (%P): Activated as <%s>\n",
                      ior.in ()));
        }

      poa_manager->activate ();

      orb->run ();

      root_poa->destroy (true, true);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("SERVER (%P): Caught exception:");
      return 1;
    }

  return 0;
}
