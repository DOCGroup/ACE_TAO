// $Id$

// ============================================================================
//
// = LIBRARY
//
// = FILENAME
//    svr.cpp
//
// = AUTHOR
//   Michael Kircher
//
// ============================================================================

#include "ace/Get_Opt.h"
#include "tao/corba.h"
#include "any_test_i.h"
//#include "Node.h"

ACE_RCSID(DOVEMIB, svr, "$Id$")

// Global Variables
static int num_of_objs = 1;

// Parses the command line arguments and returns an error status

static int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-n] <num of cubit objects>"
                           "\n", argv [0]), 1);
      }

  return 0;  // Indicates successful parsing of command line
}

// Standard command line parsing utilities used.

int
main (int argc, char *argv[])
{
  /*
  CORBA::Long long_ = 4;
  CORBA::Double double_ = 5.0;
  StructNode *structNode_ptr_ = new StructNode ("ABC");
  
  structNode_ptr_->addChild ((Node *)new DoubleNode (&double_, "My_Double"));
  structNode_ptr_->addChild ((Node *)new LongNode (&long_, "My_Long"));

  structNode_ptr_->print ();
  */
  
  TAO_TRY
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, 0, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "Having the ORB\n"));

      // Initialize the Object Adapter
      CORBA::Object_var poa_object = 
	orb->resolve_initial_references("RootPOA");
      if (CORBA::is_nil(poa_object.in()))
	ACE_ERROR_RETURN ((LM_ERROR,
			   " (%P|%t) Unable to initialize the POA.\n"),
			  1);

      PortableServer::POA_var root_poa =
	PortableServer::POA::_narrow (poa_object.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      PortableServer::POAManager_var poa_manager =
	root_poa->the_POAManager (TAO_TRY_ENV);
      TAO_CHECK_ENV;
  
      CORBA::PolicyList policies (2);
      policies.length (2);  
      policies[0] =
	root_poa->create_id_assignment_policy (PortableServer::USER_ID,
					       TAO_TRY_ENV);
      policies[1] =
	root_poa->create_lifespan_policy (PortableServer::PERSISTENT,
					  TAO_TRY_ENV);

      ACE_DEBUG ((LM_DEBUG, "Having the rootPOA\n"));

      // We use a different POA, otherwise the user would have to
      // change the object key each time it invokes the server.
      PortableServer::POA_var good_poa =
	root_poa->create_POA ("child_poa",
			      poa_manager.in (),
			      policies,
			      TAO_TRY_ENV);  
      TAO_CHECK_ENV;

      // Parse remaining command line and verify parameters.
      parse_args (argc, argv);

      ACE_DEBUG ((LM_DEBUG, "Starting the implementation\n"));

      // create a factory implementation
      Any_Test_i_ptr any_test_i_ptr_ = new Any_Test_i ();
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "Started the implementation\n"));

      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId ("Any_Test");
      good_poa->activate_object_with_id (id.in (),
                                         any_test_i_ptr_,
                                         TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Stringify the objref we'll be implementing, and print it to
      // stdout.  Someone will take that string and give it to a
      // client.  Then release the object.
      CORBA::Object_var obj =
        good_poa->id_to_reference (id.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::String_var str =
        orb->object_to_string (obj.in (),
                               TAO_TRY_ENV);

      ACE_DEBUG ((LM_DEBUG,
                  "The IOR is: <%s>\n", str.in ()));


      poa_manager->activate (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Handle requests for this object until we're killed, or one of
      // the methods asks us to exit.
      if (orb->run () == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "run"), -1);

      root_poa->destroy (1, 
			 1, 
			 TAO_TRY_ENV);
      TAO_CHECK_ENV
    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      ACE_UNUSED_ARG(sysex);
      TAO_TRY_ENV.print_exception ("System Exception");
      return -1;
    }
  TAO_CATCH (CORBA::UserException, userex)
    {
      ACE_UNUSED_ARG(userex);
      TAO_TRY_ENV.print_exception ("User Exception");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}
