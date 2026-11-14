#include "Client_Task.h"
#include "ace/Get_Opt.h"

struct Client_Options
{
  ACE_TCHAR const *ior {ACE_TEXT ("file://test.ior")};
  bool server_shutdown {};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:x"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;
      case 'x':
        options.server_shutdown = true;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -k <ior> [-x]\n"),
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
      CORBA::ORB_var const orb (CORBA::ORB_init (argc, argv));
      Client_Options options;

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const tmp (orb->string_to_object (options.ior));

      Test::Big_Reply_var const reply_gen (Test::Big_Reply::_narrow (tmp.in ()));

      if (CORBA::is_nil (reply_gen.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             ACE_TEXT ("Nil coordinator reference <%s>\n"),
                             options.ior),
                            1);
        }

      Client_Task client_task (reply_gen.in (),
                               10,
                               ACE_Thread_Manager::instance ());

      if (client_task.activate (THR_NEW_LWP | THR_JOINABLE, 4, 1) == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error activating client task\n"));
        }
      ACE_Thread_Manager::instance ()->wait ();

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Got all the replies...\n")));

      if (options.server_shutdown)
        {
          // Request the server to shutdown
          reply_gen->shutdown ();
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
