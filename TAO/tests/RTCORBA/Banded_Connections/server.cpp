// $Id$

#include "testS.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "tao/RTCORBAC.h"
#include "tao/Pool_Per_Endpoint.h"

#include "tao/Strategies/advanced_resource.h"

#if (TAO_HAS_RT_CORBA == 1)

class Test_i : public POA_Test
{
  // = TITLE
  //   An implementation for the Test interface in test.idl
  //
public:
  Test_i (CORBA::ORB_ptr orb,
          RTCORBA::PriorityBands &bands);
  // ctor

  void test_method (CORBA::Short priority,
                    CORBA::Environment&)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (CORBA::Environment&)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;
  // The ORB
  RTCORBA::PriorityBands &bands_;
  // Priority bands.
};

Test_i::Test_i (CORBA::ORB_ptr orb,
                RTCORBA::PriorityBands & bands)
  :  orb_ (CORBA::ORB::_duplicate (orb)),
     bands_ (bands)
{
}

void
Test_i::test_method (CORBA::Short priority,
                     CORBA::Environment& ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Use RTCurrent to find out the CORBA priority of the current
  // thread.

  CORBA::Object_var obj =
    this->orb_->resolve_initial_references ("RTCurrent", ACE_TRY_ENV);
  ACE_CHECK;

  RTCORBA::Current_var current =
    RTCORBA::Current::_narrow (obj.in (), ACE_TRY_ENV);
  ACE_CHECK;

  if (CORBA::is_nil (obj.in ()))
    ACE_THROW (CORBA::INTERNAL ());

  CORBA::Short servant_thread_priority =
    current->the_priority (ACE_TRY_ENV);
  ACE_CHECK;

  // Print out the info.
  int index = -1;
  ACE_DEBUG ((LM_DEBUG,
              "\nObject bands: \n"));
  for (CORBA::ULong i = 0; i < this->bands_.length (); ++i)
    {      
      ACE_DEBUG ((LM_DEBUG,
                  "%d) %d  %d\n", 
                  (i + 1), this->bands_[i].low, this->bands_[i].high));

      if (priority <= this->bands_[i].high
          && priority >= this->bands_[i].low
          && servant_thread_priority <= this->bands_[i].high
          && servant_thread_priority >= this->bands_[i].low)
        index = i + 1;
    }

  ACE_DEBUG ((LM_DEBUG,
              "Object priority: %d  "
              "Servant thread priority: %d\n",
              priority, servant_thread_priority));

  if (index == -1)
    ACE_DEBUG ((LM_DEBUG,
                "ERROR: object and thread priorities do not"
                "match the same band.\n"));
  else
    ACE_DEBUG ((LM_DEBUG,
                "Band %d was used for this invocation\n", index));
}

void
Test_i::shutdown (CORBA::Environment& ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0, ACE_TRY_ENV);
}

//*************************************************************************

const char *bands_file = "bands";
const char *ior_output_file1 = "test1.ior";
const char *ior_output_file2 = "test2.ior";
CORBA::Short poa_priority = -1;
CORBA::Short wrong_priority = -1;

// Parse command-line arguments.
int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "p:w:b:o:n:");
  int c, result;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'n':
        ior_output_file1 = get_opts.optarg;
        break;

      case 'o':
        ior_output_file2 = get_opts.optarg;
        break;

      case 'b':
        bands_file = get_opts.optarg;
        break;

      case 'p':
        result = ::sscanf (get_opts.optarg,
                           "%hd",
                           &poa_priority);
        if (result == 0 || result == EOF)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to process <-a> option"),
                            -1);
        break;

      case 'w':
        result = ::sscanf (get_opts.optarg,
                           "%hd",
                           &wrong_priority);
        if (result == 0 || result == EOF)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to process <-c> option"),
                            -1);
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-n <iorfile1> "
                           "-o <iorfile2> "
                           "-b <bands_file> "
                           "-p <poa_priority> "
                           "-w <wrong_priority> "
                           "\n",
                           argv [0]),
                          -1);
      }

  if (poa_priority < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Valid poa priority must be"
                       " specified.\nSee README file for more info\n"),
                      -1);

  return 0;
}

int
get_priority_bands (RTCORBA::PriorityBands &bands)
{
  FILE* file = ACE_OS::fopen (bands_file, "r");

  if (file == 0)
    return -1;

  ACE_Read_Buffer reader (file, 1);

  char* string = reader.read (EOF, ' ', '\0');

  if (string == 0)
    return -1;

  CORBA::ULong bands_length = (reader.replaced () + 1) / 2;
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
  PortableServer::ObjectId_var id;

  id = poa->activate_object (server_impl, ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  CORBA::Object_var server =
    poa->id_to_reference (id.in (), ACE_TRY_ENV);
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

void
object_activation_exception_test (RTPortableServer::POA_ptr poa,
                                  Test_i *server_impl,
                                  CORBA::Short priority,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      // Register with poa.
      PortableServer::ObjectId_var id =
        poa->activate_object_with_priority (server_impl,
                                            priority,
                                            ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Object_var server =
        poa->id_to_reference (id.in (),
                              ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Test failed: no exception caught\n"));
    }
  ACE_CATCH (CORBA::BAD_PARAM, ex)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "BAD_PARAM exception is caught as expected.\n"));
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG, "Test failed: unexpected exception caught\n"));
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

