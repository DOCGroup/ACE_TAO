// $Id$

// ============================================================================
//
// = FILENAME
//    ns_tree.cpp
//
// = DESCRIPTION
//
// = AUTHOR
//	Ross Lillie <lillie@rsch.comm.mot.com>
//	LMPS Systems Research Lab
//
// ============================================================================

#include "tao/TAO.h"
#include "orbsvcs/CosNamingC.h"
#include "test_objectS.h"

class test_object_impl : public POA_test_object
{
public:
  test_object_impl () {};
  ~test_object_impl () {};
};

int
main (int argc, char **argv)
{
  TAO_ORB_Manager orbmgr;
  test_object_impl myObject;

  TAO_TRY
    {
      orbmgr.init (argc, argv, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::ORB_var orb = orbmgr.orb ();

      CORBA::Object_var ns_obj = orb->resolve_initial_references ("NameService");

      if (CORBA::is_nil (ns_obj))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "failed to resolve Name Service\n"),
                           -1);

      CosNaming::NamingContext_var ns_ctx = 
	CosNaming::NamingContext::_narrow (ns_obj.in (),
                                           TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Dummy object instantiation.
      test_object_var myObject_var = myObject._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

#if !defined (HIERARCHICAL_BIND)
      CosNaming::Name test_context (1);
      test_context.length (1);
      test_context[0].id = CORBA::string_dup ("MyContext");

      CORBA::Object_var myobj =
	ns_ctx->bind_new_context (test_context,
                                  TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (CORBA::is_nil (myobj))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "unabled to resolve naming context \"MyContext\"\n"),
                           -1);
      
      CosNaming::NamingContext_var my_context = 
      	CosNaming::NamingContext::_narrow (myobj.in (),
                                           TAO_TRY_ENV);
      TAO_CHECK_ENV;
#endif /* !HIERARCHICAL_BIND */
#if defined (RECURSIVE_BIND)
      CosNaming::Name test_name (2);
      test_name.length (1);
      test_name[0].id = CORBA::string_dup ("MyContext");
      test_name[1].id = CORBA::string_dup ("MyName");

      ns_ctx->bind (test_name,
                    myObject_var.in (),
                    TAO_TRY_ENV);
      TAO_CHECK_ENV;

#else
#if defined (HIERARCHICAL_BIND)
      CosNaming::Name test_context (1);
      test_context.length (1);
      test_context[0].id = CORBA::string_dup ("MyContext");

      CORBA::Object_var myobj = ns_ctx->resolve (test_context,
                                                 TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      if (CORBA::is_nil (myobj))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "unabled to resolve naming context \"MyContext\"\n"),
                           -1);
      CosNaming::NamingContext_var my_context = 
      	CosNaming::NamingContext::_narrow (myobj.in (),
                                           TAO_TRY_ENV);
      TAO_CHECK_ENV;
#endif /* HIERARCHICAL_BIND */
      CosNaming::Name test_name (1);
      test_name.length (1);
      test_name[0].id = CORBA::string_dup ("MyName");
      
      my_context->bind (test_name,
                   myObject_var.in (),
                   TAO_TRY_ENV);
      TAO_CHECK_ENV;
#endif /* RECURSIVE_BIND */

      // Finally, try now to resolve the compound name.
      CosNaming::Name result_name (2);
      result_name.length (2);
      result_name[0].id = CORBA::string_dup ("MyContext");
      result_name[1].id = CORBA::string_dup ("MyName");

      CORBA::Object_var resolvedobj = ns_ctx->resolve (result_name,
                                                       TAO_TRY_ENV);
      TAO_CHECK_ENV;

      test_object_var resultObject = 
	test_object::_narrow (resolvedobj.in (), 
                              TAO_TRY_ENV);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("ns_tree");
      return -1;
    }
  TAO_ENDTRY;
  return 0;
}
