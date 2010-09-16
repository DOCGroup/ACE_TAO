// $Id$

#include "Common/CIF_Common.h"

//============================================================
// test_provide_facet
//============================================================
int
test_provide_facet (::Components::Navigation_ptr nav)
{
  ACE_DEBUG ((LM_DEBUG, "Start test: test_provide_facet\n"));

  try
    {
      nav->provide_facet ("provide_cif_foo");
      nav->provide_facet ("provide_cif_inherited_foo");
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_ERROR ((LM_ERROR, "Navigation test_provide_facet - "
                            "Error: Unexpected InvalidName exception caught "
                            "while testing provide_facet\n"));
      return 1;
    }
  catch (const ::CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Navigation test_provide_facet");
      return 1;
    }
  ACE_DEBUG ((LM_DEBUG, "Navigation test_provide_facet - "
                        "Provide facet test passed !\n"));

  ACE_DEBUG ((LM_DEBUG, "Start test: Navigation test_provide_facet - "
                        "InvalidName exception test\n"));
  try
    {
      nav->provide_facet ("navigation_foo_1");
      ACE_ERROR ((LM_ERROR, "Navigation test_provide_facet - "
                            "Error: No InvalidName exception caught "
                            "while testing provide_facet\n"));
      return 1;
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_DEBUG ((LM_DEBUG, "Navigation test_provide_facet - "
                            "Expected InvalidName exception caught.\n"));
    }
  catch (const ::CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Navigation test_provide_facet");
      return 1;
    }

  try
    {
      nav->provide_facet ("inherited_foo_1");
      ACE_ERROR ((LM_ERROR, "Navigation test_provide_facet - "
                            "Error: No InvalidName exception caught "
                            "while testing provide_facet\n"));
      return 1;
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_DEBUG ((LM_DEBUG, "Navigation test_provide_facet - "
                            "Expected InvalidName exception caught.\n"));
    }
  catch (const ::CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Navigation test_provide_facet");
      return 1;
    }
  ACE_DEBUG ((LM_DEBUG, "Navigation test_provide_facet - "
                        "InvalidName Exception test passed !\n"));
  return 0;
}

//============================================================
// test_get_all_facets
//============================================================
#if !defined (CCM_LW)
int
test_get_all_facets (::Components::Navigation_ptr nav)
{
  ACE_DEBUG ((LM_DEBUG, "Start test: Navigation get_all_facets\n"));

  try
    {
      ::Components::FacetDescriptions_var all_facets = nav->get_all_facets ();
      if (all_facets->length () != 2)
        {
          ACE_ERROR ((LM_ERROR, "Navigation test_get_all_facets - "
                                "Error: unexpected number of descriptions: "
                                "expected <%u> - received <%u>\n",
                                2, all_facets->length ()));
          return 1;
        }
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception ("Navigation test_get_all_facets");
      return 1;
    }

  ACE_DEBUG ((LM_DEBUG, "Navigation test_get_all_facets - "
                        "get_all_facets test passed !\n"));
  return 0;
}
#endif

