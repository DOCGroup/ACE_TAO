// $Id$

// ========================================================================
// = LIBRARY
//    TAO/tests/POA/Forwarding
//
// = FILENAME
//    server.cpp
//
// = DESCRIPTION
//
// = AUTHOR
//    Irfan Pyarali and Michael Kircher
// ========================================================================

#include "tao/corba.h"
#include "ace/Read_Buffer.h"
#include "ace/Get_Opt.h"
#include "MyFooServant.h"

ACE_RCSID(Forwarding, server, "$Id$")

static char *first_foo_forward_to_IOR_ = 0;

static char *second_foo_forward_to_IOR_ = 0;

static FILE *first_foo_ior_output_file_;

static FILE *second_foo_ior_output_file_;

static MyFirstFooServant *myFirstFooServant_ptr;
static MyFooServantLocator *myFooServantLocator_ptr;


static int
read_ior (char *filename,
          const u_int foo_number)
{
  // Open the file for reading.
  ACE_HANDLE f_handle_ = ACE_OS::open (filename,0);
  
  if (f_handle_ == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to open %s for writing: %p\n",
                       filename),
                       -1);

  ACE_Read_Buffer ior_buffer (f_handle_);

  if (foo_number == 1)
    {
      first_foo_forward_to_IOR_ = ior_buffer.read ();

      if (first_foo_forward_to_IOR_ == 0)
	ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to allocate memory to read ior: %p\n"),
			  -1);  
    }
  else if (foo_number == 2)
    {
      second_foo_forward_to_IOR_ = ior_buffer.read ();

      if (second_foo_forward_to_IOR_ == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to allocate memory to read ior: %p\n"),
                          -1);
    }
  
  if (foo_number == 1)
    ACE_DEBUG ((LM_DEBUG,
                "POA approach: Read ior: %s\n",
                first_foo_forward_to_IOR_));  
  else if (foo_number == 2)
    ACE_DEBUG ((LM_DEBUG,
                "Locator approach: Read ior: %s\n",
                second_foo_forward_to_IOR_));  
  return 0;
}


static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "f:g:k:l:o:O:p:");
  int c;
  int result; 
  
  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'f': // read the IOR from the file.
        result = read_ior (get_opts.optarg,1);
        if (result < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
			     "Unable to read ior from %s : %p\n",
			     get_opts.optarg),
			    -1);
        break;
      case 'g': // read the IOR from the file.
        result = read_ior (get_opts.optarg,2);
        if (result < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
			     "Unable to read ior from %s : %p\n",
			     get_opts.optarg),
			    -1);
        break;
      case 'k':
        first_foo_forward_to_IOR_ = get_opts.optarg;
        break;
      case 'l':
        second_foo_forward_to_IOR_ = get_opts.optarg;
        break;
      case 'o': // output the IOR to a file.
        first_foo_ior_output_file_ = ACE_OS::fopen (get_opts.optarg, "w");
        if (first_foo_ior_output_file_ == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open %s for writing: %p\n",
                             get_opts.optarg), -1);
        break; 
      case 'p': // output the IOR to a file.
        second_foo_ior_output_file_ = ACE_OS::fopen (get_opts.optarg, "w");
        if (second_foo_ior_output_file_ == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open %s for writing: %p\n",
                             get_opts.optarg), -1);
        break; 
      case 'O': 
        break;
      case '?':  
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "\nusage:  %s \n"
                           "-f forward_to_IOR_file (first foo)\n"
                           "-g forward_to_IOR_file (second foo)\n"
                           "-k forward_to_IOR (first foo)\n"
                           "-l forward_to_IOR (second foo)\n"
                           "-o file_for_IOR (first foo)\n"
                           "-p file_for_IOR (second foo)\n"
                           "\n",
                           argv [0]),
			  -1);
      }
  
  // Indicates successful parsing of command line.
  return 0;
}

static void 
get_forward_reference (char *IOR,
                       CORBA::Object_var &forward_location_var,
                       CORBA::ORB_ptr orb_ptr,
                       CORBA::Environment &env)
{
  if (IOR != 0)
    {
      forward_location_var = orb_ptr->string_to_object (IOR, env);
            
      if (env.exception () != 0)
        {
          env.print_exception ("ORB::string_to_object");
          return;
        }
      
      if (CORBA::is_nil (forward_location_var.in ()))
        ACE_DEBUG ((LM_DEBUG,
                    "Error: Forward_to location is wrong\n"));
    }
}


// Documentation !!!!  first_POA will contain an object which will
// use the POA directly to do forwarding

