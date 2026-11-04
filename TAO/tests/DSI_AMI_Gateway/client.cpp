#include "testC.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/SString.h"

struct Client_Options
{
  ACE_TString ior {ACE_TEXT ("file://gateway.ior")};
  int iterations {5};
  bool do_shutdown {};
  bool test_user_exception {};
  bool test_system_exception {};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("xusk:i:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'x':
        options.do_shutdown = true;
        break;

      case 'u':
        options.test_user_exception = true;
        break;

      case 's':
        options.test_system_exception = true;
        break;

      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case 'i':
        options.iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -x -u -s -k <ior> -i <niterations>\n"),
                           argv [0]),
                          -1);
      }

  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Client_Options options;
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        {
          return 1;
        }

      CORBA::Object_var const object =
        orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (options.ior.c_str ()));

      Simple_Server_var const server =
        Simple_Server::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Object reference <%C> is nil.\n"),
                             ACE_TEXT_ALWAYS_CHAR (options.ior.c_str ())),
                            1);
        }

      Structure the_in_structure {};
      the_in_structure.seq.length (10);

      if (options.test_user_exception)
        {
          server->raise_user_exception ();
        }
      else if (options.test_system_exception)
        {
          server->raise_system_exception ();
        }
      else
        {
          for (int i {}; i != options.iterations; ++i)
            {
              CORBA::Long const tv = i + 100;
              server->test_val (tv);
              CORBA::Long const rtv = server->test_val ();

              if (TAO_debug_level > 0)
                {
                   ACE_DEBUG ((LM_DEBUG,
                               "DSI_Simpler_Server ==== Expected result = %d for %d\n",
                               rtv, tv));
                }

              if (rtv != tv)
                {
                   ACE_ERROR ((LM_ERROR,
                             "(%P|%t) ERROR: unexpected result = %d for %d\n",
                             rtv, tv));
               }

             the_in_structure.i = i;
              CORBA::String_var name = CORBA::string_dup ("the name");

              Structure_var the_out_structure {};

             CORBA::Long const r =
                server->test_method (i,
                                     the_in_structure,
                                     the_out_structure.out (),
                                     name.inout ());

              if (TAO_debug_level > 0)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "DSI_Simpler_Server ====\n"
                              "    x = %d\n"
                              "    i = %d\n"
                              "    length = %d\n"
                              "    name = <%C>\n",
                              r,
                              the_out_structure->i,
                              the_out_structure->seq.length (),
                              name.in ()));
                }

              if (r != i)
                {
                  ACE_ERROR ((LM_ERROR,
                              "(%P|%t) ERROR: unexpected result = %d for %d",
                              r, i));
                }
            }
        }

      if (options.do_shutdown)
        {
          server->shutdown ();
        }
    }
  catch (test_exception const &ex)
    {
      if (options.test_user_exception)
        ACE_DEBUG ((LM_DEBUG,
                    "Client: caught expected user exception: %C\n",
                    ex._name()));
      else
        ex._tao_print_exception ("Client: exception caught - ");

      ACE_DEBUG ((LM_DEBUG,
                  "error code: %d\n"
                  "error info: %C\n"
                  "status: %C\n",
                  ex.error_code,
                  ex.error_message.in (),
                  ex.status_message.in ()));

      return options.test_user_exception ? 0 : 1;
    }
  catch (CORBA::NO_PERMISSION const &ex)
    {
      if (options.test_system_exception)
        ACE_DEBUG ((LM_DEBUG,
                    "Client: caught expected system exception: %C\n",
                    ex._name()));
      else
        ex._tao_print_exception ("Client: exception caught - ");

      return options.test_system_exception ? 0 : 1;
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Client: exception caught - ");
      return 1;
    }

  return 0;
}
