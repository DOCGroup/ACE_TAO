// $Id$

// ============================================================================
//
// = FILENAME
//    ns_tree.cpp
//
// = DESCRIPTION
//    Tests various features of TAO's Naming Service.  Note that this
//    uses a co-located Naming Service, rather than a remote one. 
//
// = AUTHOR
//	Ross Lillie <lillie@rsch.comm.mot.com>
//	LMPS Systems Research Lab
//     
// ============================================================================

#include "tao/TAO.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Naming/Naming_Utils.h"
#include "test_objectS.h"

ACE_RCSID(Naming, ns_tree, "$Id$")

class My_Test_Object : public POA_Test_Object
{
public:
  My_Test_Object (void) {};
  ~My_Test_Object (void) {};
};

int
main (int argc, char **argv)
{
  TAO_ORB_Manager orbmgr;
  My_Test_Object myObject;
  TAO_Naming_Server my_name_server;

  TAO_TRY
    {
      // Initialize and obtain reference to the Naming Context.
      if (orbmgr.init_child_poa (argc,
                                 argv,
                                 "child",
                                 TAO_TRY_ENV) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "failed to init ORB\n"),
                          -1);
      TAO_CHECK_ENV;

      PortableServer::POA_var child_poa;
      CORBA::ORB_var orb = orbmgr.orb ();

      child_poa = orbmgr.child_poa ();
      int result = my_name_server.init (orb,
                                        child_poa);

      if (result == -1)
        return result;

      CosNaming::Name context_name;
      context_name.length (1);
      context_name[0].id =
        CORBA::string_dup ("NameService");
      CosNaming::NamingContext_var ns_ctx = 
        my_name_server->bind_new_context (context_name,
                                          TAO_TRY_ENV);

      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      // Dummy object instantiation.
      Test_Object_var myObject_var =
        myObject._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Create a child context.
      CosNaming::Name test_context (1);
      test_context.length (1);
      test_context[0].id =
        CORBA::string_dup ("MyContext");

      CosNaming::NamingContext_var my_context = 
        my_name_server->bind_new_context (test_context,
                                          TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      ACE_DEBUG ((LM_DEBUG,
                  "Created new context OK"));

      // Bind an object to a child context.
      CosNaming::Name test_name (2);
      test_name.length (2);
      test_name[0].id =
        CORBA::string_dup ("MyContext");
      test_name[1].id =
        CORBA::string_dup ("MyName");

      my_name_server->bind (test_name,
			    myObject_var.in (),
			    TAO_TRY_ENV);
      TAO_CHECK_ENV;      
      ACE_DEBUG ((LM_DEBUG,
                  "Bound compound name OK"));

      // Finally, try now to resolve the compound name.
      CosNaming::Name result_name (2);
      result_name.length (2);
      result_name[0].id =
        CORBA::string_dup ("MyContext");
      result_name[1].id =
        CORBA::string_dup ("MyName");

      CORBA::Object_var resolvedobj =
        my_name_server->resolve (result_name,
                                 TAO_TRY_ENV);
      TAO_CHECK_ENV;

      Test_Object_var resultObject = 
    	Test_Object::_narrow (resolvedobj.in (), 
                              TAO_TRY_ENV);        
      TAO_CHECK_ENV;
      ACE_DEBUG ((LM_DEBUG,
                  "Resolved compound name OK"));
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("ns_tree");
      return -1;
    }
  TAO_ENDTRY;
  return 0;
}
