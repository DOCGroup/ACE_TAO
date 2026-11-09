#include "testS.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "../check_supported_priorities.cpp"

#include <cstdio>
#include <cstring>
#include <memory>

class Test_i : public POA_Test
{
public:
  Test_i (CORBA::ORB_ptr orb,
          RTCORBA::PriorityBands const &bands);

  void test_method (CORBA::Boolean client_propagated,
                    CORBA::Short priority) override;

  //FUZZ: disable check_for_lack_ACE_OS
  void shutdown ();
  //FUZZ: enable check_for_lack_ACE_OS

private:
  Test_i (Test_i const &) = delete;
  Test_i &operator= (Test_i const &) = delete;
  Test_i (Test_i &&) = delete;
  Test_i &operator= (Test_i &&) = delete;

  CORBA::ORB_var orb_;
  RTCORBA::PriorityBands const &bands_;
  RTCORBA::Current_var rt_current_;
};

Test_i::Test_i (CORBA::ORB_ptr orb,
                RTCORBA::PriorityBands const &bands)
  : orb_ (CORBA::ORB::_duplicate (orb)),
     bands_ (bands),
     rt_current_ ()
{
  // We resolve and store the RT Current for later use.
  CORBA::Object_var obj =
    this->orb_->resolve_initial_references ("RTCurrent");

  this->rt_current_ =
    RTCORBA::Current::_narrow (obj.in ());
}

void
Test_i::test_method (CORBA::Boolean client_propagated,
                     CORBA::Short client_priority)
{
  // Get the upcall thread's priority.
  CORBA::Short const server_priority =
    this->rt_current_->the_priority ();

  // Check which policy we are dealing with.
  if (!client_propagated)
    {
      // With the SERVER_DECLARED priority model, <client_priority> is
      // simply the priority associated with the priority propagation
      // policy in the IOR.  This should match the priority we get run
      // at.
      ACE_ASSERT (server_priority == client_priority);

      ACE_DEBUG ((LM_DEBUG,
                  "Using SERVER_DECLARED policy: request processed at priority %d\n",
                  server_priority));
    }
  else
    {
      // We are using the CLIENT_DECLARED policy, both the client
      // priority and the server priority should fall within the
      // bands.  Note that it may be the case that the server priority
      // is not exactly the same as the client priority since we are
      // using thread pools with lanes.
      //
      // Print out the bands.
      CORBA::ULong index {};
      ACE_DEBUG ((LM_DEBUG,
                  "\nPriority Bands:\n"));
      for (CORBA::ULong i = 0; i < this->bands_.length (); ++i)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "%d) %d  %d\n",
                      (i + 1),
                      this->bands_[i].low,
                      this->bands_[i].high));

          // Check which band we are using.
          if (client_priority <= this->bands_[i].high &&
              client_priority >= this->bands_[i].low &&
              server_priority <= this->bands_[i].high &&
              server_priority >= this->bands_[i].low)
            index = i + 1;
        }

      ACE_DEBUG ((LM_DEBUG,
                  "Client priority: %d  "
                  "Server processing request at priority: %d\n",
                  client_priority,
                  server_priority));

      if (index == 0)
        ACE_DEBUG ((LM_DEBUG,
                    "ERROR: object and thread priorities do not "
                    "match the same band.\n"));
      else
        ACE_DEBUG ((LM_DEBUG,
                    "Band %u was used for this invocation\n", index));
    }
}

void
Test_i::shutdown ()
{
  this->orb_->shutdown (false);
}

//*************************************************************************

struct Server_Options
{
  ACE_TCHAR const *bands_file {ACE_TEXT("bands")};
  ACE_TCHAR const *client_propagated_ior {ACE_TEXT("test1.ior")};
  ACE_TCHAR const *server_declared_ior {ACE_TEXT("test2.ior")};
};

