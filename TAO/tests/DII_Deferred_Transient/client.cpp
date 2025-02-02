// -*- C++ -*-
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
  // Indicates successful parsing of the command line
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

      ACE_DEBUG ((LM_INFO, "(%P|%t) - #### Deferred twoway DII (using get_response())\n"));

      CORBA::Request_var req = obj->_request ("get_string");
      req->set_return_type (CORBA::_tc_string);

      req->send_deferred ();
      ACE_DEBUG ((LM_INFO, "(%P|%t) - Get response...\n"));
      req->get_response ();
      req->return_value () >>= the_string;

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
                  the_string));

      ACE_DEBUG ((LM_INFO, "(%P|%t) - #### Oneway DII to initiate crash\n"));

      req = obj->_request ("crash");

      req->send_oneway ();

      ACE_OS::sleep(1); // give it some time

      ACE_DEBUG ((LM_INFO, "(%P|%t) - #### Multiple deferred twoway DII (with discarded replies)\n"));

      int iterations = 10;
      int exceptions = 0;
      for (int i=0; i<iterations ;++i)
      {
        try
        {
          ACE_DEBUG ((LM_INFO, "(%P|%t) - get_string iteration #%d\n", i));

          req = obj->_request ("get_string");
          req->set_return_type (CORBA::_tc_string);

          req->send_deferred ();
          // discard reply

          ACE_DEBUG ((LM_INFO, "(%P|%t) - sent deferred\n"));
        }
        catch (const CORBA::Exception&)
        {
          ++exceptions;
        }
      }

      if (exceptions == 0)
      {
        ACE_DEBUG ((LM_ERROR, "(%P|%t) - All requests unexpectedly succeeded\n"));
      }
      else
      {
        ACE_DEBUG ((LM_INFO, "(%P|%t) - Caught exceptions from requests as expected (%d)\n", exceptions));
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
