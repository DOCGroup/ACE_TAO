#include "testS.h"
#include "ace/Get_Opt.h"

#include "tao/Strategies/advanced_resource.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTPortableServer/RTPortableServer.h"

#include <cstdio>
#include <memory>

class Test_i : public POA_Test
{
  // = TITLE
  //   An implementation for the Test interface in test.idl
  //
public:
  explicit Test_i (CORBA::ORB_ptr orb);
  Test_i (Test_i const &) = delete;
  Test_i (Test_i &&) = delete;
  Test_i &operator= (Test_i const &) = delete;
  Test_i &operator= (Test_i &&) = delete;
  // ctor

  // = The Test methods.
  void test_method ();

  //FUZZ: disable check_for_lack_ACE_OS
  void shutdown ();
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
Test_i::test_method ()
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
  ACE_TCHAR const *ior_output_file { ACE_TEXT("test.ior") };
};

// Parse command-line arguments.
int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
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
check_for_nil (CORBA::Object_ptr obj, char const *msg)
{
  if (CORBA::is_nil (obj))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ERROR: Object reference <%C> is nil\n",
                       msg),
                      -1);

  return 0;
}

int
create_object (PortableServer::POA_ptr poa,
               CORBA::ORB_ptr orb,
               Test_i *server_impl,
               ACE_TCHAR const *filename)
{
  // Register with poa.
  PortableServer::ObjectId_var const id =
    poa->activate_object (server_impl);

  CORBA::Object_var const server =
    poa->id_to_reference (id.in ());

  // Print out the IOR.
  CORBA::String_var const ior =
    orb->object_to_string (server.in ());

  ACE_DEBUG ((LM_DEBUG, "<%C>\n\n", ior.in ()));

  // Print ior to the file.
  std::unique_ptr<FILE, int (*) (FILE *)> output_file (
    ACE_OS::fopen (filename, "w"), ACE_OS::fclose);
  if (!output_file)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot open output file for writing IOR: %s",
                       filename),
                      -1);
  ACE_OS::fprintf (output_file.get (), "%s", ior.in ());

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      Server_Options options;

      // ORB.
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      // Parse arguments.
      if (parse_args (argc, argv, options) != 0)
        return -1;

      // RootPOA.
      CORBA::Object_var const object =
        orb->resolve_initial_references("RootPOA");
      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (object.in ());
      if (check_for_nil (root_poa.in (), "RootPOA") == -1)
        return -1;

      // POAManager.
      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      // Servant.
      Test_i server_impl (orb.in ());

      // Create Object.
      int const result = create_object (root_poa.in (),
                                        orb.in (),
                                        &server_impl,
                                        options.ior_output_file);
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
        "Unexpected exception caught in Explicit_Binding test server:");
      return -1;
    }

  return 0;
}