// Parse command-line arguments.
int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("b:o:n:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'n':
        options.client_propagated_ior = get_opts.opt_arg ();
        break;

      case 'o':
        options.server_declared_ior = get_opts.opt_arg ();
        break;

      case 'b':
        options.bands_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-n <iorfile1> "
                           "-o <iorfile2> "
                           "-b <bands_file> "
                           "\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

int
get_priority_bands (RTCORBA::PriorityBands &bands,
                    ACE_TCHAR const *bands_file)
{
  //
  // Read bands from a file.
  //
  FILE *file =
    ACE_OS::fopen (bands_file, "r");

  if (file == nullptr)
    return -1;

  ACE_Read_Buffer reader (file, 1);

  struct Read_Buffer_Deleter
  {
    ACE_Allocator *allocator;

    void operator() (char *buffer) const
    {
      this->allocator->free (buffer);
    }
  };

  std::unique_ptr<char, Read_Buffer_Deleter> string (
    reader.read (EOF, ' ', '\0'), Read_Buffer_Deleter {reader.alloc ()});

  if (!string)
    return -1;

  CORBA::ULong const bands_length =
    (reader.replaced () + 1) / 2;
  bands.length (bands_length);

  int result = 1;
  char *working_string = string.get ();
  for (CORBA::ULong i = 0; i < bands_length; ++i)
    {
      result = std::sscanf (working_string, "%hd", &bands[i].low);
      if (result == 0 || result == EOF)
        break;

      working_string += std::strlen (working_string);
      working_string += 1;

      result = std::sscanf (working_string, "%hd", &bands[i].high);
      if (result == 0 || result == EOF)
        break;

      working_string += std::strlen (working_string);
      working_string += 1;

      if (bands[i].low > bands[i].high)
        {
          result = 0;
          break;
        }
    }

  if (result == 0 || result == EOF)
    return -1;
  else
    return 0;
}

int
create_object (PortableServer::POA_ptr poa,
               CORBA::ORB_ptr orb,
               Test_i * const server_impl,
               ACE_TCHAR const *filename)
{
  // Register servant with the POA.
  PortableServer::ObjectId_var id;
  id = poa->activate_object (server_impl);

  // Create object reference.
  CORBA::Object_var server =
    poa->id_to_reference (id.in ());

  // Print out the IOR.
  CORBA::String_var ior =
    orb->object_to_string (server.in ());

  // Print ior to the file.
  if (filename != nullptr)
    {
      std::unique_ptr<FILE, int (*) (FILE *)> output_file (
        ACE_OS::fopen (filename, "w"), ACE_OS::fclose);
      if (!output_file)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           filename),
                          -1);
      ACE_OS::fprintf (output_file.get (),
                       "%s",
                       ior.in ());
    }

  return 0;
}

void
object_activation_exception_test (RTPortableServer::POA_ptr poa,
                                  Test_i * const server_impl,
                                  CORBA::Short priority)
{
  try
    {
      // Register servant with POA.
      PortableServer::ObjectId_var id =
        poa->activate_object_with_priority (server_impl,
                                            priority);

      // This next line of code should not run because an exception
      // should have been raised.
      ACE_DEBUG ((LM_DEBUG, "ERROR: no exception caught\n"));
    }
  catch (const CORBA::BAD_PARAM&)
    {
      // Expected exception.
      ACE_DEBUG ((LM_DEBUG,
                  "BAD_PARAM exception is caught as expected.\n"));
    }
  catch (const CORBA::Exception&)
    {
      // Unexpected exception.
      ACE_DEBUG ((LM_DEBUG, "ERROR: unexpected exception caught\n"));
      throw;
    }
}

