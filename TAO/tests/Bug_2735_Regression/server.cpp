// $Id$

#include "tao/ORB.h"
#include "ace/Get_Opt.h"
#include "ace/Service_Config.h"
#include "ace/Log_Msg.h"

static const ACE_TCHAR* server_strat =
  ACE_STATIC_SERVICE_DIRECTIVE ("Server_Strategy_Factory",
                                "-ORBConcurrency thread-per-connection");
static const ACE_TCHAR* foo_strat =
  ACE_STATIC_SERVICE_DIRECTIVE ("Foo_Strategy_Factory", "");

enum Factory_Strategy { SERVER_STRATEGY, FOO_STRATEGY };

static Factory_Strategy factory_strategy = SERVER_STRATEGY;

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("fs"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'f':
        factory_strategy = FOO_STRATEGY;
        break;
      case 's':
        factory_strategy = SERVER_STRATEGY;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage: %s "
                           "-f "
                           "-s "
                           "\n",
                           argv [0]),
                          -1);
      }
  return 0;
}


int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      if (parse_args (argc, argv) == -1)
        return -1;

      int targc = 3;
      const ACE_TCHAR *targv[] =
        { ACE_TEXT ("server"), ACE_TEXT ("-ORBSvcConfDirective"), 0, 0 }; // null-terminated!
      switch (factory_strategy)
        {
        case FOO_STRATEGY:
          targv[2] = foo_strat;
          break;
        case SERVER_STRATEGY:
          targv[2] = server_strat;
          break;
        }
      CORBA::ORB_var orb = CORBA::ORB_init (targc, const_cast<ACE_TCHAR**> (targv));
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
