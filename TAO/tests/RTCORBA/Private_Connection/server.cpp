#include "testS.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

#include <memory>

class Test_i : public POA_Test
{
  // = TITLE
  //   An implementation for the Test interface in test.idl
  //
public:
  explicit Test_i (CORBA::ORB_ptr orb);
  // ctor

  ~Test_i () override = default;

  Test_i (Test_i const &) = delete;
  Test_i (Test_i &&) = delete;
  Test_i &operator= (Test_i const &) = delete;
  Test_i &operator= (Test_i &&) = delete;

  // = The Test methods.
  void test_method () override;

  //FUZZ: disable check_for_lack_ACE_OS
  void shutdown () override;
  //FUZZ: enable check_for_lack_ACE_OS

private:
  CORBA::ORB_var const orb_;
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
Test_i::shutdown ()
{
  this->orb_->shutdown (false);
}

//*************************************************************************

struct Server_Options
{
  ACE_TCHAR const *ior_output_file1 {ACE_TEXT("test1.ior")};
  ACE_TCHAR const *ior_output_file2 {ACE_TEXT("test2.ior")};
};

// Parse command-line arguments.
int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:p:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file1 = get_opts.opt_arg ();
        break;

      case 'p':
        options.ior_output_file2 = get_opts.opt_arg ();
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
check_for_nil (CORBA::Object_ptr obj, char const *msg)
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
               Test_i * const server_impl,
               ACE_TCHAR const *filename)
{
  // Register with poa.
  PortableServer::ObjectId_var id =
    poa->activate_object (server_impl);

  CORBA::Object_var const server =
    poa->id_to_reference (id.in ());

  // Print out the IOR.
  CORBA::String_var const ior =
    orb->object_to_string (server.in ());

  ACE_DEBUG ((LM_DEBUG, "<%C>\n\n", ior.in ()));

  // Print ior to the file.
  if (filename != nullptr)
    {
      std::unique_ptr<FILE, int (*) (FILE *)> output_file (
        ACE_OS::fopen (filename, ACE_TEXT("w")), ACE_OS::fclose);
      if (output_file == nullptr)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           filename),
                          -1);
      ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
    }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // ORB.
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      // Parse arguments.
      Server_Options options;
      if (parse_args (argc, argv, options) != 0)
        return -1;

      // RootPOA.
      CORBA::Object_var object =
        orb->resolve_initial_references("RootPOA");
      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (object.in ());
      if (check_for_nil (root_poa.in (), "RootPOA") == -1)
        return -1;

      // POAManager.
      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      // Servants.
      Test_i server_impl1 (orb.in ());
      Test_i server_impl2 (orb.in ());

      // Create Objects.
      int result = create_object (root_poa.in (),
                                 orb.in (),
                                 &server_impl1,
                                 options.ior_output_file1);
      if (result == -1)
        return -1;

      result = create_object (root_poa.in (),
                              orb.in (),
                              &server_impl2,
                              options.ior_output_file2);
      if (result == -1)
        return -1;

      // Run ORB Event loop.
      poa_manager->activate ();

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "Server ORB event loop finished\n"));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (
        "Unexpected exception caught in Private_Connection test server:");
      return -1;
    }

  return 0;
}
