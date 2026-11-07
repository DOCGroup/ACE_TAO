#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include <memory>
#include <string>

struct Server_Options
{
  std::basic_string<ACE_TCHAR> ready_file {ACE_TEXT ("ready")};
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
          options.ready_file = get_opts.opt_arg ();
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Usage: server [-o <ready_file>]\n")),
                            -1);
        }
    }
  return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Server_Options options;
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const object = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const poa = PortableServer::POA::_narrow (object.in ());
      PortableServer::POAManager_var const manager = poa->the_POAManager ();
      manager->activate ();

      {
        std::unique_ptr<FILE, int (*) (FILE *)> file (
          ACE_OS::fopen (options.ready_file.c_str (), ACE_TEXT ("w")), ACE_OS::fclose);
        if (!file)
          return 1;
        ACE_OS::fputs ("ready\n", file.get ());
      }

      ACE_Time_Value runtime (40);
      orb->run (runtime);
      poa->destroy (true, true);
      orb->destroy ();
      return 0;
    }
  catch (CORBA::Exception const &ex)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) Transport_Idle_Scanner: %C\n"),
                  ex._name ()));
      return 1;
    }
}
