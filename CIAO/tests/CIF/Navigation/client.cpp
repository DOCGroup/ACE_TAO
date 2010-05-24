// $Id$

#include "Common/CIF_Common.h"
#include "NavigationEC.h"

const char * artifact_name = "Navigation";

int
run_test (::Navigation_ptr nav)
{
  int ret = 0;
  try
    {
      try
        {
          nav->provide_facet ("navigation_foo");
          nav->provide_facet ("inherited_foo");
        }
      catch (const ::Components::InvalidName &)
        {
          ACE_ERROR ((LM_ERROR, "Error: Unexpected InvalidName exception caught "
                                "while testing provide_facet\n"));
          ++ret;
        }
      ACE_DEBUG ((LM_DEBUG, "Provide facet test passed !\n"));

      try
        {
          nav->provide_facet ("navigation_foo_1");
          ACE_ERROR ((LM_ERROR, "Error: No InvalidName exception caught "
                                "while testing provide_facet\n"));
          ++ret;
        }
      catch (const ::Components::InvalidName &)
        {
          ACE_DEBUG ((LM_DEBUG, "Expected InvalidName exception caught.\n"));
        }

      try
        {
          nav->provide_facet ("inherited_foo_1");
          ACE_ERROR ((LM_ERROR, "Error: No InvalidName exception caught "
                                "while testing provide_facet\n"));
          ++ret;
        }
      catch (const ::Components::InvalidName &)
        {
          ACE_DEBUG ((LM_DEBUG, "Expected InvalidName exception caught.\n"));
        }

      #if !defined (CCM_LW)
        ::Components::FacetDescriptions_var all_facets = nav->get_all_facets ();
        if (all_facets->length () != 2)
          {
            ACE_ERROR ((LM_ERROR, "Error: unexpected number of descriptions: "
                        "expected <%u> - received <%u>\n",
                        2, all_facets->length ()));
            ++ret;
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
                ++ret;
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
            ++ret;
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
            ++ret;
        }
        catch (const ::Components::InvalidName &e)
          {
            ACE_DEBUG ((LM_DEBUG, "Expected InvalidName exception caught "
                                  "while testing get_named_facets\n"));
          }

        if (nav->same_component (nav))
          {
            ACE_DEBUG ((LM_DEBUG, "Same component test passed !\n"));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG, "Error: Same component test failed!\n"));
            ++ret;
          }
        try
          {
            nav->same_component (0);
          }
        catch (const CORBA::Exception& ex)
          {
            ex._tao_print_exception ("Expected exception caught:");
            ++ret;
          }
      #endif
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Unexpected exception caught while running test.\n"));
      ++ret;
    }
  return ret;
}

int
ACE_TMAIN (int argc,  ACE_TCHAR **argv)
{
  using namespace ::CIAO::Deployment;

  CIF_Common cmd;
  int ret = 0;
  try
    {
      if (cmd.init (argc, argv, artifact_name) != 0)
        {
          ACE_ERROR ((LM_ERROR, "ACE_TMAIN : Initialisation failed.\n"));
          return 1;
        }

      ComponentServer_var server = cmd.create_componentserver ();
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

      ret = run_test (nav.in ());

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
  if (ret != 0)
    {
      ACE_ERROR ((LM_ERROR, "ACE_TMAIN : "
              " %d error found during tests.\n",
              ret));
    }
  else
    {
      ACE_ERROR ((LM_ERROR, "ACE_TMAIN : "
              " No error found during tests.\n"));
    }
  return ret;
}
