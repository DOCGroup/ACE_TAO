// $Id$

#include "testS.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "../check_supported_priorities.cpp"

class Test_i : public POA_Test
{
public:
  Test_i (CORBA::ORB_ptr orb,
          RTCORBA::PriorityBands &bands
          ACE_ENV_ARG_DECL);

  void test_method (CORBA::Boolean client_propagated,
                    CORBA::Short priority
                    ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;
  RTCORBA::PriorityBands &bands_;
  RTCORBA::Current_var rt_current_;
};

Test_i::Test_i (CORBA::ORB_ptr orb,
                RTCORBA::PriorityBands &bands
                ACE_ENV_ARG_DECL)
  :  orb_ (CORBA::ORB::_duplicate (orb)),
     bands_ (bands),
     rt_current_ ()
{
  // We resolve and store the RT Current for later use.
  CORBA::Object_var obj =
    this->orb_->resolve_initial_references ("RTCurrent"
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->rt_current_ =
    RTCORBA::Current::_narrow (obj.in ()
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
Test_i::test_method (CORBA::Boolean client_propagated,
                     CORBA::Short client_priority
                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Get the upcall thread's priority.
  CORBA::Short server_priority =
    this->rt_current_->the_priority (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

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
      int index = -1;
      ACE_DEBUG ((LM_DEBUG,
                  "\nPriority Bands: \n"));
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

      if (index == -1)
        ACE_DEBUG ((LM_DEBUG,
                    "ERROR: object and thread priorities do not"
                    "match the same band.\n"));
      else
        ACE_DEBUG ((LM_DEBUG,
                    "Band %d was used for this invocation\n", index));
    }
}

void
Test_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

//*************************************************************************

const char *bands_file = "bands";
const char *ior_output_file1 = "test1.ior";
const char *ior_output_file2 = "test2.ior";

// Parse command-line arguments.
int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "b:o:n:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'n':
        ior_output_file1 = get_opts.opt_arg ();
        break;

      case 'o':
        ior_output_file2 = get_opts.opt_arg ();
        break;

      case 'b':
        bands_file = get_opts.opt_arg ();
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
get_priority_bands (RTCORBA::PriorityBands &bands)
{
  //
  // Read bands from a file.
  //
  FILE* file =
    ACE_OS::fopen (bands_file, "r");

  if (file == 0)
    return -1;

  ACE_Read_Buffer reader (file, 1);

  char *string =
    reader.read (EOF, ' ', '\0');

  if (string == 0)
    return -1;

  CORBA::ULong bands_length =
    (reader.replaced () + 1) / 2;
  bands.length (bands_length);

  int result = 1;
  char* working_string = string;
  for (CORBA::ULong i = 0; i < bands_length; ++i)
    {
      result = ::sscanf (working_string,
                         "%hd",
                         &bands[i].low);
      if (result == 0 || result == EOF)
        break;

      working_string += ACE_OS::strlen (working_string);
      working_string += 1;

      result = ::sscanf (working_string,
                         "%hd",
                         &bands[i].high);
      if (result == 0 || result == EOF)
        break;

      working_string += ACE_OS::strlen (working_string);
      working_string += 1;

      if (bands[i].low > bands[i].high)
        {
          result = 0;
          break;
        }
    }

  reader.alloc ()->free (string);

  if (result == 0 || result == EOF)
    return -1;
  else
    return 0;
}

int
create_object (PortableServer::POA_ptr poa,
               CORBA::ORB_ptr orb,
               Test_i *server_impl,
               const char *filename
               ACE_ENV_ARG_DECL)
{
  // Register servant with the POA.
  PortableServer::ObjectId_var id;
  id = poa->activate_object (server_impl
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Create object reference.
  CORBA::Object_var server =
    poa->id_to_reference (id.in ()
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Print out the IOR.
  CORBA::String_var ior =
    orb->object_to_string (server.in ()
                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Print ior to the file.
  if (filename != 0)
    {
      FILE *output_file =
        ACE_OS::fopen (filename, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           filename),
                          -1);
      ACE_OS::fprintf (output_file,
                       "%s",
                       ior.in ());
      ACE_OS::fclose (output_file);
    }

  return 0;
}

void
object_activation_exception_test (RTPortableServer::POA_ptr poa,
                                  Test_i *server_impl,
                                  CORBA::Short priority
                                  ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      // Register servant with POA.
      PortableServer::ObjectId_var id =
        poa->activate_object_with_priority (server_impl,
                                            priority
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // This next line of code should not run because an exception
      // should have been raised.
      ACE_DEBUG ((LM_DEBUG, "ERROR: no exception caught\n"));
    }
  ACE_CATCH (CORBA::BAD_PARAM, ex)
    {
      // Expected exception.
      ACE_DEBUG ((LM_DEBUG,
                  "BAD_PARAM exception is caught as expected.\n"));
    }
  ACE_CATCHANY
    {
      // Unexpected exception.
      ACE_DEBUG ((LM_DEBUG, "ERROR: unexpected exception caught\n"));
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

void
poa_creation_exception_test (PortableServer::POA_ptr root_poa,
                             PortableServer::POAManager_ptr manager,
                             CORBA::PolicyList &policies
                             ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      // Create a POA with invalid policies.
      PortableServer::POA_var child_poa =
        root_poa->create_POA ("Child_POA",
                              manager,
                              policies
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // This next line of code should not run because an exception
      // should have been raised.
      ACE_DEBUG ((LM_DEBUG, "ERROR: no exception caught\n"));
    }
  ACE_CATCH (PortableServer::POA::InvalidPolicy, ex)
    {
      // Expected exception.
      ACE_DEBUG ((LM_DEBUG,
                  "InvalidPolicy exception is caught as expected.\n"));
    }
  ACE_CATCHANY
    {
      // Unexpected exception.
      ACE_DEBUG ((LM_DEBUG, "ERROR: unexpected exception\n"));
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

int
main (int argc, char *argv[])
{
  CORBA::ORB_var orb;

  ACE_TRY_NEW_ENV
    {
      // Initialize ORB.
      orb =
        CORBA::ORB_init (argc,
                         argv,
                         ""
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Parse arguments.
      int result =
        parse_args (argc,
                    argv);
      if (result != 0)
        return result;

      // Make sure we can support multiple priorities that are required
      // for this test.
      check_supported_priorities (orb.in ());

      // Get the RTORB.
      CORBA::Object_var object =
        orb->resolve_initial_references ("RTORB"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RTCORBA::RTORB_var rt_orb =
        RTCORBA::RTORB::_narrow (object.in ()
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get the RootPOA.
      object =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get the POA Manager.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Obtain priority bands to be used in this test from the file
      // specified by the user.
      RTCORBA::PriorityBands bands;
      result = get_priority_bands (bands);
      if (result != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error reading priority bands from file\n"),
                          result);

      // Create a thread-pool.
      CORBA::ULong stacksize = 0;
      CORBA::Boolean allow_request_buffering = 0;
      CORBA::ULong max_buffered_requests = 0;
      CORBA::ULong max_request_buffer_size = 0;
      CORBA::Boolean allow_borrowing = 0;
      CORBA::ULong static_threads = 1;
      CORBA::ULong dynamic_threads = 0;

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
      RTCORBA::ThreadpoolId threadpool_id =
        rt_orb->create_threadpool_with_lanes (stacksize,
                                              lanes,
                                              allow_borrowing,
                                              allow_request_buffering,
                                              max_buffered_requests,
                                              max_request_buffer_size
                                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

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
        rt_orb->create_priority_banded_connection_policy (false_bands
                                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create a thread-pool policy.
      poa_policy_list[1] =
        rt_orb->create_threadpool_policy (threadpool_id
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Try to create a POA with invalid policies.  Should throw an
      // exception.
      poa_creation_exception_test (root_poa.in (),
                                   poa_manager.in (),
                                   poa_policy_list
                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Two policies for the next POA.
      poa_policy_list.length (2);

      // Create a priority model policy.
      poa_policy_list[0] =
        rt_orb->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED,
                                              0
                                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create a thread-pool policy.
      poa_policy_list[1] =
        rt_orb->create_threadpool_policy (threadpool_id
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create POA with CLIENT_PROPAGATED priority model, with lanes
      // but no bands.
      PortableServer::POA_var client_propagated_poa =
        root_poa->create_POA ("client_propagated_poa",
                              poa_manager.in (),
                              poa_policy_list
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Three policies for the next POA.
      poa_policy_list.length (3);

      // Default POA priority comes from the 'middle' lane's priority.
      CORBA::Short poa_priority =
        lanes[lanes.length () / 2].lane_priority;

      // Create a priority model policy.
      poa_policy_list[0] =
        rt_orb->create_priority_model_policy (RTCORBA::SERVER_DECLARED,
                                              poa_priority
                                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create a bands policy.
      poa_policy_list[1] =
        rt_orb->create_priority_banded_connection_policy (bands
                                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create a thread-pool policy.
      poa_policy_list[2] =
        rt_orb->create_threadpool_policy (threadpool_id
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create POA with SERVER_DECLARED priority model, with bands
      // and lanes.
      PortableServer::POA_var server_declared_poa =
        root_poa->create_POA ("server_declared_poa",
                              poa_manager.in (),
                              poa_policy_list
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Test: Attempt to register an object with priority that
      // doesn't match lanes.  Should get BAD_PARAM exception.
      ACE_DEBUG ((LM_DEBUG,
                  "\n<---Test--->: Servant priority does not match lanes\n\n"));

      RTPortableServer::POA_var rt_server_declared_poa =
        RTPortableServer::POA::_narrow (server_declared_poa.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Activation with incorrect priority should fail.
      CORBA::Short wrong_priority = 10000;
      object_activation_exception_test (rt_server_declared_poa.in (),
                                        0,
                                        wrong_priority
                                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create first servant and register with <client_propagated_poa>.
      Test_i server_impl (orb.in (),
                          bands
                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      result = create_object (client_propagated_poa.in (),
                              orb.in (),
                              &server_impl,
                              ior_output_file1
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (result != 0)
        return result;

      // Create second servant and register with <server_declared_poa>.
      Test_i server_impl2 (orb.in (),
                           bands
                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      result = create_object (server_declared_poa.in (),
                              orb.in (),
                              &server_impl2,
                              ior_output_file2
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (result != 0)
        return result;

      // Activate POA manager.
      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Run ORB.
      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Destroy ORB.
      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::INTERNAL, exception)
    {
      int minor_code =
        exception.minor ();

      if (ACE_BIT_ENABLED (minor_code, TAO_RTCORBA_THREAD_CREATION_LOCATION_CODE) &&
          errno == EPERM)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot create thread with scheduling policy %s\n"
                             "because the user does not have the appropriate privileges, terminating program....\n"
                             "Check svc.conf options and/or run as root\n",
                             sched_policy_name (orb->orb_core ()->orb_params ()->ace_sched_policy ())),
                            2);
        }
      else
        // Unexpected error.
        ACE_ASSERT (0);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unexpected exception caught in Banded_Connections test server:");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
