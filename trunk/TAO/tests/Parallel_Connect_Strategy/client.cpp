// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"

ACE_RCSID(Hello, client, "$Id$")

const char *ior = "file://test.ior";
int kill_server = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:x");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 'x':
        kill_server = 1;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-x "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      ACE_High_Res_Timer hrt;
      ACE_hrtime_t elapsed;

      ACE_DEBUG ((LM_DEBUG,"Narrowing IOR - "));
      hrt.start();

      Test::Hello_var hello =
        Test::Hello::_narrow(tmp.in ());
      hrt.stop();
      hrt.elapsed_microseconds (elapsed);
      hrt.reset();

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("call completed in %d usec\n"),
                  elapsed ));

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior),
                            1);
        }
      if (kill_server)
        {
          hello->shutdown ();
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,"Starting invocation 1 - "));
          hrt.start();
          CORBA::String_var the_string =
            hello->get_string ();
          hrt.stop();
          hrt.elapsed_microseconds (elapsed);
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("call completed in %d usec\n"),
                      elapsed ));
          ACE_DEBUG ((LM_DEBUG,"Starting invocation 2 - "));
          hrt.reset();
          hrt.start();
          the_string = hello->get_string ();
          hrt.stop();
          hrt.elapsed_microseconds (elapsed);
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("call completed in %d usec\n"),
                      elapsed ));
        }
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
