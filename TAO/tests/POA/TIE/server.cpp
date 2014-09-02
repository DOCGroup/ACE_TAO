
//=============================================================================
/**
 *  @file    server.cpp
 *
 *  $Id$
 *
 *  In this example,
 *
 *
 *  @author Irfan Pyarali
 */
//=============================================================================


#include "ace/streams.h"
#include "ace/SString.h"
#include "test_i.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior_1 = ACE_TEXT ("ior_1");
const ACE_TCHAR *ior_2 = ACE_TEXT ("ior_2");
const ACE_TCHAR *ior_3 = ACE_TEXT ("ior_3");
const ACE_TCHAR *ior_4 = ACE_TEXT ("ior_4");
const ACE_TCHAR *ior_5 = ACE_TEXT ("ior_5");
const ACE_TCHAR *ior_6 = ACE_TEXT ("ior_6");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("a:b:c:d:e:f:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'a':
          ior_1 = get_opts.opt_arg ();
          break;
      case 'b':
          ior_2 = get_opts.opt_arg ();
          break;
      case 'c':
          ior_3 = get_opts.opt_arg ();
          break;
      case 'd':
          ior_4 = get_opts.opt_arg ();
          break;
      case 'e':
          ior_5 = get_opts.opt_arg ();
          break;
      case 'f':
          ior_6 = get_opts.opt_arg ();
          break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-a <ior_1> "
                           "-b <ior_2> "
                           "-c <ior_3> "
                           "-d <ior_4> "
                           "-e <ior_5> "
                           "-f <ior_6> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{

  char str[256];

  try
    {
      ACE_OS::strcpy (str, "CORBA::ORB_init");

      // Initialize the ORB first.
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
          return 1;
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

      // Create A tie
      Tie_i a_tie_i (30);
      POA_A_tie <Tie_i> a_tie_impl (a_tie_i, first_poa.in ());

      // Create B tie
      Tie_i b_tie_i (31);
      POA_Outer::B_tie <Tie_i> b_tie_impl (b_tie_i, first_poa.in ());

      // Create C tie
      Tie_i c_tie_i (32);
      POA_Outer::Inner::C_tie <Tie_i> c_tie_impl (c_tie_i, first_poa.in ());

      ACE_OS::strcpy (str, "POA_A::_this");
      // Get Object Reference for the a_impl object.
      A_var a = a_impl._this ();

      ACE_OS::strcpy (str, "POA_Outer::B::_this");
      // Get Object Reference for the b_impl object.
      Outer::B_var b = b_impl._this ();

      ACE_OS::strcpy (str, "POA_Outer::Inner::C::_this");
      // Get Object Reference for the c_impl object.
      Outer::Inner::C_var c = c_impl._this ();

      ACE_OS::strcpy (str, "POA_A::_this");
      // Get Object Reference for the a_tie_impl object.
      A_var a_tie = a_tie_impl._this ();

      ACE_OS::strcpy (str, "POA_Outer::B::_this");
      // Get Object Reference for the a_tie_impl object.
      Outer::B_var b_tie = b_tie_impl._this ();

      ACE_OS::strcpy (str, "POA_Outer::C::_this");
      // Get Object Reference for the c_tie_impl object.
      Outer::Inner::C_var c_tie = c_tie_impl._this ();

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

      // Stringyfy all the object references and print them out.
      CORBA::String_var forth_ior =
        orb->object_to_string (a_tie.in ());

      FILE *output_file_1 = ACE_OS::fopen (ACE_TEXT_ALWAYS_CHAR(ior_1), "w");
      FILE *output_file_2 = ACE_OS::fopen (ACE_TEXT_ALWAYS_CHAR(ior_2), "w");
      FILE *output_file_3 = ACE_OS::fopen (ACE_TEXT_ALWAYS_CHAR(ior_3), "w");
      FILE *output_file_4 = ACE_OS::fopen (ACE_TEXT_ALWAYS_CHAR(ior_4), "w");

      if (output_file_1)
      {
        ACE_OS::fprintf (output_file_1,
                         "%s",
                         first_ior.in ());
        ACE_OS::fclose (output_file_1);
      }

      if (output_file_2)
      {
        ACE_OS::fprintf (output_file_2,
                         "%s",
                         second_ior.in ());
        ACE_OS::fclose (output_file_2);
      }

      if (output_file_3)
      {
        ACE_OS::fprintf (output_file_3,
                         "%s",
                         third_ior.in ());
        ACE_OS::fclose (output_file_3);
      }

      if (output_file_4)
      {
        ACE_OS::fprintf (output_file_4,
                         "%s",
                         forth_ior.in ());
        ACE_OS::fclose (output_file_4);
      }

      // Stringyfy all the object references and print them out.
      CORBA::String_var fifth_ior =
        orb->object_to_string (b_tie.in ());

      // Stringyfy all the object references and print them out.
      CORBA::String_var sixth_ior =
        orb->object_to_string (c_tie.in ());

      FILE *output_file_5 = ACE_OS::fopen (ACE_TEXT_ALWAYS_CHAR(ior_5), "w");
      FILE *output_file_6 = ACE_OS::fopen (ACE_TEXT_ALWAYS_CHAR(ior_6), "w");

      if (output_file_5)
      {
        ACE_OS::fprintf (output_file_5,
                         "%s",
                         fifth_ior.in ());
        ACE_OS::fclose (output_file_5);
      }

      if (output_file_6)
      {
        ACE_OS::fprintf (output_file_6,
                         "%s",
                         sixth_ior.in ());
        ACE_OS::fclose (output_file_6);
      }

      poa_manager->activate ();

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (str);
      return -1;
    }

  return 0;
}

