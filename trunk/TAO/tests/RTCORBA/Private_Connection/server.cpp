// $Id$

#include "testS.h"
#include "ace/Get_Opt.h"

#if (TAO_HAS_RT_CORBA == 1)

class Test_i : public POA_Test
{
  // = TITLE
  //   An implementation for the Test interface in test.idl
  //
public:
  Test_i (CORBA::ORB_ptr orb);
  // ctor

  // = The Test methods.
  void test_method (CORBA::Environment&)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (CORBA::Environment&)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;
  // The ORB
};

Test_i::Test_i (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Test_i::test_method (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "Server: test_method invoked.\n"));
}

void
Test_i::shutdown (CORBA::Environment& ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0, ACE_TRY_ENV);
}

//*************************************************************************

const char *ior_output_file1 = "test1.ior";
const char *ior_output_file2 = "test2.ior";

// Parse command-line arguments.
int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:p:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file1 = get_opts.optarg;
        break;

      case 'p':
        ior_output_file2 = get_opts.optarg;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile1>"
                           "-p <iorfile2>"
                           "\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

int
check_for_nil (CORBA::Object_ptr obj, const char *msg)
{
  if (CORBA::is_nil (obj))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ERROR: Object reference <%s> is nil\n",
                       msg),
                      -1);
  else
    return 0;
}

int
create_object (PortableServer::POA_ptr poa,
               CORBA::ORB_ptr orb,
               Test_i *server_impl,
               const char *filename,
               CORBA::Environment &ACE_TRY_ENV)
{
  // Register with poa.
  PortableServer::ObjectId_var id = 
    poa->activate_object (server_impl, ACE_TRY_ENV);

  ACE_CHECK_RETURN (-1);

  CORBA::Object_var server =
    poa->id_to_reference (id.in (),
                          ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  // Print out the IOR.
  CORBA::String_var ior =
    orb->object_to_string (server.in (), ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);
  
  ACE_DEBUG ((LM_DEBUG, "<%s>\n\n", ior.in ()));

  // Print ior to the file.
  if (filename != 0)
    {
      FILE *output_file= ACE_OS::fopen (filename, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           filename),
                          -1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);
    }

  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      // ORB.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Parse arguments.
      if (parse_args (argc, argv) != 0)
        return 1;

      // RootPOA.
      CORBA::Object_var object =
        orb->resolve_initial_references("RootPOA", ACE_TRY_ENV);
      ACE_TRY_CHECK;
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (check_for_nil (root_poa.in (), "RootPOA") == -1)
        return 1;

      // POAManager.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Servants.
      Test_i server_impl1 (orb.in ());
      Test_i server_impl2 (orb.in ());
      
      // Create Objects.
      int result;
      result = create_object (root_poa.in (),
                              orb.in (),
                              &server_impl1,
                              ior_output_file1,
                              ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (result == -1)
        return 1;

      result = create_object (root_poa.in (),
                              orb.in (),
                              &server_impl2,
                              ior_output_file2,
                              ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (result == -1)
        return 1;

      // Run ORB Event loop.
      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      orb->run (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Server ORB event loop finished\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
      "Unexpected exception caught in Private_Connection test server:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

#else /* TAO_HAS_RT_CORBA == 1 */

int
main (int argc, char *argv[])
{
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
  ACE_ERROR_RETURN ((LM_ERROR,
                     "\nRTCORBA must be enabled to run this test!\n"),
                    1);
}

#endif /* TAO_HAS_RT_CORBA == 1 */
