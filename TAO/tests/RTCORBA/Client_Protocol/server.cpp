// $Id$

#include "testS.h"
#include "ace/Get_Opt.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTCORBA/RT_Policy_i.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "tao/Strategies/advanced_resource.h"

class Test_i : public POA_Test
{
  // = TITLE
  //   An implementation for the Test interface in test.idl
  //
public:
  Test_i (CORBA::ORB_ptr orb);
  // ctor

  void test_method (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
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
Test_i::test_method (TAO_ENV_SINGLE_ARG_DECL_NOT_USED /*TAO_ENV_SINGLE_ARG_PARAMETER*/)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "Test method invoked on the sever\n"));
}

void
Test_i::shutdown (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 TAO_ENV_ARG_PARAMETER);
}

//*************************************************************************

const char *ior_output_file1 = "test1.ior";
const char *ior_output_file2 = "test2.ior";
CORBA::ULong protocol_type = 0;

// Parse command-line arguments.
int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "s:c:p:");
  int c, result;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 's':
        ior_output_file1 = get_opts.opt_arg ();
        break;

      case 'c':
        ior_output_file2 = get_opts.opt_arg ();
        break;

      case 'p':
        result = ::sscanf (get_opts.opt_arg (),
                           "%u",
                           &protocol_type);
        if (result == 0 || result == EOF)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to process <-p> option"),
                            -1);
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-s <iorfile> "
                           "-c <iorfile> "
                           "-p <protocol_type> "
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
               const char *filename
               TAO_ENV_ARG_DECL)
{
  // Register with poa.
  PortableServer::ObjectId_var id =
    poa->activate_object (server_impl TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CORBA::Object_var server =
    poa->id_to_reference (id.in ()
                          TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Print out the IOR.
  CORBA::String_var ior =
    orb->object_to_string (server.in () TAO_ENV_ARG_PARAMETER);
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
        CORBA::ORB_init (argc, argv, "" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Parse arguments.
      if (parse_args (argc, argv) != 0)
        return 1;

      // RTORB.
      CORBA::Object_var object =
        orb->resolve_initial_references ("RTORB" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      RTCORBA::RTORB_var rt_orb = RTCORBA::RTORB::_narrow (object.in ()
                                                           TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (check_for_nil (rt_orb.in (), "RTORB") == -1)
        return 1;

      // RootPOA.
      object =
        orb->resolve_initial_references("RootPOA" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (check_for_nil (root_poa.in (), "RootPOA") == -1)
        return 1;

      // POAManager.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create child POA with RTCORBA::ClientProtocolPolicy set.
      RTCORBA::ProtocolList protocols;
      protocols.length (1);
      protocols[0].protocol_type = protocol_type;
      protocols[0].orb_protocol_properties =
        TAO_Protocol_Properties_Factory::create_orb_protocol_property
        (protocol_type);
      protocols[0].transport_protocol_properties =
        TAO_Protocol_Properties_Factory::create_transport_protocol_property
        (protocol_type);

      CORBA::PolicyList poa_policy_list;
      poa_policy_list.length (1);
      poa_policy_list[0] =
        rt_orb->create_client_protocol_policy (protocols
                                               TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var poa =
        root_poa->create_POA ("Child_POA",
                              poa_manager.in (),
                              poa_policy_list
                              TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Servant.
      Test_i server_impl (orb.in ());

      // Create object 1.
      int result;
      ACE_DEBUG ((LM_DEBUG, "\nActivated object one as "));
      result = create_object (poa.in (), orb.in (), &server_impl,
                              ior_output_file1 TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (result == -1)
        return 1;

      // Create object 2.
      ACE_DEBUG ((LM_DEBUG, "\nActivated object two as "));
      result = create_object (root_poa.in (), orb.in (), &server_impl,
                              ior_output_file2 TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (result == -1)
        return 1;

      // Run ORB Event loop.
      poa_manager->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->run (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Server ORB event loop finished\n\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
      "Unexpected exception caught in ClientProtocolPolicy: test server");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

