// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT("file://server.ior");
int client_num = 1;
int report_exception = 1;
bool is_shutdown = false;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("c:e:k:s"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'c':
        client_num = ACE_OS::atoi(get_opts.opt_arg ());
        break;
      case 'e':
        report_exception = ACE_OS::atoi(get_opts.opt_arg ());
        break;
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 's':
        is_shutdown = true;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-c <client #> "
                           "-e <0|1> "
                           "-k <ior> "
                           "-s (shutdown) "
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
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      Test::Hello_var hello =
        Test::Hello::_narrow(tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior),
                            1);
        }

      if (is_shutdown)
      {
        hello->shutdown();
      }
      else
      {
        CORBA::String_var the_string =
          hello->get_string (client_num);
        ACE_DEBUG((LM_DEBUG,
                   "Client %d received return text of: [%C %d].\n",
                   client_num,
                   the_string.in(),
                   client_num));
      }
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      if (report_exception)
        ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