void
poa_creation_exception_test (PortableServer::POA_ptr root_poa,
                             PortableServer::POAManager_ptr manager,
                             CORBA::PolicyList const &policies)
{
  try
    {
      // Create a POA with invalid policies.
      PortableServer::POA_var child_poa =
        root_poa->create_POA ("Child_POA",
                              manager,
                              policies);

      // This next line of code should not run because an exception
      // should have been raised.
      ACE_DEBUG ((LM_DEBUG, "ERROR: no exception caught\n"));
    }
  catch (const PortableServer::POA::InvalidPolicy&)
    {
      // Expected exception.
      ACE_DEBUG ((LM_DEBUG,
                  "InvalidPolicy exception is caught as expected.\n"));
    }
  catch (const CORBA::Exception&)
    {
      // Unexpected exception.
      ACE_DEBUG ((LM_DEBUG, "ERROR: unexpected exception\n"));
      throw;
    }
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  CORBA::ORB_var orb;

  try
    {
      // Initialize ORB.
      orb =
        CORBA::ORB_init (argc,
                         argv);

      // Parse arguments.
      Server_Options options;
      int result =
        parse_args (argc, argv, options);
      if (result != 0)
        return result;

      // Make sure we can support multiple priorities that are required
      // for this test.
      if (!check_supported_priorities (orb.in ()))
        return 2;

      // Get the RTORB.
      CORBA::Object_var object =
        orb->resolve_initial_references ("RTORB");

      RTCORBA::RTORB_var const rt_orb =
        RTCORBA::RTORB::_narrow (object.in ());

      // Get the RootPOA.
      object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (object.in ());

      // Get the POA Manager.
      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      // Obtain priority bands to be used in this test from the file
      // specified by the user.
      RTCORBA::PriorityBands bands;
      result = get_priority_bands (bands, options.bands_file);
      if (result != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error reading priority bands from file\n"),
                          result);

      // Create a thread-pool.
      CORBA::ULong const stacksize {};
      CORBA::Boolean const allow_request_buffering {};
      CORBA::ULong const max_buffered_requests {};
      CORBA::ULong const max_request_buffer_size {};
      CORBA::Boolean const allow_borrowing {};
      CORBA::ULong const static_threads {1};
      CORBA::ULong const dynamic_threads {};

      // The lanes in the pool should match the bands.
      RTCORBA::ThreadpoolLanes lanes;
      lanes.length (bands.length ());

      // For each band, setup up a thread lane.
      for (CORBA::ULong i = 0;
           i < bands.length ();
           ++i)
        {
          lanes[i].static_threads = static_threads;
          lanes[i].dynamic_threads = dynamic_threads;

          // Lane priority is in the middle of the band priorities.
          lanes[i].lane_priority =
            (bands[i].low + bands[i].high) / 2;
        }

      // Create the thread-pool.
      RTCORBA::ThreadpoolId const threadpool_id =
        rt_orb->create_threadpool_with_lanes (stacksize,
                                              lanes,
                                              allow_borrowing,
                                              allow_request_buffering,
                                              max_buffered_requests,
                                              max_request_buffer_size);

      // Test: Attempt to create a POA with priority bands that do not
      // match the lanes.  Should get POA::InvalidPolicy exception.
      ACE_DEBUG ((LM_DEBUG,
                  "\n<---Test--->: Bands do not match lanes\n\n"));

      // False bands.
      RTCORBA::PriorityBands false_bands (bands);
      false_bands[0].low = 10000;
      false_bands[0].high = 10005;

      CORBA::PolicyList poa_policy_list;
      poa_policy_list.length (2);

      // Create a bands policy.
      poa_policy_list[0] =
        rt_orb->create_priority_banded_connection_policy (false_bands);

      // Create a thread-pool policy.
      poa_policy_list[1] =
        rt_orb->create_threadpool_policy (threadpool_id);

      // Try to create a POA with invalid policies.  Should throw an
      // exception.
      poa_creation_exception_test (root_poa.in (),
                                   poa_manager.in (),
                                   poa_policy_list);

      // Two policies for the next POA.
      poa_policy_list.length (2);

      // Create a priority model policy.
      poa_policy_list[0] =
        rt_orb->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED,
                                              0);

      // Create a thread-pool policy.
      poa_policy_list[1] =
        rt_orb->create_threadpool_policy (threadpool_id);

      // Create POA with CLIENT_PROPAGATED priority model, with lanes
      // but no bands.
      PortableServer::POA_var const client_propagated_poa =
        root_poa->create_POA ("client_propagated_poa",
                              poa_manager.in (),
                              poa_policy_list);

      // Three policies for the next POA.
      poa_policy_list.length (3);

      // Default POA priority comes from the 'middle' lane's priority.
      CORBA::Short const poa_priority =
        lanes[lanes.length () / 2].lane_priority;

      // Create a priority model policy.
      poa_policy_list[0] =
        rt_orb->create_priority_model_policy (RTCORBA::SERVER_DECLARED,
                                              poa_priority);

      // Create a bands policy.
      poa_policy_list[1] =
        rt_orb->create_priority_banded_connection_policy (bands);

      // Create a thread-pool policy.
      poa_policy_list[2] =
        rt_orb->create_threadpool_policy (threadpool_id);

      // Create POA with SERVER_DECLARED priority model, with bands
      // and lanes.
      PortableServer::POA_var const server_declared_poa =
        root_poa->create_POA ("server_declared_poa",
                              poa_manager.in (),
                              poa_policy_list);

      // Test: Attempt to register an object with priority that
      // doesn't match lanes.  Should get BAD_PARAM exception.
      ACE_DEBUG ((LM_DEBUG,
                  "\n<---Test--->: Servant priority does not match lanes\n\n"));

      RTPortableServer::POA_var const rt_server_declared_poa =
        RTPortableServer::POA::_narrow (server_declared_poa.in ());

      // Activation with incorrect priority should fail.
      CORBA::Short const wrong_priority {10000};
      object_activation_exception_test (rt_server_declared_poa.in (),
                                        nullptr,
                                        wrong_priority);

      // Create first servant and register with <client_propagated_poa>.
      Test_i server_impl (orb.in (),
                          bands);
      result = create_object (client_propagated_poa.in (),
                              orb.in (),
                              &server_impl,
                              options.client_propagated_ior);
      if (result != 0)
        return result;

      // Create second servant and register with <server_declared_poa>.
      Test_i server_impl2 (orb.in (),
                           bands);
      result = create_object (server_declared_poa.in (),
                              orb.in (),
                              &server_impl2,
                              options.server_declared_ior);
      if (result != 0)
        return result;

      // Activate POA manager.
      poa_manager->activate ();

      // Run ORB.
      orb->run ();

      // Destroy ORB.
      orb->destroy ();
    }
  catch (const CORBA::INTERNAL& exception)
    {
      int minor_code =
        exception.minor ();

      if (errno == EPERM)
        {
          if (ACE_BIT_ENABLED (minor_code, TAO_RTCORBA_THREAD_CREATION_LOCATION_CODE))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Cannot create thread with scheduling policy %s\n"
                                 "because the user does not have the appropriate privileges, terminating program....\n"
                                 "Check svc.conf options and/or run as root\n",
                                 sched_policy_name (orb->orb_core ()->orb_params ()->ace_sched_policy ())),
                                2);
            }
          else
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "ERROR: would expect that TAO_RTCORBA_THREAD_CREATION_LOCATION_CODE "
                                 "minor code is set, minor code is %d",
                                 TAO_RTCORBA_THREAD_CREATION_LOCATION_CODE), -1);
            }
        }
      else
        {
          exception._tao_print_exception (
            "Unexpected exception caught in Banded_Connections test server:");
          return -1;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Unexpected exception caught in Banded_Connections test server:");
      return -1;
    }

  return 0;
}
