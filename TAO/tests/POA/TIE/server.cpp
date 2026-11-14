
//=============================================================================
/**
 *  @file    server.cpp
 *
 *  In this example,
 *
 *  @author Irfan Pyarali
 */
//=============================================================================
#include "ace/streams.h"
#include "test_i.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Get_Opt.h"
#include <array>
#include <cstring>
#include <memory>

struct Options
{
  std::array<ACE_TCHAR const *, 6> ior_files {{
    ACE_TEXT ("ior_1"), ACE_TEXT ("ior_2"), ACE_TEXT ("ior_3"),
    ACE_TEXT ("ior_4"), ACE_TEXT ("ior_5"), ACE_TEXT ("ior_6")
  }};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("a:b:c:d:e:f:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'a':
        options.ior_files[0] = get_opts.opt_arg ();
        break;
      case 'b':
        options.ior_files[1] = get_opts.opt_arg ();
        break;
      case 'c':
        options.ior_files[2] = get_opts.opt_arg ();
        break;
      case 'd':
        options.ior_files[3] = get_opts.opt_arg ();
        break;
      case 'e':
        options.ior_files[4] = get_opts.opt_arg ();
        break;
      case 'f':
        options.ior_files[5] = get_opts.opt_arg ();
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

static int
write_ior (char const *filename, char const *ior)
{
  std::unique_ptr<FILE, int (*) (FILE *)> const output_file (
    ACE_OS::fopen (filename, "w"), ACE_OS::fclose);
  if (!output_file)
    ACE_ERROR_RETURN ((LM_ERROR, "Cannot open IOR file %s for writing\n", filename), -1);

  int const result = ACE_OS::fprintf (output_file.get (), "%s", ior);
  if (result < 0 || static_cast<size_t> (result) != std::strlen (ior))
    ACE_ERROR_RETURN ((LM_ERROR, "Failed writing IOR to %s\n", filename), -1);

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  char const *operation {};

  try
    {
      Options options;
      operation = "CORBA::ORB_init";

      // Initialize the ORB first.
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      // Obtain the RootPOA.
      CORBA::Object_var const obj =
        orb->resolve_initial_references ("RootPOA");

      operation = "PortableServer::POA::_narrow";

      // Get the POA_var object from Object_var.
      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (obj.in ());

      operation = "PortableServer::POA::the_POAManager";
      // Get the POAManager of the RootPOA.
      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      // Policies for the firstPOA to be created.
      CORBA::PolicyList policies (2);
      policies.length (2);

      operation = "PortableServer::POA::create_lifespan_policy";
      // Lifespan policy
      policies[0] =
        root_poa->create_lifespan_policy (PortableServer::PERSISTENT);

      operation = "PortableServer::POA::create_implicit_activation_policy";
      // Implicit activation policy
      policies[1] =
        root_poa->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION);

      operation = "PortableServer::POA::create_POA";
      // Create the firstPOA under the RootPOA.
      PortableServer::POA_var const first_poa =
        root_poa->create_POA ("firstPOA",
                              poa_manager.in (),
                              policies);

      // Creation of POAs is over. Destroy the Policy objects.
      for (CORBA::ULong i = 0;
           i < policies.length ();
           ++i)
        {
          CORBA::Policy_ptr const policy = policies[i];
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

      operation = "POA_A::_this";
      // Get Object Reference for the a_impl object.
      A_var const a = a_impl._this ();

      operation = "POA_Outer::B::_this";
      // Get Object Reference for the b_impl object.
      Outer::B_var const b = b_impl._this ();

      operation = "POA_Outer::Inner::C::_this";
      // Get Object Reference for the c_impl object.
      Outer::Inner::C_var const c = c_impl._this ();

      operation = "POA_A::_this";
      // Get Object Reference for the a_tie_impl object.
      A_var const a_tie = a_tie_impl._this ();

      operation = "POA_Outer::B::_this";
      // Get Object Reference for the a_tie_impl object.
      Outer::B_var const b_tie = b_tie_impl._this ();

      operation = "POA_Outer::C::_this";
      // Get Object Reference for the c_tie_impl object.
      Outer::Inner::C_var const c_tie = c_tie_impl._this ();

      operation = "CORBA::ORB::object_to_string";
      // Stringyfy all the object references and print them out.
      CORBA::String_var const first_ior =
        orb->object_to_string (a.in ());

      CORBA::String_var const second_ior =
        orb->object_to_string (b.in ());

      CORBA::String_var const third_ior =
        orb->object_to_string (c.in ());

      CORBA::String_var const forth_ior =
        orb->object_to_string (a_tie.in ());

      CORBA::String_var const fifth_ior =
        orb->object_to_string (b_tie.in ());

      CORBA::String_var const sixth_ior =
        orb->object_to_string (c_tie.in ());

      std::array<char const *, 6> const iors {{
        first_ior.in (), second_ior.in (), third_ior.in (),
        forth_ior.in (), fifth_ior.in (), sixth_ior.in ()
      }};
      for (CORBA::ULong i {}; i < iors.size (); ++i)
        if (write_ior (ACE_TEXT_ALWAYS_CHAR (options.ior_files[i]), iors[i]) != 0)
          return -1;

      poa_manager->activate ();

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (operation);
      return -1;
    }

  return 0;
}

