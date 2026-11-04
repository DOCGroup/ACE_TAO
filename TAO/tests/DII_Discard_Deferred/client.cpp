// -*- C++ -*-
#include "tao/DynamicInterface/Request.h"
#include "tao/DynamicInterface/DII_CORBA_methods.h"
#include "TestC.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "ace/SString.h"

struct Client_Options
{
  ACE_TString ior {ACE_TEXT ("file://test.ior")};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -k <ior>\n"),
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
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const obj =
        orb->string_to_object (options.ior.c_str ());

      // The string is borrowed from the request's return Any, which owns it.
      char const *the_string {};

      ACE_DEBUG ((LM_INFO, "(%P|%t) - #### Deferred twoway DII (using get_response())\n"));

      CORBA::Request_var req = obj->_request ("get_string");
      req->set_return_type (CORBA::_tc_string);

      req->send_deferred ();
      ACE_DEBUG ((LM_INFO, "(%P|%t) - Get response...\n"));
      req->get_response ();
      req->return_value () >>= the_string;

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
                  the_string));
      ACE_DEBUG ((LM_INFO, "(%P|%t) - #### Deferred twoway DII (using poll_response())\n"));

      req = obj->_request ("get_string");
      req->set_return_type (CORBA::_tc_string);

      req->send_deferred ();
      do {
        ACE_OS::sleep (1);
        ACE_DEBUG ((LM_INFO, "(%P|%t) - Polling...\n"));
      } while (!req->poll_response ());
      req->return_value () >>= the_string;

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
                  the_string));

      ACE_DEBUG ((LM_INFO, "(%P|%t) - #### Deferred twoway DII with discarded request\n"));

      req = obj->_request ("get_string");
      req->set_return_type (CORBA::_tc_string);

      req->send_deferred ();
      ACE_DEBUG ((LM_INFO, "(%P|%t) - Discarding request and waiting 5 seconds\n"));

      // Simulate other activity that exercises dynamic memory management.
      req = CORBA::Request::_nil ();
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - discarded\n"));
      int constexpr iterations = 50;
      for (int i {}; i < iterations; ++i)
      {
        ACE_Time_Value timeout (0, 100000);
        orb->perform_work (timeout);
        ACE_OS::sleep (timeout); // Sleep the rest of the interval, if any.
      }
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - continue\n"));


      ACE_DEBUG ((LM_INFO, "(%P|%t) - #### Oneway shutdown DII\n"));

      req = obj->_request ("shutdown");
      req->send_oneway ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
