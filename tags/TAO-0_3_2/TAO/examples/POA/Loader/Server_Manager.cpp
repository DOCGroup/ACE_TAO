// $Id$

#include "Server_Manager.h"

ACE_RCSID(Loader, Server_Manager, "$Id$")

Server_i::Server_i(void)
    : ior_output_file_ (0),
      policies_ (4)
{
}

Server_i::~Server_i(void)
{
}

// This method parses the input.

int
Server_i::parse_args (int argc, 
                      char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "f:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'f':
        ior_output_file_ = get_opts.optarg;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "[-f ior_output_file] "
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

// The IORs obtained are written into files for further use.

int
Server_i::write_iors_to_file (const char *first_ior,
                              const char *second_ior)
{
  if (ior_output_file_ == 0)
    // No filename was specified; simply return
    return 0;

  char ior_output_file_1[BUFSIZ];
  char ior_output_file_2[BUFSIZ];

  ACE_OS::sprintf (ior_output_file_1,
                   "%s_1",
                   ior_output_file_);
  ACE_OS::sprintf (ior_output_file_2,
                   "%s_2",
                   ior_output_file_);

  FILE *output_file_1 = ACE_OS::fopen (ior_output_file_1,
                                       "w");
  FILE *output_file_2 = ACE_OS::fopen (ior_output_file_2,
                                       "w");
  if (output_file_1 == 0 
      || output_file_2 == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot open output files for writing IORs: %s, %s\n",
                       ior_output_file_1,
                       ior_output_file_2),
                      -1);

  int result = ACE_OS::fprintf (output_file_1,
				"%s",
				first_ior);
  if (result <= 0
      || ACE_static_cast (size_t,result) != ACE_OS::strlen (first_ior))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_OS::fprintf failed while writing %s to %s\n",
                       first_ior,
                       ior_output_file_1),
                      -1);

  result = ACE_OS::fprintf (output_file_2,
                            "%s",
                            second_ior);
  if (result <= 0
      || ACE_static_cast (size_t,result) != ACE_OS::strlen (second_ior))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_OS::fprintf failed while writing %s to %s\n",
                       second_ior,
                       ior_output_file_2),
                      -1);
  ACE_OS::fclose (output_file_1);
  ACE_OS::fclose (output_file_2);
  return 0;
}

// Initialisation of the ORB and POA.