int setup_first_poa (PortableServer::POA_ptr root_poa_ptr,
                     PortableServer::POAManager_ptr poa_manager_ptr, 
                     PortableServer::POA_var &first_poa_var)
{
  CORBA::Environment env;

  // Policies for the childPOA to be created.
  CORBA::PolicyList policies (2); 
  policies.length (2);

  // The next two policies are common to both
  
  // Id Assignment Policy
  policies[0] = root_poa_ptr->create_id_assignment_policy (PortableServer::USER_ID, env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_id_assignment_policy");
      return -1;
    }
  
  // Lifespan policy
  policies[1] = root_poa_ptr->create_lifespan_policy (PortableServer::PERSISTENT, env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_lifespan_policy");
      return -1;
    }

    
  ACE_CString name = "firstPOA";
    
  // Create firstPOA as the child of RootPOA with the above policies
  // firstPOA will use SERVANT_ACTIVATOR because of RETAIN policy.
  first_poa_var = root_poa_ptr->create_POA (name.c_str (),
                                            poa_manager_ptr,
                                            policies,
                                            env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_POA");
      return -1;
    }

  return 0;
}


// second_POA will contain an object which will use the
// Servant_Locator to do forwarding

int setup_second_poa (PortableServer::POA_ptr root_poa,
                      PortableServer::POAManager_ptr poa_manager_ptr, 
                      PortableServer::POA_var &second_poa_var)
{
  CORBA::Environment env;

  // Policies for the childPOA to be created.
  CORBA::PolicyList policies (4); 
  policies.length (4);

  // The next two policies are common to both
  
  // Id Assignment Policy
  policies[0] = root_poa->create_id_assignment_policy (PortableServer::USER_ID, env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_id_assignment_policy");
      return -1;
    }
  
  // Lifespan policy
  policies[1] = root_poa->create_lifespan_policy (PortableServer::PERSISTENT, env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_lifespan_policy");
      return -1;
    }
        
  // Tell the POA to use a servant manager
  policies[2] =
  root_poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER, env);
  if (env.exception () != 0)
    {
	    env.print_exception ("PortableServer::POA::create_request_processing_policy");
	    return -1;
    }
    
  // Servant Retention Policy -> Use a locator
  policies[3] =
  root_poa->create_servant_retention_policy (PortableServer::NON_RETAIN, env);
  if (env.exception () != 0)
    {
	    env.print_exception ("PortableServer::POA::create_servant_retention_policy");
	    return -1; 
    }
    
  ACE_CString name = "secondPOA";
    
  // Create secondPOA as child of RootPOA with the above policies
  // secondPOA will use a SERVANT_LOCATOR because of NON_RETAIN
  // policy.
  second_poa_var = root_poa->create_POA (name.c_str (),
	                      			       poa_manager_ptr,
				                             policies,
				                             env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_POA");
      return -1;
    }
  
  // Creation of childPOAs is over. Destroy the Policy objects.
  for (CORBA::ULong i = 0;
       i < policies.length () && env.exception () == 0;
       ++i)
    {
      CORBA::Policy_ptr policy = policies[i];
      policy->destroy (env);
    }

  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::destroy");
      return -1;
    }
  return 0;
}

int create_first_servant (PortableServer::POA_ptr first_poa_ptr,
                          CORBA::ORB_ptr orb_ptr)
{
  CORBA::Environment env;

  // Get the forward_to reference to feed it into object
  // implementations
  CORBA::Object_var first_foo_forward_to_var;

  get_forward_reference (first_foo_forward_to_IOR_,
                         first_foo_forward_to_var,
                         orb_ptr,
                         env);

  if (env.exception () != 0)
    {
      env.print_exception ("get_forward_reference");
      return -1;
    }

  ACE_NEW_RETURN (myFirstFooServant_ptr, 
                  MyFirstFooServant (orb_ptr,
                                     first_poa_ptr, 
                                     27,
                                     first_foo_forward_to_var.in ()),
                  -1);

  // Create ObjectId and use that ObjectId to activate the
  // first_foo_impl object.
  PortableServer::ObjectId_var first_foo_oid_var =
    PortableServer::string_to_ObjectId ("firstFoo");

  first_poa_ptr->activate_object_with_id (first_foo_oid_var.in (),
                                          myFirstFooServant_ptr,
                                          env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::activate_object_with_id");
      return -1;
    }
  
  // Get Object reference for first_foo_impl object.
  Foo_var first_foo_var = myFirstFooServant_ptr->_this (env);

  if (env.exception () != 0)
    {
      env.print_exception ("POA_Foo::_this");
      return -1;
    }
  
  // Stringyfy the object reference and print it out.
  CORBA::String_var first_foo_ior_var =
    orb_ptr->object_to_string (first_foo_var.in (), env);
  
  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::object_to_string");
      return -1;
    }
  
  ACE_DEBUG ((LM_DEBUG,
              "POA approach: Own IOR: %s\n",
              first_foo_ior_var.in ()));
  
  if (first_foo_ior_output_file_)
    {
      ACE_OS::fprintf (first_foo_ior_output_file_,
                       "%s",
                       first_foo_ior_var.in ());
      ACE_OS::fclose (first_foo_ior_output_file_);
      
      ACE_DEBUG ((LM_DEBUG, "POA approach: Wrote IOR to a file.\n"));
    }
  return 0;
}


