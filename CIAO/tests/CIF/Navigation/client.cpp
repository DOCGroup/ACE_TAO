// $Id$

#include "Common/CIF_Common.h"

//============================================================
// test_provide_facet
//============================================================
int
test_provide_facet (::Components::Navigation_ptr nav)
{
  ACE_DEBUG ((LM_DEBUG, "Navigation test_provide_facet - "
                        "Start test\n"));

  try
    {
      ::CORBA::Object_var prov_cif_foo =
        nav->provide_facet ("provide_cif_foo");
      if (::CORBA::is_nil (prov_cif_foo.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Navigation test_provide_facet - "
                                "Error: provide_facet with provide_cif_foo "
                                "returned a NIL pointer\n"));
          return 1;
        }
      ::CORBA::Object_var prov_cif_der_foo =
        nav->provide_facet ("provide_cif_derived_foo");
      if (::CORBA::is_nil (prov_cif_der_foo.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Navigation test_provide_facet - "
                                "Error: provide_facet with provide_cif_derived_foo "
                                "returned a NIL pointer\n"));
          return 1;
        }
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

  ACE_DEBUG ((LM_DEBUG, "Navigation test_provide_facet (InvalidName) - "
                        "Start test\n"));
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
      nav->provide_facet ("derived_foo_1");
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
  ACE_DEBUG ((LM_DEBUG, "Navigation test_get_all_facets - "
                        "Start test\n"));

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
  ACE_DEBUG ((LM_DEBUG, "Navigation test_get_named_facets - "
                        "Start test\n"));
  try
    {
      ::Components::NameList_var names;
      names->length (2);
      names[0] = CORBA::string_dup ("provide_cif_foo");
      names[1] = CORBA::string_dup ("provide_cif_derived_foo");
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
      ::Components::NameList_var names;
      names->length (2);
      names[0] = CORBA::string_dup ("provide_cif_foo_invalid_name");
      names[1] = CORBA::string_dup ("provide_cif_derived_foo_invalid_name");
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
  ACE_DEBUG ((LM_DEBUG, "Navigation test_same_component - "
                        "Start test\n"));

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

  ACE_DEBUG ((LM_DEBUG, "Navigation test_same_component (Exception) - "
                        "Start test\n"));
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
//============================================================
// test_get_all_ports
//============================================================
int
test_get_all_ports (::Components::CCMObject_ptr cmp)
{
  ACE_DEBUG ((LM_DEBUG, "Navigation test_get_all_ports - "
                        "Start test\n"));
  int ret = 0;
  try
    {
      ::Components::ComponentPortDescription * cpd;
      cpd = cmp->get_all_ports ();

      //only the facets are available.
      ::Components::FacetDescriptions fds;
      fds = cpd->facets ();
      if (fds.length () != 2)
        {
          ACE_ERROR ((LM_ERROR, "Navigation test_get_all_ports - "
                                "Error: Did not receive the expected "
                                "number of facets: "
                                "expected <2> - received <%d>\n",
                                fds.length ()));
          return 1;
        }
      for (::CORBA::ULong i = 0UL; i < fds.length (); ++i)
        {
          if (::ACE_OS::strcmp (fds[i]->name (), "provide_cif_foo") == 0 ||
              ::ACE_OS::strcmp (fds[i]->name (), "provide_cif_derived_foo") == 0)
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
          ++ret;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "Navigation get_all_ports - "
                                "Expected number of Receptacles found\n"));
        }
#if !defined (CCM_NOEVENT)
      ::Components::ConsumerDescriptions cds;
      cds = cpd->consumers ();
      if (cds.length () != 0)
        {
          ACE_ERROR ((LM_ERROR, "Navigation get_all_ports - "
                                "Error: Found Consumers while not  "
                                "configured\n"));
          ++ret;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "Navigation get_all_ports - "
                                "Expected number of Consumers found\n"));
        }

      ::Components::EmitterDescriptions eds;
      eds = cpd->emitters ();
      if (eds.length () != 1)
        {
          ACE_ERROR ((LM_ERROR, "Navigation get_all_ports - "
                                "Error: Unexpected number of emitters found: "
                                "expected <1> - received <%d>\n",
                                eds.length ()));
          ++ret;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "Navigation get_all_ports - "
                                "Expected number of Emitters found\n"));
        }

      ::Components::PublisherDescriptions pds;
      pds = cpd->publishers ();
      if (pds.length () != 2)
        {
          ACE_ERROR ((LM_ERROR, "Navigation get_all_ports - "
                                "Error: Unexpected number publishers found:  "
                                "expected <2> - received <%d>\n",
                                pds.length ()));
          ++ret;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "Navigation get_all_ports - "
                                "Expected number of Publishers found\n"));
        }
