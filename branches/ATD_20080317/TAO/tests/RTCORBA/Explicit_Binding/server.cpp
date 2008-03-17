// $Id$

#include "testS.h"
#include "ace/Get_Opt.h"

#include "tao/Strategies/advanced_resource.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTPortableServer/RTPortableServer.h"

class Test_i : public POA_Test
{
  // = TITLE
  //   An implementation for the Test interface in test.idl
  //
public:
  Test_i (CORBA::ORB_ptr orb);
  // ctor

  // = The Test methods.
  void test_method (void);

  //FUZZ: disable check_for_lack_ACE_OS
  void shutdown (void);
  //FUZZ: enable check_for_lack_ACE_OS

private:
  CORBA::ORB_var orb_;
  // The ORB
};

Test_i::Test_i (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Test_i::test_method (/* */)
{
  ACE_DEBUG ((LM_DEBUG,
              "Server: test_method invoked.\n"));
}

void
Test_i::shutdown (void)
{
  this->orb_->shutdown (0);
}

//*************************************************************************

const char *ior_output_file = "test.ior";

// Parse command-line arguments.
int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
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
               const char *filename)
{
  // Register with poa.
  PortableServer::ObjectId_var id =
    poa->activate_object (server_impl);


  CORBA::Object_var server =
    poa->id_to_reference (id.in ());

  // Print out the IOR.
  CORBA::String_var ior =
    orb->object_to_string (server.in ());

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
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      // ORB.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      // Parse arguments.
      if (parse_args (argc, argv) != 0)
        return -1;

      // RootPOA.
      CORBA::Object_var object =
        orb->resolve_initial_references("RootPOA");
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ());
      if (check_for_nil (root_poa.in (), "RootPOA") == -1)
        return -1;

      // POAManager.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      // Servant.
      Test_i server_impl (orb.in ());

      // Create Object.
      int result;
      result = create_object (root_poa.in (),
                              orb.in (),
                              &server_impl,
                              ior_output_file);
      if (result == -1)
        return -1;

      // Run ORB Event loop.
      poa_manager->activate ();

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "Server ORB event loop finished\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Unexpected exception caught in Explicit_Binding test server:");
      return -1;
    }

  return 0;
}
