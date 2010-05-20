// $Id$

#include "ace/Get_Opt.h"
#include "Common/CIF_Common.h"
#include "NavigationEC.h"

const char *cs_path = "ciao_componentserver";
CORBA::ULong spawn_delay = 30;

const char * artifact_name = "Navigation";

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("s:d:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 's':
        cs_path = ACE_TEXT_ALWAYS_CHAR (get_opts.opt_arg ());
        break;

      case 'd':
        spawn_delay = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage: %s "
                           "-s <path> "
                           "-d <uint> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
run_test (::Navigation_ptr nav)
{
  try
    {
      try
        {
          nav->provide_facet ("navigation_foo");
          nav->provide_facet ("inherited_foo");
        }
      catch (const ::Components::InvalidName &e)
        {
          ACE_ERROR ((LM_ERROR, "Error: Unexpected InvalidName exception caught "
                                "while testing provide_facet\n"));
        }
      ACE_DEBUG ((LM_DEBUG, "Provide facet test succeeded\n"));

      try
        {
          nav->provide_facet ("navigation_foo_1");
          ACE_ERROR ((LM_ERROR, "Error: No InvalidName exception caught "
                                "while testing provide_facet\n"));
        }
      catch (const ::Components::InvalidName &e)
        {
          ACE_DEBUG ((LM_DEBUG, "Expected InvalidName exception caught.\n"));
        }

      try
        {
          nav->provide_facet ("inherited_foo_1");
          ACE_ERROR ((LM_ERROR, "Error: No InvalidName exception caught "
                                "while testing provide_facet\n"));
        }
      catch (const ::Components::InvalidName &e)
        {
          ACE_DEBUG ((LM_DEBUG, "Expected InvalidName exception caught.\n"));
        }
      ACE_DEBUG ((LM_DEBUG, "Provide facet Exception test succeeded\n"));

      #if !defined (CCM_LW)
        ::Components::FacetDescriptions_var all_facets = nav->get_all_facets ();
        if (all_facets->length () != 2)
          {
            ACE_ERROR ((LM_ERROR, "Error: unexpected number of descriptions: "
                        "expected <%u> - received <%u>\n",
                        2, all_facets->length ()));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG, "get_all_facets test passed !\n"));
          }

        try
          {
            ::Components::NameList names;
            names.length (2);
            names[0] = CORBA::string_dup ("navigation_foo");
            names[1] = CORBA::string_dup ("inherited_foo");
            ::Components::FacetDescriptions_var named_facets = nav->get_named_facets (names);
            if (named_facets->length () != 2)
              {
                ACE_ERROR ((LM_ERROR, "Error: unexpected number of descriptions: "
                            "expected <%u> - received <%u>\n",
                            2, named_facets->length ()));
              }
            else
              {
                ACE_DEBUG ((LM_DEBUG, "get_named_facets test passed !\n"));
              }
          }
        catch (const ::Components::InvalidName &e)
          {
            ACE_ERROR ((LM_ERROR, "Error: Unexpected InvalidName exception caught "
                                  "while testing get_named_facets\n"));
          }

        try
          {
            ::Components::NameList names;
            names.length (2);
            names[0] = CORBA::string_dup ("navigation_foo_1");
            names[1] = CORBA::string_dup ("inherited_foo_1");
            ::Components::FacetDescriptions_var named_facets = nav->get_named_facets (names);
            ACE_ERROR ((LM_ERROR, "Error: No InvalidName exception caught "
                                  "while testing get_named_facets\n"));
          }
        catch (const ::Components::InvalidName &e)
          {
            ACE_DEBUG ((LM_DEBUG, "Expected InvalidName exception caught "
                                  "while testing get_named_facets\n"));
          }

//         boolean same_component (in Object object_ref);
      #endif
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Unexpected exception caught while running test.\n"));
      return 1;
    }
  return 0;
}

int
ACE_TMAIN (int argc,  ACE_TCHAR **argv)
{
  using namespace ::CIAO::Deployment;

  CIF_Common cmd;
  try
    {
      cmd.init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      ComponentServer_var server = cmd.create_componentserver (spawn_delay,
                                                               cs_path,
                                                               artifact_name);
      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Error: Got nil object reference from create_component_server "
                                "operation\n"));
          return 1;
        }

      Container_var cont = cmd.create_container (server.in ());
      if (CORBA::is_nil (cont.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Error: Got nil object reference from create_container "
                                "operation on server\n"));
          return 1;
        }

      Components::CCMObject_var comp = cmd.install_component (cont.in (),
                                                              artifact_name);
      if (CORBA::is_nil (comp.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Error: Installing component failed.\n"));
          return 1;
        }
      ::Navigation_var nav = ::Navigation::_narrow (comp.in ());

      if (CORBA::is_nil (nav.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Narrow failed from CCMObject to Navigation\n"));
          return 1;
        }

      run_test (nav.in ());

      cmd.shutdown (server.in (), cont.in (), comp.in ());
    }
  catch (const ::Components::CreateFailure &)
    {
      ACE_ERROR ((LM_ERROR, "Error: Caught CreateFailure exception.\n"));
      return  1;
    }
  catch (const ::Components::RemoveFailure &)
    {
      ACE_ERROR ((LM_ERROR, "Error: Caught RemoveFailure exception.\n"));
      return  1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Error: Caught unknown exception\n"));
      return  1;
    }
  return 0;
}
