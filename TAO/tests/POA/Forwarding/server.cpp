// $Id$

// ===========================================================================================
// = LIBRARY
//    TAO/tests/POA/Forwarding
//
// = FILENAME
//    server.cpp
//
// = DESCRIPTION
//
// = AUTHOR
//    Irfan Pyarali
// ===========================================================================================

#include "tao/corba.h"
#include "ace/Read_Buffer.h"
#include "MyFooServant.h"

static char *forward_to_IOR_ = 0;
FILE* ior_output_file_;


int
read_ior (char *filename)
{
  // Open the file for reading.
  ACE_HANDLE f_handle_ = ACE_OS::open (filename,0);
  
  if (f_handle_ == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
    "Unable to open %s for writing: %p\n",
    filename),
    -1);
  ACE_Read_Buffer ior_buffer (f_handle_);
  forward_to_IOR_ = ior_buffer.read ();

  if (forward_to_IOR_ == 0)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Unable to allocate memory to read ior: %p\n"),
                        -1);

   ACE_DEBUG ((LM_DEBUG, "read ior from the file\n"));

   return 0;
}




static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "f:o:O:");
  int c;
  int result; 

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'f': // read the IOR from the file.
        result = read_ior (get_opts.optarg);
        if (result < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
          "Unable to read ior from %s : %p\n",
          get_opts.optarg),
          -1);
        break;
      case 'o': // output the IOR to a file.
        ior_output_file_ = ACE_OS::fopen (get_opts.optarg, "w");
        if (ior_output_file_ == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open %s for writing: %p\n",
                             get_opts.optarg), -1);
        break;  
      case '?':   
      default:
        break;
/*        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           "-f second_server_IOR_file"
                           "-o file_for_IOR"
                           "\n",
                           argv [0]),
                          -1);*/
      }

  // Indicates successful parsing of command line.
  return 0;
}

int
main (int argc, char **argv)
{
  int result = parse_args (argc, argv);
  if (result == -1)
    return -1;

  CORBA::Environment env;

  // Initialize the ORB first.
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, 0, env);

  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB_init");
      return -1;
    }

  // Obtain the RootPOA.
  CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");

  // Get the POA_var object from Object_var.
  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (obj.in (), env);

  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }

  // Get the POAManager of the RootPOA.
  PortableServer::POAManager_var poa_manager =
    root_poa->the_POAManager (env);

  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::the_POAManager");
      return -1;
    }

  // Policies for the childPOA to be created.
  CORBA::PolicyList policies (2); 
  policies.length (2);

  // Id Assignment Policy
  policies[0] =
    root_poa->create_id_assignment_policy (PortableServer::USER_ID, env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_id_assignment_policy");
      return -1;
    }

  // Lifespan policy
  policies[1] = 
    root_poa->create_lifespan_policy (PortableServer::PERSISTENT, env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_lifespan_policy");
      return -1;
    }

  // Create the childPOA under the RootPOA.
  ACE_CString name = "childPOA";
  PortableServer::POA_var child_poa =
    root_poa->create_POA (name.c_str (),
                          poa_manager.in (),
                          policies,
                          env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_POA");
      return -1;
    }

  // Creation of childPOA is over. Destroy the Policy objects.
  for (CORBA::ULong i = 0;
       i < policies.length () && env.exception () == 0;
       ++i)
    {
      CORBA::Policy_ptr policy = policies[i];
      policy->destroy (env);
    }
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_POA");
      return -1;
    }

  
  CORBA::Object_var forward_to;

  if (forward_to_IOR_ != 0)
    {
      forward_to = orb->string_to_object (forward_to_IOR_, env);

      ACE_DEBUG ((LM_DEBUG,"\nFound IOR is:<%s>\n",forward_to_IOR_));

      
      if (env.exception () != 0)
        {
          env.print_exception ("ORB::string_to_object");
          return -1;
        }

      if (CORBA::is_nil(forward_to.in()))
      {
          ACE_DEBUG ((LM_DEBUG,"Forward_to location is wrong\n"));
      }
      else
      {
          ACE_DEBUG ((LM_DEBUG,"Forward_to location is ok\n"));
      }
    }

  // Create MyFooServant
  MyFooServant foo_impl (child_poa.in (), 
                         27,
                         forward_to.in ());

  // Create ObjectId and use that ObjectId to activate the foo_impl
  // object.
  //
  // Operation Used :
  //  void activate_object_with_id( in ObjectId oid, in Servant p_servant)
  //       raises (ObjectAlreadyActive, ServantAlreadyActive, WrongPolicy);
  PortableServer::ObjectId_var oid =
    PortableServer::string_to_ObjectId ("Foo");

  child_poa->activate_object_with_id (oid.in (),
                                      &foo_impl,
                                      env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::activate_object_with_id");
      return -1;
    }

  // Get Object reference for foo_impl object.
  Foo_var foo = foo_impl._this (env);
  if (env.exception () != 0)
    {
      env.print_exception ("POA_Foo::_this");
      return -1;
    }

  // Stringyfy the object reference and print it out.
  CORBA::String_var ior =
    orb->object_to_string (foo.in (), env);

  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::object_to_string");
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG,
              "The IOR is: <%s>\n",
              ior.in ()));

  if (ior_output_file_)
    {
      ACE_OS::fprintf (ior_output_file_,
                       "%s",
                       ior.in ());
      ACE_OS::fclose (ior_output_file_);

      ACE_DEBUG ((LM_DEBUG, "wrote ior to the file\n"));
    }

  
  poa_manager->activate (env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POAManager::activate");
      return -1;
    }

  if (orb->run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "CORBA::ORB::run"), -1);

  // Destroy RootPOA. (Also destroys childPOA)
  root_poa->destroy (CORBA::B_TRUE,
                     CORBA::B_TRUE,
                     env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::destroy");
      return -1;
    }

  return 0;
}
