// $Id$

#include "testS.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

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

const ACE_TCHAR *ior_output_file1 = ACE_TEXT("test1.ior");
const ACE_TCHAR *ior_output_file2 = ACE_TEXT("test2.ior");

// Parse command-line arguments.
int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:p:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file1 = get_opts.opt_arg ();
        break;

      case 'p':
        ior_output_file2 = get_opts.opt_arg ();
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
                       "ERROR: Object reference <%C> is nil\n",
                       msg),
                      -1);
  else
    return 0;
}

int
create_object (PortableServer::POA_ptr poa,
               CORBA::ORB_ptr orb,
               Test_i *server_impl,
               const ACE_TCHAR *filename)
{
  // Register with poa.
  PortableServer::ObjectId_var id =
    poa->activate_object (server_impl);


  CORBA::Object_var server =
    poa->id_to_reference (id.in ());

  // Print out the IOR.
  CORBA::String_var ior =
    orb->object_to_string (server.in ());

  ACE_DEBUG ((LM_DEBUG, "<%C>\n\n", ior.in ()));

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

      // Servants.
      Test_i server_impl1 (orb.in ());
      Test_i server_impl2 (orb.in ());

      // Create Objects.
      int result;
      result = create_object (root_poa.in (),
                              orb.in (),
                              &server_impl1,
                              ior_output_file1);
      if (result == -1)
        return -1;

      result = create_object (root_poa.in (),
                              orb.in (),
                              &server_impl2,
                              ior_output_file2);
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
        "Unexpected exception caught in Private_Connection test server:");
      return -1;
    }

  return 0;
}
