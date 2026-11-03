#include "tao/ORB.h"
#include "ace/Get_Opt.h"
#include "ace/Service_Config.h"
#include "ace/Log_Msg.h"

static ACE_TCHAR const *const server_strat =
  ACE_STATIC_SERVICE_DIRECTIVE ("Server_Strategy_Factory",
                                "-ORBConcurrency thread-per-connection");
static ACE_TCHAR const *const foo_strat =
  ACE_STATIC_SERVICE_DIRECTIVE ("Foo_Strategy_Factory", "");

enum class Factory_Strategy { server, foo };

static int
parse_args (int argc, ACE_TCHAR *argv[], Factory_Strategy &factory_strategy)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("fs"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'f':
        factory_strategy = Factory_Strategy::foo;
        break;
      case 's':
        factory_strategy = Factory_Strategy::server;
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
  Factory_Strategy factory_strategy {Factory_Strategy::server};
  try
    {
      if (parse_args (argc, argv, factory_strategy) == -1)
        return -1;

      int targc {3};
      ACE_TCHAR const *targv[] =
        { ACE_TEXT ("server"), ACE_TEXT ("-ORBSvcConfDirective"), nullptr, nullptr };
      switch (factory_strategy)
        {
        case Factory_Strategy::foo:
          targv[2] = foo_strat;
          break;
        case Factory_Strategy::server:
          targv[2] = server_strat;
          break;
        }
      CORBA::ORB_var const orb = CORBA::ORB_init (targc, const_cast<ACE_TCHAR **> (targv));
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
