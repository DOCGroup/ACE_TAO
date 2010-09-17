// $Id$

#include "testC.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"

const ACE_TCHAR *ior = ACE_TEXT("file://gateway.ior");
int niterations = 5;
int do_shutdown = 0;
int test_user_exception = 0;
int test_system_exception = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("xusk:i:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'x':
        do_shutdown = 1;
        break;

      case 'u':
        test_user_exception = 1;
        break;

      case 's':
        test_system_exception = 1;
        break;

      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'i':
        niterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-x "
                           "-u "
                           "-s "
                           "-k <ior> "
                           "-i <niterations> "
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
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        {
          return 1;
        }

      CORBA::Object_var object =
        orb->string_to_object (ior);

      Simple_Server_var server =
        Simple_Server::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             ior),
                            1);
        }

      Structure the_in_structure;
      the_in_structure.seq.length (10);

      if (test_user_exception == 1)
        {
          server->raise_user_exception ();
        }
      else if (test_system_exception == 1)
        {
          server->raise_system_exception ();
        }
      else
        {
          for (int i = 0; i != niterations; ++i)
            {
              CORBA::Long const tv = i + 100;
              server->test_val(tv);
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

             Structure_var the_out_structure;

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

      if (do_shutdown)
        {
          server->shutdown ();
        }
    }
  catch (const test_exception& ex)
    {
      if (test_user_exception == 1)
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

      return test_user_exception == 1 ? 0 : 1;
    }
  catch (const CORBA::NO_PERMISSION& ex)
    {
      if (test_system_exception == 1)
        ACE_DEBUG ((LM_DEBUG,
                    "Client: caught expected system exception: %C\n",
                    ex._name()));
      else
        ex._tao_print_exception ("Client: exception caught - ");

      return test_system_exception == 1 ? 0 : 1;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Client: exception caught - ");
      return 1;
    }

  return 0;
}
