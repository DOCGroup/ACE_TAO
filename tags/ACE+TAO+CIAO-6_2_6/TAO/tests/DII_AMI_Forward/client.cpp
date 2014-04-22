// -*- C++ -*-

/*
 * $Id$
 */

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

const ACE_TCHAR *ior = ACE_TEXT("file://server.ior");

int do_shutdown = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("xk:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'x':
        do_shutdown = 1;
        break;

      case 'k':
        ior = get_opts.optarg;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-x "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates successful parsing of the command line
  return 0;
}

void
do_primary_test (CORBA::Object_var &server,
                 Messaging::ReplyHandler_var &callback)
{
  try
    {

      CORBA::Request_var req = server->_request("do_forward");

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "Object reference <%s> is nil.\n",
                      ior));
          return;
        }

      ACE_DEBUG((LM_DEBUG,"Client sending test string\n"));
      CORBA::String_var test_string = CORBA::string_dup ("123 look at me");
      req->add_in_arg ("text") <<=
        CORBA::Any::from_string(test_string.in(),30);
      req->sendc(callback.in());
    }
  catch (CORBA::Exception &ex)
    {
      ACE_ERROR ((LM_ERROR,
                  "Client caught exception: %C\n",ex._name()));
    }
}

int do_shutdown_test (CORBA::Object_var &server)
{
  ACE_DEBUG ((LM_DEBUG,
              "[client] invoking shutdown on the server \n"));
  try
    {
      CORBA::Request_var req = server->_request("shutdown");
      req->invoke();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Client: exception caught during shutdown - ");
      return 1;
    }
  return 0;
}


int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int result = 0;

  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      ACE_DEBUG ((LM_DEBUG,"Client using ior source %s\n", ior));
      CORBA::Object_var server = orb->string_to_object (ior);

      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var root =
        PortableServer::POA::_narrow (obj.in());

      PortableServer::POAManager_var pm = root->the_POAManager();
      pm->activate();
      bool got_reply = false;
      Messaging::ReplyHandler_var callback = new DII_ReplyHandler(got_reply);

      do_primary_test (server,callback);

      for (int i = 0; i < 100 && !got_reply; i++)
        {
          ACE_Time_Value t(0,10000);
          orb->perform_work(t);
        }

      if (do_shutdown)
        result = do_shutdown_test (server);

      ACE_DEBUG ((LM_DEBUG,"Shutting down and destrying ORB.\n"));
      orb->destroy();
      ACE_DEBUG ((LM_DEBUG,"ORB destroyed\n"));
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception("ERROR : unexpected CORBA exception caugth :");
      ++result;
    }
  return result;
}
