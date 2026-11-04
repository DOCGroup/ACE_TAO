#include "tao/DynamicInterface/Request.h"  /* This must come first for
                                              G++ 3.4 or better */
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/Log_Msg.h"
#include "DII_ReplyHandler.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/PortableServer/POAC.h"
#include "tao/PortableServer/POAManagerC.h"

#include <cstddef>

struct Client_Options
{
  ACE_TString ior {ACE_TEXT ("file://server.ior")};
  bool do_shutdown {};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("xk:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'x':
        options.do_shutdown = true;
        break;
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -x -k <ior>\n"),
                           argv[0]),
                          -1);
      }

  return 0;
}

void
do_primary_test (CORBA::Object_var const &server,
                 Messaging::ReplyHandler_var const &callback,
                 ACE_TCHAR const *ior)
{
  if (CORBA::is_nil (server.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Object reference <%s> is nil.\n"),
                  ior));
      return;
    }

  try
    {
      CORBA::Request_var const req = server->_request ("do_forward");
      ACE_DEBUG ((LM_DEBUG, "Client sending test string\n"));
      CORBA::String_var const test_string = CORBA::string_dup ("123 look at me");
      req->add_in_arg ("text") <<=
        CORBA::Any::from_string (test_string.in (), 30);
      req->sendc (callback.in ());
    }
  catch (CORBA::Exception const &ex)
    {
      ACE_ERROR ((LM_ERROR, "Client caught exception: %C\n", ex._name ()));
    }
}

int
do_shutdown_test (CORBA::Object_var const &server)
{
  ACE_DEBUG ((LM_DEBUG, "[client] invoking shutdown on the server\n"));
  try
    {
      CORBA::Request_var const req = server->_request ("shutdown");
      req->invoke ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Client: exception caught during shutdown - ");
      return 1;
    }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int result {};
  Client_Options options;

  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      if (parse_args (argc, argv, options) != 0)
        return 1;

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Client using ior source %s\n"),
                  options.ior.c_str ()));
      CORBA::Object_var const server =
        orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (options.ior.c_str ()));

      CORBA::Object_var const obj =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const root =
        PortableServer::POA::_narrow (obj.in ());
      PortableServer::POAManager_var const pm = root->the_POAManager ();
      pm->activate ();

      bool got_reply {};
      Messaging::ReplyHandler_var callback {};
      ACE_NEW_THROW_EX (callback, DII_ReplyHandler (got_reply), CORBA::NO_MEMORY ());
      do_primary_test (server, callback, options.ior.c_str ());

      for (std::size_t i = 0; i < 100 && !got_reply; ++i)
        {
          ACE_Time_Value timeout (0, 10000);
          orb->perform_work (timeout);
        }

      if (options.do_shutdown)
        result = do_shutdown_test (server);

      ACE_DEBUG ((LM_DEBUG, "Shutting down and destroying ORB.\n"));
      orb->destroy ();
      ACE_DEBUG ((LM_DEBUG, "ORB destroyed\n"));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("ERROR: unexpected CORBA exception caught:");
      ++result;
    }
  return result;
}
