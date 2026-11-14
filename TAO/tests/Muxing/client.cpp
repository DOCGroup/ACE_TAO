#include "Client_Task.h"
#include "ace/Get_Opt.h"
#include <string>

struct Options
{
  std::basic_string<ACE_TCHAR> ior {ACE_TEXT ("file://test.ior")};
  bool shutdown_server {};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:x"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;
      case 'x':
        options.shutdown_server = true;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "[-x]"
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
  Options options;
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const tmp =
        orb->string_to_object (options.ior.c_str ());

      Test::Receiver_var const receiver =
        Test::Receiver::_narrow (tmp.in ());

      if (CORBA::is_nil (receiver.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil coordinator reference <%s>\n",
                             options.ior.c_str ()),
                            1);
        }

      if (options.shutdown_server)
        {
          receiver->shutdown ();
        }
      else
        {
          Client_Task client_task (receiver.in (),
                                  1000,
                                  32768,
                                  ACE_Thread_Manager::instance ());

          if (client_task.activate (THR_NEW_LWP | THR_JOINABLE, 4, 1) == -1)
              {
              ACE_ERROR ((LM_ERROR, "Error activating client task\n"));
              }
          ACE_Thread_Manager::instance ()->wait ();

          CORBA::Long const count = receiver->get_event_count ();

          ACE_DEBUG ((LM_DEBUG, "(%P) - Receiver got %d messages\n",
                      count));
        }
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