int
Server_i::init (int argc, char **argv)
{
  CORBA::Environment TAO_TRY_ENV;

  TAO_TRY
    {
      // Initialize the ORB.
      orb_ = CORBA::ORB_init (argc,
                              argv,
                              0,
                              TAO_TRY_ENV); 
      TAO_CHECK_ENV;

      int result = parse_args (argc, argv);
      if (result != 0)
        return result;

      // Get an Object reference to RootPOA.
      CORBA::Object_var obj =
        orb_->resolve_initial_references ("RootPOA");

      // Narrow the Object reference to a POA reference
      root_poa_ = PortableServer::POA::_narrow (obj.in (),
                                                TAO_TRY_ENV);  
      TAO_CHECK_ENV;
  
      // Get the POAManager of RootPOA
      poa_manager_ = root_poa_->the_POAManager (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    
    }
  TAO_CATCHANY 
    {
      TAO_TRY_ENV.print_exception ("Server_i:init_poa ()");
      return 1;
    }
  TAO_ENDTRY;
  
  return 0;
}

// This method creates an poa with 4 policies of which the servent
// retention policy decides whether the Servant Activator or the
// Servant Locator would be used by the Servant Manager.

PortableServer::POA_ptr
Server_i::create_poa (const char *name,
                      int servant_retention_policy)
{
  CORBA::Environment TAO_TRY_ENV;
  PortableServer::POA_ptr my_poa = 0;
 
  TAO_TRY
    {  
      policies_.length (4);
      
      // ID Assignment Policy.
      policies_[0] =
        root_poa_->create_id_assignment_policy 
        (PortableServer::USER_ID,
         TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Lifespan Policy.
      policies_[1] =
        root_poa_->create_lifespan_policy 
        (PortableServer::PERSISTENT,
         TAO_TRY_ENV);
      TAO_CHECK_ENV;
  
      // Request Processing Policy.
      policies_[2] =
        root_poa_->create_request_processing_policy 
        (PortableServer::USE_SERVANT_MANAGER,
         TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      // Servant Retention Policy.
      if (servant_retention_policy == 1)
      policies_[3] =
        root_poa_->create_servant_retention_policy 
        (PortableServer::RETAIN,
         TAO_TRY_ENV);
      
      if (servant_retention_policy == 0)
      policies_[3] =
        root_poa_->create_servant_retention_policy 
        (PortableServer::NON_RETAIN,
         TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Create myPOA as the child of RootPOA with the above
      // policies_. myPOA will use SERVANT_ACTIVATOR or
      // SERVANT_LOCATOR depending upon the servant retention policy
      // being RETAIN or NONRETAIN respectively.
      my_poa = root_poa_->create_POA (name,
                                      poa_manager_.in (),
                                      policies_,
                                      TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      // Destroy the policy objects as they have been passed to
      // create_POA and no longer needed.
      for (CORBA::ULong i = 0;
           i < policies_.length () && TAO_TRY_ENV.exception () == 0;
           ++i)
        {
          CORBA::Policy_ptr policy = policies_[i];
          policy->destroy (TAO_TRY_ENV);
        }
    }
  TAO_CATCHANY 
    {
      TAO_TRY_ENV.print_exception ("Server_i:create_poa ()");
      return 0;
    }
  TAO_ENDTRY;  

  return my_poa;
}

// The Servant Activator object is created and initialised.

int
Server_i::create_activator (PortableServer::POA_var first_poa)
{
  CORBA::Environment TAO_TRY_ENV;
   
  TAO_TRY
    { 
      // An Servant Activator object is created which will activate
      // the servant on demand.
      ACE_NEW_RETURN (servant_activator_impl_,
                      ServantActivator_i (orb_.in (),
                                          "Generic_Servant",
                                          "supply_servant",
                                          "destroy_servant"),
                      0);
      
      PortableServer::ServantActivator_var servant_activator =
        servant_activator_impl_->_this (TAO_TRY_ENV);
      TAO_CHECK_ENV;
 
      // Set ServantActivator_i object as the servant_manager of
      // firstPOA.
      first_poa->set_servant_manager (servant_activator.in (),
                                      TAO_TRY_ENV);
      TAO_CHECK_ENV;
  
      // Create a reference with user created ID in firstPOA which
      // uses the MyFooServantActivator. 
      PortableServer::ObjectId_var first_foo_oid =
        PortableServer::string_to_ObjectId ("firstFoo");
                                                      
      first_foo_ = first_poa->create_reference_with_id (first_foo_oid.in (),
                                                        "IDL:Foo:1.0",
                                                        TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY 
    {
      TAO_TRY_ENV.print_exception ("Server_i:create_activator ()");
      return 1;
    }
  TAO_ENDTRY;  

  return 0;
}

// The Servant Locator object is created and initialised.

int
Server_i::create_locator (PortableServer::POA_var second_poa)
{
  CORBA::Environment TAO_TRY_ENV;
   
  TAO_TRY
    { 
      // An Servant Locator object is created which will activate 
      // the servant on demand.
      ACE_NEW_RETURN (servant_locator_impl_,
                      ServantLocator_i (orb_.in (),
                                        "Generic_Servant",
                                        "supply_servant",
                                        "destroy_servant"),
                      0);
      
      PortableServer::ServantLocator_var servant_locator =
        servant_locator_impl_->_this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Set ServantLocator_i object as the servant Manager of
      // secondPOA.

      second_poa->set_servant_manager (servant_locator.in (),
                                       TAO_TRY_ENV);
      TAO_CHECK_ENV;
        
      // Try to create a reference with user created ID in second_poa
      // which uses MyFooServantLocator. 
      PortableServer::ObjectId_var second_foo_oid =
        PortableServer::string_to_ObjectId ("secondFoo");

      second_foo_ = 
        second_poa->create_reference_with_id (second_foo_oid.in (),
                                              "IDL:Foo:1.0",
                                              TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY 
    {
      TAO_TRY_ENV.print_exception ("Server_i:create_locator ()");
      return 1;
    }
  TAO_ENDTRY;   

  return 0;
}

// The execution process of the server.

int 
Server_i::run (void) 
{
  CORBA::Environment TAO_TRY_ENV;
   
  TAO_TRY
    { 
      // Invoke object_to_string on the references created in firstPOA
      // and secondPOA.

      CORBA::String_var first_foo_ior =
        orb_->object_to_string (first_foo_.in (),
                               TAO_TRY_ENV);
      TAO_CHECK_ENV;
 
      CORBA::String_var second_foo_ior =
        orb_->object_to_string (second_foo_.in (),
                               TAO_TRY_ENV);
      TAO_CHECK_ENV;
 
      // Print the ior's of first_foo and second_foo.

      ACE_DEBUG ((LM_DEBUG,"%s\n%s\n",
                  first_foo_ior.in (),
                  second_foo_ior.in ()));

      int write_result = 
        this->write_iors_to_file (first_foo_ior.in (),
                                  second_foo_ior.in ());
      if (write_result != 0)
        return write_result;

      // Set the poa_manager state to active, ready to process
      // requests.
      poa_manager_->activate (TAO_TRY_ENV);
     
      TAO_CHECK_ENV;
  
      // Run the ORB.
      if (orb_->run () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "CORBA::ORB::run"),
                          -1);
       
      // Destroy the root_poa_ and also first_poa and second_poa.
      root_poa_->destroy (1,
                          1,
                          TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY 
    {
      TAO_TRY_ENV.print_exception ("Server_i:run ()");
      return 1;
    }
  TAO_ENDTRY;
  
  return 0;
}