#endif
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
  if (ret == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Navigation test_get_all_ports - "
                            "Test passed\n"));
    }
  return ret;
}
#endif

#if !defined (CCM_LW)
//============================================================
// test_get_all_publishers
//============================================================
int
test_get_all_publishers (::Components::CCMObject_ptr cmp)
{
  ACE_DEBUG ((LM_DEBUG, "Navigation test_get_all_publishers - "
                        "Start test\n"));
  try
    {
      ::Components::PublisherDescriptions_var pds;
      pds = cmp->get_all_publishers ();
      if (pds->length () != 2)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Navigation test_get_all_publishers - "
                            "Error: get_all_publishers returned an "
                            "unexpected number of publishers: "
                            "expected <2> - received <%d>\n",
                            pds->length ()),
                            1);
        }
    }
  catch (const ::CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Navigation test_get_all_publishers");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Navigation test_get_all_publishers - "
                            "Error: Unexpected exception caught.\n"));
      return 1;
    }
  ACE_DEBUG ((LM_DEBUG, "Navigation test_get_all_publishers - "
                        "Test passed\n"));
  return 0;
}
#endif

#if !defined (CCM_LW)
//============================================================
// test_get_named_publishers
//============================================================
int
test_get_named_publishers (::Components::CCMObject_ptr cmp)
{
  ACE_DEBUG ((LM_DEBUG, "Navigation test_get_named_publishers - "
                        "Start test\n"));

  ::Components::PublisherDescriptions_var pds;
  try
    {
      ::Components::NameList_var two_names;
      two_names->length (2);
      two_names[0] = ::CORBA::string_dup ("publish_do_something");
      two_names[1] = ::CORBA::string_dup ("publish_do_something_else");
      pds = cmp->get_named_publishers (two_names);
      if (pds->length () != 2)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Navigation test_get_named_publishers - "
                            "Error: get_named_publishers returned an "
                            "unexpected number of publishers: "
                            "expected <2> - received <%d>\n",
                            pds->length ()),
                            1);
        }
    }
  catch (const ::CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Navigation test_get_named_publishers");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Navigation test_get_named_publishers - "
                            "Error: Unexpected exception caught.\n"));
      return 1;
    }
  ACE_DEBUG ((LM_DEBUG, "Navigation test_get_named_publishers - "
                        "Test passed\n"));

  // Test InvalidName exception
  try
    {
      ::Components::NameList_var invalid_names;
      invalid_names->length (3);
      invalid_names[0] = ::CORBA::string_dup ("publish_do_something");
      invalid_names[1] = ::CORBA::string_dup ("publish_do_something_invalid_name");
      invalid_names[2] = ::CORBA::string_dup ("publish_do_something_else");
      pds = cmp->get_named_publishers (invalid_names);
      ACE_ERROR_RETURN ((LM_ERROR,
                        "Navigation test_get_named_publishers - "
                        "Error: No InvalidName exception received\n"),
                        1);
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_DEBUG ((LM_DEBUG, "Navigation test_get_named_publishers - "
                            "Received expected InvalidName "
                            "exception during connect\n"));
    }
  catch (const ::CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Navigation test_get_named_publishers");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Navigation test_get_named_publishers - "
                            "Error: Unexpected exception caught.\n"));
      return 1;
    }
  ACE_DEBUG ((LM_DEBUG, "Navigation test_get_named_publishers - "
                        "InvalidName test passed!\n"));
  return 0;
}
#endif