void
poa_creation_exception_test (PortableServer::POA_ptr root_poa,
                             PortableServer::POAManager_ptr manager,
                             CORBA::PolicyList &policies,
                             CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      PortableServer::POA_var child_poa =
        root_poa->create_POA ("Child_POA",
                              manager,
                              policies,
                              ACE_TRY_ENV);
      ACE_TRY_CHECK;
      ACE_DEBUG ((LM_DEBUG, "Test failed: no exception caught\n"));
    }
  ACE_CATCH (PortableServer::POA::InvalidPolicy, ex)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "InvalidPolicy exception is caught as expected.\n"));
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG, "Test failed: unexpected exception\n"));
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
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

      // RTORB.
      CORBA::Object_var object =
        orb->resolve_initial_references ("RTORB", ACE_TRY_ENV);
      ACE_TRY_CHECK;
      RTCORBA::RTORB_var rt_orb = RTCORBA::RTORB::_narrow (object.in (),
                                                           ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (check_for_nil (rt_orb.in (), "RTORB") == -1)
        return 1;

      // RootPOA.
      object =
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

      // Obtain priority bands to be used in this test from the file
      // specified by the user.
      RTCORBA::PriorityBands bands;
      if (get_priority_bands (bands) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error reading priority bands from file\n"),
                          1);

      // Servant.
      Test_i server_impl (orb.in (), bands);
      Test_i server_impl2 (orb.in (), bands);


      // Test 1: Attempt to create a POA with priority bands but
      // without the priority model.  Should get POA::InvalidPolicy
      // exception. 
      ACE_DEBUG ((LM_DEBUG,
                  "\n     Test 1\n"));

      CORBA::PolicyList poa_policy_list;
      poa_policy_list.length (1);
      poa_policy_list[0] =
        rt_orb->create_priority_banded_connection_policy 
        (bands,
         ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      poa_creation_exception_test (root_poa.in (),
                                   poa_manager.in (),
                                   poa_policy_list,
                                   ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Test 2: Attempt to create a POA with priority bands that do
      // not match the resources (i.e., endpoints/lanes).  Should get 
      // POA::InvalidPolicy exception. 
      ACE_DEBUG ((LM_DEBUG,
                  "\n     Test 2\n"));
      
      RTCORBA::PriorityBands false_bands;
      false_bands.length (2);
      false_bands[0].low = 10000;
      false_bands[0].high = 10005;
      false_bands[1].low = 26;
      false_bands[1].high = 30;

      poa_policy_list[0] =
        rt_orb->create_priority_banded_connection_policy 
        (false_bands,
         ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      poa_creation_exception_test (root_poa.in (),
                                   poa_manager.in (),
                                   poa_policy_list,
                                   ACE_TRY_ENV);
      ACE_TRY_CHECK;      

      // Create POA with CLIENT_PROPAGATED priority model, no bands.
      poa_policy_list[0] =
        rt_orb->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED,
                                              0,
                                              ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      PortableServer::POA_var client_propagated_poa =
        root_poa->create_POA ("client_propagated_poa",
                              poa_manager.in (),
                              poa_policy_list,
                              ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Create POA with SERVER_DECLARED priority model, with bands.
      poa_policy_list.length (2);
      poa_policy_list[0] =
        rt_orb->create_priority_model_policy (RTCORBA::SERVER_DECLARED,
                                              poa_priority,
                                              ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      poa_policy_list[1] =
        rt_orb->create_priority_banded_connection_policy 
        (bands,
         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POA_var server_declared_poa =
        root_poa->create_POA ("server_declared_poa",
                              poa_manager.in (),
                              poa_policy_list,
                              ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Test 3: Attempt to register an object with priority that
      // doesn't match POA resources (i.e., endpoints/lanes).  Should
      // get BAD_PARAM exception.
      ACE_DEBUG ((LM_DEBUG,
                  "\n     Test 3\n"));

      RTPortableServer::POA_var rt_server_declared_poa =
        RTPortableServer::POA::_narrow (server_declared_poa.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (check_for_nil (rt_server_declared_poa.in (), "RTPOA") == -1)
        return 1;

      object_activation_exception_test (rt_server_declared_poa.in (), 
                                        &server_impl, 
                                        wrong_priority,
                                        ACE_TRY_ENV);
      ACE_TRY_CHECK;


      // Create object 1 and register with <client_propagated_poa>.
      int result;
      ACE_DEBUG ((LM_DEBUG, "\nActivated object one as "));
      result = create_object (client_propagated_poa.in (), 
                              orb.in (), 
                              &server_impl,
                              ior_output_file1, 
                              ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (result == -1)
        return 1;

      // Create object 2 and register with <server_declared_poa>.
      ACE_DEBUG ((LM_DEBUG, "\nActivated object two as "));
      result = create_object (server_declared_poa.in (), 
                              orb.in (), 
                              &server_impl2,
                              ior_output_file2, 
                              ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (result == -1)
        return 1;

      // Activate POA manager.
      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Start ORB event loop.
      // @@ Currently we are using Reactor per priority to emulate
      // threadpool with lanes.  Once POA threadpools are implemented,
      // this code should be replaced with standard threadpool apis.
      TAO_Pool_Per_Endpoint pool (orb.in ());
      pool.run (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "\nServer ORB event loop finished\n\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
      "Unexpected exception caught in Banded_Connections test server:");
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
