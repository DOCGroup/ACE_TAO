// $Id$

#include "LB_server.h"
#include "Basic.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"

const ACE_TCHAR *group_file = ACE_TEXT("group.ior");
const ACE_TCHAR *group_ior = ACE_TEXT("file://group.ior");

const ACE_TCHAR *direct_obj_file = ACE_TEXT("direct_obj2.ior");

int server_id = 1;
int pong_delay = 0;


int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:k:n:j:l:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        group_file = get_opts.opt_arg ();
        break;
      case 'k':
        group_ior = get_opts.opt_arg ();
        break;
      case 'n':
        server_id = ACE_OS::atoi(get_opts.opt_arg());
        break;
      case 'j':
        direct_obj_file = get_opts.opt_arg ();
        break;
      case 'l':
        pong_delay = ACE_OS::atoi(get_opts.opt_arg());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-n <server_id> -o <group_ior_file> -k <group_ior> "
                           "-j <direct_obj_file> -l <pong_delay>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}


int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      const char *location1 = "MyLocation 1";
      const char *location2 = "MyLocation 2";
      const char *location3 = "MyLocation 3";
      const char *location4 = "MyLocation 4";
      const char *location5 = "MyLocation 5";
      const char *location6 = "MyLocation 6";

      LB_server lb_server (argc, argv);

      if (lb_server.start_orb_and_poa () == -1)
        return 1;

      if (parse_args (argc, argv) == -1)
        return 1;

      if (server_id == 1 && lb_server.create_object_group () == -1)
        return 1;
      else if (lb_server.get_object_group () == -1)
        return 1;

      const char ** location = 0;

      switch (server_id)
      {
        case 1:
          location = &location1;
          break;
        case 2:
          location = &location2;
          break;
        case 3:
          location = &location3;
          break;
        case 4:
          location = &location4;
          break;
        case 5:
          location = &location5;
          break;
        case 6:
          location = &location6;
          break;
      }

      Basic *basic_servant;

      ACE_NEW_RETURN (basic_servant,
                      Basic (lb_server.object_group (),
                             lb_server.load_manager (),
                             lb_server.orb (),
                             *location,
                             server_id),
                      1);
      PortableServer::ServantBase_var owner_transfer(basic_servant);

      if (lb_server.register_servant (basic_servant, *location) == -1)
        {
          (void) lb_server.destroy ();
          return 1;
        }

      if (server_id == 2)
        {
          Basic *direct_basic_servant = 0;

          ACE_NEW_RETURN (direct_basic_servant,
                          Basic (server_id),
                          1);

          PortableServer::ServantBase_var owner_transfer(direct_basic_servant);

          Test::Basic_var direct_basic =
            direct_basic_servant->_this ();

          CORBA::String_var ior =
            lb_server.orb ()->object_to_string (direct_basic.in ());

          FILE *output_file =
            ACE_OS::fopen (direct_obj_file, "w");

          if (output_file == 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "Cannot open output file(%s) for writing IOR:", direct_obj_file));
              if (lb_server.destroy () == -1)
                return 1;
            }

          ACE_OS::fprintf (output_file, "%s", ior.in());
          ACE_OS::fclose (output_file);
        }

      //  ACE_OS::sleep (1000);
      lb_server.orb ()->run ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t)%T server - event loop finished\n"));

      if (lb_server.destroy () == -1)
        return 1;

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("lb_server exception");
      return 1;
    }

  return 0;
}