#if !defined (CCM_LW)
int
//============================================================
// test_get_all_emitters
//============================================================
test_get_all_emitters (::Components::CCMObject_ptr cmp)
{
  ACE_DEBUG ((LM_DEBUG, "Navigation test_get_all_emitters - "
                        "Start test\n"));
  try
    {
      ::Components::EmitterDescriptions_var eds;
      eds = cmp->get_all_emitters ();
      if (eds->length () != 1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Navigation test_get_all_emitters - "
                            "Error: get_all_emitters returned an "
                            "unexpected number of emitters: "
                            "expected <1> - received <%d>\n",
                            eds->length ()),
                            1);
        }

    }
  catch (const ::CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Navigation test_get_all_emitters");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Navigation test_get_all_emitters - "
                            "Error: Unexpected exception caught.\n"));
      return 1;
    }
  ACE_DEBUG ((LM_DEBUG, "Navigation test_get_all_emitters - "
                        "Test passed\n"));
  return 0;
}
#endif
#if !defined (CCM_LW)
int
//============================================================
// test_get_named_emitters
//============================================================
test_get_named_emitters (::Components::CCMObject_ptr cmp)
{
  ACE_DEBUG ((LM_DEBUG, "Navigation test_get_named_emitters - "
                        "Start test\n"));
  ::Components::EmitterDescriptions_var eds;

  try
    {

      ::Components::NameList_var one_name;
      one_name.length (1);
      one_name[0] = ::CORBA::string_dup ("emit_do_something");
      eds = cmp->get_named_emitters (one_name);
      if (eds->length () != 1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Navigation test_get_named_emitters - "
                            "Error: get_named_emitters returned an "
                            "unexpected number of emitters: "
                            "expected <1> - received <%d>\n",
                            eds->length ()),
                            1);
        }
    }
  catch (const ::CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Navigation test_get_named_emitters");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Navigation test_get_named_emitters - "
                            "Error: Unexpected exception caught.\n"));
      return 1;
    }
  ACE_DEBUG ((LM_DEBUG, "Navigation test_get_named_emitters - "
                        "Test passed\n"));

  // Test InvalidName exception
  try
    {
      ::Components::NameList_var invalid_names;
      invalid_names->length (2);
      invalid_names[0] = ::CORBA::string_dup ("emit_do_something");
      invalid_names[1] = ::CORBA::string_dup ("emit_do_something_invalid_name");
      eds = cmp->get_named_emitters (invalid_names);
      ACE_ERROR_RETURN ((LM_ERROR,
                        "Navigation test_get_named_emitters - "
                        "Error: No InvalidName exception received\n"),
                        1);
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_DEBUG ((LM_DEBUG, "Navigation test_get_named_emitters - "
                            "Received expected InvalidName "
                            "exception during connect\n"));
    }
  catch (const ::CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Navigation test_get_named_emitters");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Navigation test_get_named_emitters - "
                            "Error: Unexpected exception caught.\n"));
      return 1;
    }
  ACE_DEBUG ((LM_DEBUG, "Navigation test_get_named_emitters - "
                        "InvalidName test passed!\n"));
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

  ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
  ret += test_get_all_publishers (cmp);

  ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
  ret += test_get_named_publishers (cmp);

  ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
  ret += test_get_all_emitters (cmp);

  ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
  ret += test_get_named_emitters (cmp);
#else
  ACE_UNUSED_ARG (cmp);
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
      ::CORBA::Object_var prov = cmd.get_provider_cmp ();
      if (::CORBA::is_nil (prov.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Unable to get provider component\n"),
                            1);
        }

      ::Components::Navigation_var nav = cmd.get_navigation_interface ();
      if (::CORBA::is_nil (nav.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Unable to get navigation interface\n"),
                            1);
        }
      ::Components::CCMObject_var prov_cmp = ::Components::CCMObject::_narrow (prov);
      ret = run_test (nav.in (), prov_cmp.in ());

      ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
      cmd.test_provider_component ();
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