//============================================================
// test_get_named_facets
//============================================================
#if !defined (CCM_LW)
int
test_get_named_facets (::Components::Navigation_ptr nav)
{
  ACE_DEBUG ((LM_DEBUG, "Start test: Navigation test_get_named_facets\n"));
  try
    {
      ::Components::NameList names;
      names.length (2);
      names[0] = CORBA::string_dup ("provide_cif_foo");
      names[1] = CORBA::string_dup ("provide_cif_inherited_foo");
      ::Components::FacetDescriptions_var named_facets = nav->get_named_facets (names);

      if (named_facets->length () != 2)
        {
          ACE_ERROR ((LM_ERROR, "Navigation get_named_facets - "
                                "Error: unexpected number of descriptions: "
                                "expected <%u> - received <%u>\n",
                                2, named_facets->length ()));
          return 1;
        }
    }
  catch (const ::Components::InvalidName &e)
    {
      ACE_ERROR ((LM_ERROR, "Navigation test_get_named_facets - "
                            "Error: Unexpected InvalidName exception caught "
                            "while testing get_named_facets\n"));
      return 1;
    }
  catch (const ::CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Navigation test_get_named_facets");
      return 1;
    }
  ACE_DEBUG ((LM_DEBUG, "Navigation test_get_named_facets - "
                        "get_named_facets test passed !\n"));

  ACE_DEBUG ((LM_DEBUG, "Navigation test_get_named_facets - "
                        "InvalidName exception\n"));
  try
    {
      ::Components::NameList names;
      names.length (2);
      names[0] = CORBA::string_dup ("provide_cif_foo_invalid_name");
      names[1] = CORBA::string_dup ("provide_cif_inherited_foo_invalid_name");
      ::Components::FacetDescriptions_var named_facets = nav->get_named_facets (names);
      ACE_ERROR ((LM_ERROR, "Navigation test_get_named_facets - "
                            "Error: No InvalidName exception caught "
                            "while testing get_named_facets\n"));
      return 1;
    }
  catch (const ::Components::InvalidName &e)
    {
      ACE_DEBUG ((LM_DEBUG, "Navigation test_get_named_facets - "
                            "Expected InvalidName exception caught "
                            "while testing get_named_facets\n"));
    }
  catch (const ::CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Navigation test_get_named_facets");
      return 1;
    }
  ACE_DEBUG ((LM_DEBUG, "Navigation test_get_named_facets - "
                        "InvalidName Exception test passed !\n"));
  return 0;
}
#endif

#if !defined (CCM_LW)
//============================================================
// test_same_component
//============================================================
int
test_same_component (::Components::Navigation_ptr nav)
{
  ACE_DEBUG ((LM_DEBUG, "Start test:  test_same_component\n"));

  if (nav->same_component (nav))
    {
      ACE_DEBUG ((LM_DEBUG, "Navigation test_same_component - "
                            "Same component test passed !\n"));
    }
  else
    {
      ACE_ERROR ((LM_ERROR, "Navigation test_same_component - "
                            "Error: Same component test failed!\n"));
      return 1;
    }
  ACE_DEBUG ((LM_DEBUG, "Navigation test_same_component - "
                        "Same component test passed!\n"));

  ACE_DEBUG ((LM_DEBUG, "Start test: Navigation test_same_component - "
                        "Exception test.\n"));
  try
    {
      nav->same_component (0);
    }
  catch (const ::CORBA::Exception& ex)
    {
      ACE_DEBUG ((LM_DEBUG, "Navigation test_same_component - "
                            "Expected exception received.\n"));
    }
  ACE_DEBUG ((LM_DEBUG, "Navigation test_same_component - "
                        "Same component Exception test passed!\n"));
  return 0;
}
#endif

