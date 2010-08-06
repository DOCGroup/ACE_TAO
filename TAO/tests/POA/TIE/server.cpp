// $Id$

// =========================================================================
// = LIBRARY
//    TAO/tests/POA/TIE
//
// = FILENAME
//    server.cpp
//
// = DESCRIPTION
//    In this example,
//
// = AUTHOR
//    Irfan Pyarali
// =========================================================================

#include "ace/streams.h"
#include "ace/SString.h"
#include "test_i.h"
#include "ace/OS_NS_stdio.h"

ACE_RCSID(TIE, server, "$Id$")

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{

  char str[256];

  try
    {
      ACE_OS::strcpy (str, "CORBA::ORB_init");

      // Initialize the ORB first.
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      // Obtain the RootPOA.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      ACE_OS::strcpy (str, "PortableServer::POA::_narrow");

      // Get the POA_var object from Object_var.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      ACE_OS::strcpy (str, "PortableServer::POA::the_POAManager");
      // Get the POAManager of the RootPOA.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();


      // Policies for the firstPOA to be created.
      CORBA::PolicyList policies (2);
      policies.length (2);

      ACE_OS::strcpy (str,"PortableServer::POA::create_lifespan_policy");
      // Lifespan policy
      policies[0] =
        root_poa->create_lifespan_policy (PortableServer::PERSISTENT);

      ACE_OS::strcpy (str, "PortableServer::POA::create_implicit_activation_policy");
      // Implicit activation policy
      policies[1] =
        root_poa->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION);

      ACE_OS::strcpy (str, "PortableServer::POA::create_POA");
      // Create the firstPOA under the RootPOA.
      ACE_CString name = "firstPOA";
      PortableServer::POA_var first_poa =
        root_poa->create_POA (name.c_str (),
                              poa_manager.in (),
                              policies);


      ACE_OS::strcpy (str,"PortableServer::POA::create_POA");

      // Creation of POAs is over. Destroy the Policy objects.
      for (CORBA::ULong i = 0;
           i < policies.length ();
           ++i)
        {
          CORBA::Policy_ptr policy = policies[i];
          policy->destroy ();
        }

      // Create A_i
      A_i a_impl (27, first_poa.in ());

      // Create B_i
      Outer_i::B_i b_impl (28, first_poa.in ());

      // Create C_i
      Outer_i::Inner_i::C_i c_impl (29, first_poa.in ());

#if defined (ACE_HAS_USING_KEYWORD)
      // Create A tie
      Tie_i a_tie_i (30);
      POA_A_tie <Tie_i> a_tie_impl (a_tie_i, first_poa.in ());

      // Create B tie
      Tie_i b_tie_i (31);
      POA_Outer::B_tie <Tie_i> b_tie_impl (b_tie_i, first_poa.in ());

      // Create C tie
      Tie_i c_tie_i (32);
      POA_Outer::Inner::C_tie <Tie_i> c_tie_impl (c_tie_i, first_poa.in ());

#endif /* ACE_HAS_USING_KEYWORD */

      ACE_OS::strcpy (str, "POA_A::_this");
      // Get Object Reference for the a_impl object.
      A_var a = a_impl._this ();


      ACE_OS::strcpy (str, "POA_Outer::B::_this");
      // Get Object Reference for the b_impl object.
      Outer::B_var b = b_impl._this ();

      ACE_OS::strcpy (str, "POA_Outer::Inner::C::_this");
      // Get Object Reference for the c_impl object.
      Outer::Inner::C_var c = c_impl._this ();

#if defined (ACE_HAS_USING_KEYWORD)
      ACE_OS::strcpy (str, "POA_A::_this");
      // Get Object Reference for the a_tie_impl object.
      A_var a_tie = a_tie_impl._this ();

      ACE_OS::strcpy (str, "POA_Outer::B::_this");
      // Get Object Reference for the a_tie_impl object.
      Outer::B_var b_tie = b_tie_impl._this ();

      ACE_OS::strcpy (str, "POA_Outer::C::_this");
      // Get Object Reference for the c_tie_impl object.
      Outer::Inner::C_var c_tie = c_tie_impl._this ();

#endif /* ACE_HAS_USING_KEYWORD */

      ACE_OS::strcpy (str, "CORBA::ORB::object_to_string");
      // Stringyfy all the object references and print them out.
      CORBA::String_var first_ior =
        orb->object_to_string (a.in ());

      // Stringyfy all the object references and print them out.
      CORBA::String_var second_ior =
        orb->object_to_string (b.in ());

      // Stringyfy all the object references and print them out.
      CORBA::String_var third_ior =
        orb->object_to_string (c.in ());

#if defined (ACE_HAS_USING_KEYWORD)
      // Stringyfy all the object references and print them out.
      CORBA::String_var forth_ior =
        orb->object_to_string (a_tie.in ());

      FILE *output_file_1 = ACE_OS::fopen ("ior_1", "w");
      FILE *output_file_2 = ACE_OS::fopen ("ior_2", "w");
      FILE *output_file_3 = ACE_OS::fopen ("ior_3", "w");
      FILE *output_file_4 = ACE_OS::fopen ("ior_4", "w");

      if (output_file_1)
        ACE_OS::fprintf (output_file_1,
                         "%s",
                         first_ior.in ());
      if (output_file_2)
        ACE_OS::fprintf (output_file_2,
                         "%s",
                         second_ior.in ());
      if (output_file_3)
        ACE_OS::fprintf (output_file_3,
                         "%s",
                         third_ior.in ());
      if (output_file_4)
        ACE_OS::fprintf (output_file_4,
                         "%s",
                         forth_ior.in ());

      ACE_OS::fclose (output_file_1);
      ACE_OS::fclose (output_file_2);
      ACE_OS::fclose (output_file_3);
      ACE_OS::fclose (output_file_4);


      // Stringyfy all the object references and print them out.
      CORBA::String_var fifth_ior =
        orb->object_to_string (b_tie.in ());


      // Stringyfy all the object references and print them out.
      CORBA::String_var sixth_ior =
        orb->object_to_string (c_tie.in ());

      FILE *output_file_5 = ACE_OS::fopen ("ior_5", "w");
      FILE *output_file_6 = ACE_OS::fopen ("ior_6", "w");

      if (output_file_5)
        ACE_OS::fprintf (output_file_5,
                         "%s",
                         fifth_ior.in ());
      if (output_file_6)
        ACE_OS::fprintf (output_file_6,
                         "%s",
                         sixth_ior.in ());

      ACE_OS::fclose (output_file_5);
      ACE_OS::fclose (output_file_6);

#endif /* ACE_HAS_USING_KEYWORD */

      poa_manager->activate ();

      orb->run ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (str);
      return -1;
    }

  return 0;
}

