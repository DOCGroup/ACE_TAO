// $Id$

//========================================================================
//
// = LIBRARY
//     TAO/tests/Smart_Proxy
//
// = FILENAME
//     server.cpp
//
// = DESCRIPTION
//     This is the server program that tests TAO's Smart Proxy extension.
//
// = AUTHOR
//     Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
//=========================================================================

#include "testS.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Smart_Proxy, server, "$Id$")

// The servant

class Test_i : public POA_Test
{
public:
  Test_i (CORBA::ORB_ptr orb);

  CORBA::Short method  (CORBA::Short boo
                        TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Test::Oops));

  void shutdown  (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;

};

Test_i::Test_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

CORBA::Short
Test_i :: method (CORBA::Short boo
                  TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Test::Oops))
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Test_i::method () invoked\n")));
  if (boo == 5)
    ACE_THROW_RETURN (Test::Oops ("Invalid boo\n"),
                      -1);

  return 0;
}

void
Test_i::shutdown (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 TAO_ENV_ARG_PARAMETER);
}

static const char *ior_output_file = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = ACE_OS::strdup (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  TAO_ENV_DECLARE_NEW_ENV;

  ACE_TRY
    {
      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            ""
                                            TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test_i servant (orb.in ());
      // Obtain RootPOA.
      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA"
                                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ()
                                      TAO_ENV_ARG_PARAMETER);

      ACE_TRY_CHECK;

      // Get the POAManager of the RootPOA.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test_var Test_object =
        servant._this (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior =
        orb->object_to_string (Test_object.in ()
                               TAO_ENV_ARG_PARAMETER);

      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != 0)
        {
          FILE *output_file =
            ACE_OS::fopen (ior_output_file, "w");

          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               ior_output_file),
                              1);

          ACE_OS::fprintf (output_file,
                           "%s",
                           ior.in ());
          ACE_OS::fclose (output_file);
        }

      poa_manager->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->run (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "event loop finished\n"));

      root_poa->destroy (1,
                         1
                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception in setting up server");
      ACE_ASSERT (0);
    }
  ACE_ENDTRY;
  return 0;
}