#if !defined (CCM_LW)
int
test_get_all_ports (::Components::CCMObject_ptr cmp)
{
  ACE_DEBUG ((LM_DEBUG, "Navigation - test_get_all_ports - "
                        "Start test\n"));
  try
    {
      ::Components::ComponentPortDescription * cpd;
      cpd = cmp->get_all_ports ();

      //only the facets are available.
      ::Components::FacetDescriptions fds;
      fds = cpd->facets ();
      if (fds.length () != 2)
        {
          ACE_ERROR ((LM_ERROR, "Navigation - test_get_all_ports - "
                                "Error: Did not receive the expected "
                                "number of facets: "
                                "expected <2> - received <%d>\n",
                                fds.length ()));
          return 1;
        }
      for (::CORBA::ULong i = 0UL; i < fds.length (); ++i)
        {
          if (::ACE_OS::strcmp (fds[i]->name (), "provide_cif_foo") == 0 ||
              ::ACE_OS::strcmp (fds[i]->name (), "provide_cif_inherited_foo") == 0)
            {
              ACE_DEBUG ((LM_DEBUG, "Navigation get_all_ports - "
                                    "Correct facetdescription found <%C>\n",
                                    fds[i]->name ()));
            }
          else
            {
              ACE_ERROR ((LM_ERROR, "Navigation get_all_ports - "
                                    "Error Incorrect facetdescription found <%C>\n",
                                    fds[i]->name ()));
            }
        }

      ::Components::ReceptacleDescriptions rds;
      rds = cpd->receptacles ();
      if (rds.length () != 0)
        {
          ACE_ERROR ((LM_ERROR, "Navigation get_all_ports - "
                                "Error: Found receptacles an a Navigation "
                                "Component\n"));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "Navigation get_all_ports - "
                                "Expected number of Receptacles found\n"));
        }

      ::Components::ConsumerDescriptions cds;
      cds = cpd->consumers ();
      if (cds.length () != 0)
        {
          ACE_ERROR ((LM_ERROR, "Navigation get_all_ports - "
                                "Error: Found Consumers while not  "
                                "configured\n"));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "Navigation get_all_ports - "
                                "Expected number of Consumers found\n"));
        }

      ::Components::EmitterDescriptions eds;
      eds = cpd->emitters ();
      if (eds.length () != 0)
        {
          ACE_ERROR ((LM_ERROR, "Navigation get_all_ports - "
                                "Error: Found Emitters while not  "
                                "configured\n"));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "Navigation get_all_ports - "
                                "Expected number of Emitters found\n"));
        }

      ::Components::PublisherDescriptions pds;
      pds = cpd->publishers ();
      if (pds.length () != 0)
        {
          ACE_ERROR ((LM_ERROR, "Navigation get_all_ports - "
                                "Error: Found Publishers while not  "
                                "configured\n"));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "Navigation get_all_ports - "
                                "Expected number of Publishers found\n"));
        }
    }
  catch (const ::CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Navigation get_all_ports");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Navigation get_all_ports - "
                            "Error: Unexpected exception caught.\n"));
      return 1;
    }
  ACE_DEBUG ((LM_DEBUG, "Navigation - test_get_all_ports - "
                        "Test passed\n"));
  return 0;
}
#endif

int
run_test (::Components::Navigation_ptr nav,
          ::Components::CCMObject_ptr cmp)
{
  ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
  int ret = test_provide_facet (nav);

#if !defined (CCM_LW)
   ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
   ret += test_get_all_facets (nav);

   ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
   ret += test_get_named_facets (nav);

  ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
  ret += test_same_component (nav);


  ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
  ret += test_get_all_ports (cmp);
#endif
  return ret;
}

int
ACE_TMAIN (int argc,  ACE_TCHAR **argv)
{
  CIF_Common cmd;
  int ret = 0;
  try
    {
      if (cmd.init (argc, argv) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Error: Unable to initalize\n"),
                            1);

        }
      ::CORBA::Object_var obj = cmd.get_provider_cmp ();
      if (::CORBA::is_nil (obj.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Unable to get navigation component\n"),
                            1);
        }

      ::Components::Navigation_var nav = cmd.get_navigation_interface ();
      if (::CORBA::is_nil (nav.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Unable to get navigation interface\n"),
                            1);
        }
      ::Components::CCMObject_var cmp = ::Components::CCMObject::_narrow (obj);
      ret = run_test (nav.in (), cmp.in ());

      cmd.shutdown ();
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception ("Navigation main");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                        "Error: Caught unknown exception\n"),
                        1);
    }

  ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
  ACE_DEBUG ((LM_DEBUG, "SUMMARY : \n"));
  if (ret != 0)
    {
      ACE_ERROR ((LM_ERROR, "\tNavigation main - "
              " %d error(s) found during tests.\n",
              ret));
    }
  else
    {
      ACE_ERROR ((LM_ERROR, "\tNavigation main - "
              " No problems found during tests.\n"));
    }
  return ret;
}