int 
create_second_servant (PortableServer::POA_ptr second_poa_ptr,
                       CORBA::ORB_ptr orb_ptr)
{
  CORBA::Environment env;
  
  CORBA::Object_var second_foo_forward_to_var;
  get_forward_reference (second_foo_forward_to_IOR_,
                         second_foo_forward_to_var,
                         orb_ptr,
                         env);
  if (env.exception () != 0)
    {
      env.print_exception ("get_forward_reference");
      return -1;
    }

  // instantiate the servant locator and set it for the second child
  // POA The locator gets to know where to forward to
  
  ACE_NEW_RETURN (myFooServantLocator_ptr, 
                  MyFooServantLocator (orb_ptr, 
                                       second_foo_forward_to_var.in ()),
                  -1);
  PortableServer::ServantLocator_var servant_locator_var = 
    myFooServantLocator_ptr->_this (env);
  
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POAManager::_this");
      return -1;
    }
  
  // Set MyFooServantLocator object as the servant Manager of
  // secondPOA.

  second_poa_ptr->set_servant_manager (servant_locator_var.in (), env);
  
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POAManager::set_servant_manager");
      return -1;
    }
  
  // Create the second MyFooServant
  //            ======
  
  // Try to create a reference with user created ID in second_poa
  // which uses MyFooServantLocator.
  
  PortableServer::ObjectId_var second_foo_oid_var =
    PortableServer::string_to_ObjectId ("secondFoo");
  
  CORBA::Object_var second_foo_var =
    second_poa_ptr->create_reference_with_id (second_foo_oid_var.in (),
                                          "IDL:Foo:1.0", env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_reference_with_id");
      return -1;
    }
  
  // Invoke object_to_string on the references created in firstPOA and
  // secondPOA.

  CORBA::String_var second_foo_ior_var = 
    orb_ptr->object_to_string (second_foo_var.in (), env);
  
  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::object_to_string");
      return -1;
    }
  
  ACE_DEBUG ((LM_DEBUG,
              "Locator approach: Own IOR: %s\n",
              second_foo_ior_var.in ()));
  
  
  if (second_foo_ior_output_file_)
    {
      ACE_OS::fprintf (second_foo_ior_output_file_,
                       "%s",
                       second_foo_ior_var.in ());
      ACE_OS::fclose (second_foo_ior_output_file_);

      ACE_DEBUG ((LM_DEBUG, "Locator approach: Wrote IOR to a file.\n"));
    }
  
  return 0;
}

int
main (int argc, char **argv)
{
  // @@ Michael, this function is WAY too long!  Can you please break
  // it up into a number of subfunctions and put them into a class or
  // something?!  It's impossible to tell what's going on here!
  int result = parse_args (argc, argv);

  if (result == -1)
    return -1;

  CORBA::Environment env;
  
  // Initialize the ORB first.
  CORBA::ORB_var orb_var = CORBA::ORB_init (argc, argv, 0, env);
  
  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB_init");
      return -1;
    }

  // Obtain the RootPOA.
  CORBA::Object_var obj_var = orb_var->resolve_initial_references ("RootPOA");
  
  // Get the POA_var object from Object_var.
  PortableServer::POA_var root_poa_var =
    PortableServer::POA::_narrow (obj_var.in (), env);
  
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }
  
  // Get the POAManager of the RootPOA.
  PortableServer::POAManager_var poa_manager_var =
    root_poa_var->the_POAManager (env);
  
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::the_POAManager");
      return -1;
    }
  
  PortableServer::POA_var first_poa_var;
  PortableServer::POA_var second_poa_var;

  if (setup_first_poa (root_poa_var.in(), poa_manager_var.in(), first_poa_var) == -1)
    return -1; 
  if (setup_second_poa (root_poa_var.in(), poa_manager_var.in(), second_poa_var) == -1)
    return -1;  
    
  // Create the first MyFooServant
  //            =====
  if (create_first_servant (first_poa_var.in(), orb_var.in()) == -1)
    return -1;

  if (create_second_servant (second_poa_var.in(), orb_var.in()) == -1)
    return -1;    
  
  poa_manager_var->activate (env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POAManager::activate");
      return -1;
    }
  
  if (orb_var->run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "CORBA::ORB::run"),
                      -1);

  
  // Destroy RootPOA. (Also destroys childPOA)
  root_poa_var->destroy (1,
                         1,
                         env);
  delete myFirstFooServant_ptr;
  delete myFooServantLocator_ptr;

  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::destroy");
      return -1;
    }

  
  return 0;
}
