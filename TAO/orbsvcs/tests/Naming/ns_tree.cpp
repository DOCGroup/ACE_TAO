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
//     
// ============================================================================

#include "tao/TAO.h"
#include "orbsvcs/CosNamingC.h"
#include "test_objectS.h"

class Test_Object_impl : public POA_Test_Object
{
public:
  Test_Object_impl (void) {};
  ~Test_Object_impl (void) {};
};

int
main (int argc, char **argv)
{
  TAO_ORB_Manager orbmgr;
  Test_Object_impl myObject;

  TAO_TRY
    {
      // Initialize and obtain reference to the Naming Context
      if (orbmgr.init (argc, argv, TAO_TRY_ENV) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "failed to init ORB\n"),
                           -1);
      TAO_CHECK_ENV;

      CORBA::ORB_var orb = orbmgr.orb ();
      CORBA::Object_var ns_obj =
        orb->resolve_initial_references ("NameService");

      if (CORBA::is_nil (ns_obj))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "failed to resolve Name Service\n"),
                           -1);

      CosNaming::NamingContext_var ns_ctx = 
	CosNaming::NamingContext::_narrow (ns_obj.in (),
                                           TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Dummy object instantiation.
      Test_Object_var myObject_var =
        myObject._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;


      // Create a child context.
      CosNaming::Name test_context (1);
      test_context.length (1);
      test_context[0].id =
        CORBA::string_dup ("MyContext");

      CORBA::Object_var mycontext =
	ns_ctx->bind_new_context (test_context,
                                  TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (CORBA::is_nil (mycontext))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "unabled to create new naming context \"MyContext\"\n"),
                           -1);
      
      CosNaming::NamingContext_var my_context = 
      	CosNaming::NamingContext::_narrow (mycontext.in (),
                                           TAO_TRY_ENV);
      TAO_CHECK_ENV;
      cerr << "Created new context OK" << endl;

      // bind an object to a child context     
      CosNaming::Name test_name (2);
      test_name.length (2);
      test_name[0].id = CORBA::string_dup ("MyContext");
      test_name[1].id = CORBA::string_dup ("MyName");

      ns_ctx->bind (test_name,
                    myObject_var.in (),
                    TAO_TRY_ENV);
      TAO_CHECK_ENV;      
      cerr << "Bound compound name OK" << endl;

      // Finally, try now to resolve the compound name.
      CosNaming::Name result_name (2);
      result_name.length (2);
      result_name[0].id = CORBA::string_dup ("MyContext");
      result_name[1].id = CORBA::string_dup ("MyName");

      CORBA::Object_var resolvedobj = ns_ctx->resolve (result_name,
                                                       TAO_TRY_ENV);
      TAO_CHECK_ENV;

      Test_Object_var resultObject = 
	Test_Object::_narrow (resolvedobj.in (), 
                              TAO_TRY_ENV);        

      cerr << "Resolved compound name OK" << endl;      
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("ns_tree");
      return -1;
    }
  TAO_ENDTRY;
  return 0;
}
