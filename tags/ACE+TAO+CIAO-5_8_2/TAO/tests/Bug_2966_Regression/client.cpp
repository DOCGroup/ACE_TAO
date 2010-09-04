// -*- C++ -*-
// $Id$

#include "tao/DynamicInterface/Request.h"
#include "tao/DynamicInterface/DII_CORBA_methods.h"
#include "TestC.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var obj = orb->string_to_object(ior);

      const char* the_string = 0;

      ACE_DEBUG ((LM_INFO, "(%P|%t) - #### Synchronous twoway DII\n"));

      CORBA::Request_var req = obj->_request ("get_string");
      req->set_return_type (CORBA::_tc_string);

      req->invoke ();
      req->return_value () >>= the_string;

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
                  the_string));

      ACE_DEBUG ((LM_INFO, "(%P|%t) - #### Deferred twoway DII (using get_response())\n"));

      req = obj->_request ("get_string");
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
        ACE_OS::sleep(1);
        ACE_DEBUG ((LM_INFO, "(%P|%t) - Polling...\n"));
      } while (!req->poll_response ());
      req->return_value () >>= the_string;

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
                  the_string));

      ACE_DEBUG ((LM_INFO, "(%P|%t) - #### Oneway shutdown DII\n"));

      req = obj->_request ("shutdown");
      req->send_oneway ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
