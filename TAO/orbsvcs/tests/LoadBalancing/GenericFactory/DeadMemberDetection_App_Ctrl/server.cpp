#include "LB_server.h"
#include "Basic.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include <array>
#include <cstddef>
#include <memory>
#include <string>

static int
parse_args (int argc, ACE_TCHAR *argv[], LB_Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:k:n:j:l:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.group_file = get_opts.opt_arg ();
        break;
      case 'k':
        options.group_ior = get_opts.opt_arg ();
        break;
      case 'n':
        options.server_id = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'j':
        options.direct_obj_file = get_opts.opt_arg ();
        break;
      case 'l':
        options.pong_delay = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -n <server_id> -o <group_ior_file> "
                                     "-k <group_ior> -j <direct_obj_file> "
                                     "-l <pong_delay>\n"),
                           argv[0]),
                          -1);
      }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  LB_Server_Options options {};
  LB_server lb_server (argc, argv, options);

  try
    {
      if (lb_server.start_orb_and_poa () == -1)
        {
          (void) lb_server.destroy ();
          return 1;
        }

      if (parse_args (argc, argv, options) == -1)
        {
          (void) lb_server.destroy ();
          return 1;
        }

      if (options.server_id < 1 || options.server_id > 6)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Invalid server id %d; expected a value from 1 to 6\n"),
                      options.server_id));
          (void) lb_server.destroy ();
          return 1;
        }

      if (options.server_id == 1)
        {
          if (lb_server.create_object_group () == -1)
            {
              (void) lb_server.destroy ();
              return 1;
            }
        }
      else if (lb_server.get_object_group () == -1)
        {
          (void) lb_server.destroy ();
          return 1;
        }

      std::array<std::string, 6> const locations {
        "MyLocation 1", "MyLocation 2", "MyLocation 3",
        "MyLocation 4", "MyLocation 5", "MyLocation 6"
      };
      std::string const &location =
        locations[static_cast<size_t> (options.server_id - 1)];

      Basic *basic_servant {};
      ACE_NEW_RETURN (basic_servant,
                      Basic (lb_server.object_group (),
                             lb_server.load_manager (),
                             lb_server.orb (),
                             location,
                             options.server_id,
                             options.pong_delay),
                      1);
      // ServantBase_var owns this servant; keep the alias borrowed below.
      PortableServer::ServantBase_var basic_owner (basic_servant);

      if (lb_server.register_servant (basic_servant, location) == -1)
        {
          (void) lb_server.destroy ();
          return 1;
        }

      PortableServer::ServantBase_var direct_basic_owner;
      if (options.server_id == 2)
        {
          Basic *direct_basic_servant {};
          ACE_NEW_RETURN (direct_basic_servant, Basic (options.server_id), 1);
          direct_basic_owner = direct_basic_servant;

          Test::Basic_var direct_basic = direct_basic_servant->_this ();
          CORBA::String_var ior =
            lb_server.orb ()->object_to_string (direct_basic.in ());

          using File_Ptr = std::unique_ptr<FILE, int (*) (FILE *)>;
          File_Ptr output_file (ACE_OS::fopen (options.direct_obj_file, "w"),
                                ACE_OS::fclose);
          if (!output_file)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("Cannot open output file %s for writing IOR\n"),
                          options.direct_obj_file));
              (void) lb_server.destroy ();
              return 1;
            }

          if (ACE_OS::fprintf (output_file.get (), "%s", ior.in ()) < 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("Cannot write IOR to output file %s\n"),
                          options.direct_obj_file));
              (void) lb_server.destroy ();
              return 1;
            }
        }

      lb_server.orb ()->run ();
      ACE_DEBUG ((LM_DEBUG, "(%P|%t)%T server - event loop finished\n"));

      if (lb_server.destroy () == -1)
        return 1;
    }
  catch (const CORBA::Exception &ex)
    {
      ex._tao_print_exception ("lb_server exception");
      (void) lb_server.destroy ();
      return 1;
    }

  return 0;
}
