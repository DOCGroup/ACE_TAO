//$Id$
#include "Manager.h"
#include "Client_i.h"
#include "testC.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "tao/IORManipulation/IORManip_Loader.h"
#include "tao/PortableServer/PortableServer.h"
#include "orbsvcs/FaultTolerance/FT_Service_Activate.h"
#include "ace/streams.h"



// Files which have the IOR
const char *first_ior = 0;
const char *second_ior = 0;
const char *ior_output_file = 0;

// Objects
CORBA::Object_var object_primary = 0;
CORBA::Object_var object_secondary = 0;

// Reference to the IOR manipulator
TAO_IOP::TAO_IOR_Manipulation_ptr iorm = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "a:b:c:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'a':
        first_ior = get_opts.opt_arg ();
        break;
      case 'b':
        second_ior = get_opts.opt_arg ();
        break;
      case 'c':
        ior_output_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-a <iorfile>"
                           "-b <iorfile>"
                           "-c <output ior file>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}


int
main (int argc,
      char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;

  Manager manager;

  ACE_TRY
    {
      // Initilaize the ORB, POA etc.
      manager.init (argc,
                    argv
                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // the command line arguments
      if (parse_args (argc, argv) == -1)
        return -1;

      // Merge the different IORS
      manager.make_merged_iors (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      cout << "Merged IORS" <<endl;
      // Set properties. This is the most important portion of the
      // test
      manager.set_properties (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      cout << "Set prop" <<endl;

      // Write IOR to file
      manager.write_to_file ();

      // Client, who is going to use the merged IOR
      // Construct that with the managers ORB
      Client_i client_imp (manager.orb ());
      client_imp.init (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

Manager::Manager (void)
  :orb_ (0),
   merged_set_ (0)
{
  //no-op
}

void
Manager::init (int argc,
               char *argv[]
               ACE_ENV_ARG_DECL)
{
  this->orb_ = CORBA::ORB_init (argc,
                                argv,
                                0
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Obtain the RootPOA.
  CORBA::Object_var obj_var =
    this->orb_->resolve_initial_references ("RootPOA"
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Get the POA_var object from Object_var.
  PortableServer::POA_var root_poa_var =
    PortableServer::POA::_narrow (obj_var.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Get the POAManager of the RootPOA.
  PortableServer::POAManager_var poa_manager_var =
    root_poa_var->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  poa_manager_var->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

int
Manager::make_merged_iors (ACE_ENV_SINGLE_ARG_DECL)
{
  // First  server
  object_primary =
    this->orb_->string_to_object (first_ior
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  //Second server
  object_secondary =
    this->orb_->string_to_object (second_ior
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Get an object reference for the ORBs IORManipultion object!
  CORBA::Object_ptr IORM =
    this->orb_->resolve_initial_references (TAO_OBJID_IORMANIPULATION,
                                            0
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  iorm =
    TAO_IOP::TAO_IOR_Manipulation::_narrow (IORM ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);


  // Create the list
  TAO_IOP::TAO_IOR_Manipulation::IORList iors (2);
  iors.length(2);
  iors [0] = CORBA::Object::_duplicate (object_primary.in ());
  iors [1] = CORBA::Object::_duplicate (object_secondary.in ());

  // Create a merged set 1;
  merged_set_ =
    iorm->merge_iors (iors ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
Manager::set_properties (ACE_ENV_SINGLE_ARG_DECL)
{
  FT::TagFTGroupTaggedComponent ft_tag_component;

  // Property values

  // Major and Minor revision numbers
  ft_tag_component.version.major = (CORBA::Octet) 1;
  ft_tag_component.version.minor = (CORBA::Octet) 0;

  // Domain id
  const char *id = "iogr_testing";
  ft_tag_component.ft_domain_id = id;

  // Object group id
  ft_tag_component.object_group_id =
    (CORBA::ULongLong) 10;

  // Version
  ft_tag_component.object_group_ref_version =
    (CORBA::ULong) 5;

  // Construct the IOGR Property class
  TAO_FT_IOGR_Property iogr_prop (ft_tag_component);

  // Set the property
  CORBA::Boolean retval = iorm->set_property (&iogr_prop,
                                              this->merged_set_.in ()
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Set the primary
  // See we are setting the second ior as the primary
  if (retval != 0)
    {
      retval = iorm->set_primary (&iogr_prop,
                                  object_secondary.in (),
                                  this->merged_set_.in ()
                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
    }

  return 0;
}

int
Manager::run (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_TRY
    {
      this->orb_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_ERROR_RETURN ((LM_DEBUG,
                         "Error in run \n"),
                        -1);
    }
  ACE_ENDTRY;

  return 0;
}

int
Manager::write_to_file (void)
{
  //
  CORBA::String_var iorref =
    this->orb_->object_to_string (this->merged_set_.in ());

  if (ior_output_file != 0)
    {
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                          1);
      ACE_OS::fprintf (output_file, "%s", iorref.in ());
      ACE_OS::fclose (output_file);
    }

  return 0;
}

CORBA::ORB_ptr
Manager::orb (void)
{
  return this->orb_.in ();
}

Client_i::Client_i (CORBA::ORB_ptr orb)
  :orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
run_test (Simple_Server_ptr server
          ACE_ENV_ARG_DECL);

void
Client_i::init (ACE_ENV_SINGLE_ARG_DECL)
{
  // Open the file for reading.
  ACE_HANDLE f_handle = ACE_OS::open (ior_output_file,
                                      0);

  if (f_handle == ACE_INVALID_HANDLE)
    ACE_ERROR ((LM_ERROR,
                "Unable to open %s for writing: %p\n",
                ior_output_file));

  ACE_Read_Buffer ior_buffer (f_handle);

  char *data = ior_buffer.read ();

  if (data == 0)
    ACE_ERROR ((LM_ERROR,
                "Unable to read ior: %p\n"));


  int argc = 0;
  char **argv = 0;
  this->orb_ = CORBA::ORB_init (argc,
                                argv,
                                0
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Object_var object =
    this->orb_->string_to_object (data
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Combined IOR stuff
  Simple_Server_var server =
    Simple_Server::_narrow (object.in ()
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (server.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "Object reference <%s> is nil\n",
                  data));
    }

  run_test (server.in ()
            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ior_buffer.alloc ()->free (data);
  ACE_OS::close (f_handle);
}


void run_test (Simple_Server_ptr server
               ACE_ENV_ARG_DECL)
{
  // We do this twice as we know that there are only two servers.
  for (CORBA::ULong i = 0;
       i < 2;
       i++)
    {
      ACE_TRY
        {
          for (CORBA::ULong j = 0;
               j < 10;
               j++)
            {
              // Make a remote call
              server->remote_call (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("*********************************\n")));
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("I am going to shutdown the server\n")));
          server->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
          ACE_OS::sleep (2);
        }
      ACE_CATCH (CORBA::TRANSIENT, t)
        {
          if (t.completed () != CORBA::COMPLETED_NO)
            {
              ACE_PRINT_EXCEPTION (t, "Unexpected kind of TRANSIENT");
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("The completed status %d\n"), t.completed ()));
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("Automagically re-issuing request on TRANSIENT\n")));
              ACE_OS::sleep (1);
            }
        }
      ACE_CATCH (CORBA::COMM_FAILURE, f)
        {
          ACE_PRINT_EXCEPTION (f, "A (sort of) expected COMM_FAILURE");
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Automagically re-issuing request on COMM_FAILURE\n")));
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "Unexpected exception");
          ACE_RE_THROW;
        }
      ACE_ENDTRY;
      ACE_CHECK;
    }
}
