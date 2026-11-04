#include "tao/DynamicInterface/Request.h"
#include "TestC.h"
#include "tao/ORB.h"
#include "ace/Get_Opt.h"

struct Client_Options
{
  ACE_TString ior {ACE_TEXT ("file://test.ior")};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
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
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Client_Options options;
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const tmp = orb->string_to_object (
        ACE_TEXT_ALWAYS_CHAR (options.ior.c_str ()));

      Test::Hello_var const hello =
        Test::Hello::_narrow (tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference\n"),
                            1);
        }

      CORBA::Request_var const request = tmp->_request ("get_helloworld");
      request->set_return_type (Test::_tc_HelloWorld);

      CORBA::Any any;
      any <<= "Hi From DOC Group";
      request->arguments ()->add_value ("msg", any, CORBA::ARG_IN);

      request->invoke ();

      CORBA::NamedValue_ptr const results = request->result ();
      CORBA::Any * const obj_any = results->value ();

      CORBA::Object_var obj2 {};
      *obj_any >>= CORBA::Any::to_object (obj2);
      if (CORBA::is_nil (obj2.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Any_To_Object reference\n"),
                            1);
        }

      Test::HelloWorld_var const hello_world =
        Test::HelloWorld::_narrow (obj2.in ());

      if (CORBA::is_nil (hello_world.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::HelloWorld reference\n"),
                            1);
        }

      CORBA::String_var const the_string =
        hello_world->get_string ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
                  the_string.in ()));

      hello->shutdown ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
