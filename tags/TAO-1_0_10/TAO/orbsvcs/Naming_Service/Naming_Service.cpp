// $Id$

#include "ace/Get_Opt.h"
#include "Naming_Service.h"

ACE_RCSID(Naming_Service, Naming_Service, "$Id$")

  // Default Constructor.

TAO_Naming_Service::TAO_Naming_Service (void)
  : ior_output_file_ (0),
    pid_file_name_ (0),
    context_size_ (ACE_DEFAULT_MAP_SIZE),
    persistence_file_name_ (0),
    time_ (0)
{
}

// Constructor taking command-line arguments.

TAO_Naming_Service::TAO_Naming_Service (int argc,
                                        char* argv[])
  : ior_output_file_ (0),
    pid_file_name_ (0),
    context_size_ (ACE_DEFAULT_MAP_SIZE),
    persistence_file_name_ (0),
    time_ (0)
{
  this->init (argc, argv);
}

int
TAO_Naming_Service::parse_args (int argc,
                                char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "do:p:s:t:f:");
  int c;
  int size, time;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;
      case 'o': // outputs the naming service ior to a file.
        this->ior_output_file_ =
          ACE_OS::fopen (get_opts.optarg, "w");

        if (this->ior_output_file_ == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open %s for writing: %p\n",
                             get_opts.optarg), -1);
        break;
      case 'p':
        this->pid_file_name_ = get_opts.optarg;
        break;
      case 'f':
        this->persistence_file_name_ = get_opts.optarg;
        break;
      case 's':
        size = ACE_OS::atoi (get_opts.optarg);
        if (size >= 0)
          this->context_size_ = size;
        break;
      case 't':
         time = ACE_OS::atoi (get_opts.optarg);
        if (time >= 0)
          this->time_ = time;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-NScontextname <contextname> "
                           "-o <ior_output_file> "
                           "-p <pid_file_name> "
                           "-f <persistence_file_name> "
                           "\n",
                           argv [0]),
                          -1);
      }
  return 0;
}

// Initialize the state of the TAO_Naming_Service object
int
TAO_Naming_Service::init (int argc,
                          char *argv[])
{
  int result;
  CORBA::ORB_var orb;
  PortableServer::POA_var child_poa;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->orb_ = CORBA::ORB_init (argc, argv, 0, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Get the POA from the ORB.
      CORBA::Object_var poa_object =
        this->orb_->resolve_initial_references ("RootPOA", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT (" (%P|%t) Unable to initialize the POA.\n")),
                          -1);

      // Get the POA object.
      this->root_poa_ = PortableServer::POA::_narrow (poa_object.in (),
                                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Get the POA_Manager.
      PortableServer::POAManager_var poa_manager = 
        this->root_poa_->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::PolicyList policies (2);
      policies.length (2);

      // Id Assignment policy
      policies[0] =
        this->root_poa_->create_id_assignment_policy (PortableServer::USER_ID,
                                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Lifespan policy
      policies[1] =
        this->root_poa_->create_lifespan_policy (PortableServer::PERSISTENT,
                                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // We use a different POA, otherwise the user would have to change
      // the object key each time it invokes the server.

      this->ns_poa_ = this->root_poa_->create_POA ("NameService",
                                                   poa_manager.in (),
                                                   policies,
                                                   ACE_TRY_ENV);
      // Warning!  If create_POA fails, then the policies won't be
      // destroyed and there will be hell to pay in memory leaks!
      ACE_TRY_CHECK;

      // Creation of the new POAs over, so destroy the Policy_ptr's.
      for (CORBA::ULong i = 0;
           i < policies.length ();
           ++i)
        {
          CORBA::Policy_ptr policy = policies[i];
          policy->destroy (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }

      // Check the non-ORB arguments.  this needs to come before we
      // initialize my_naming_server so that we can pass on some of
      // the command-line arguments.

      result = this->parse_args (argc, argv);
      if (result < 0)
        return result;

      result = this->my_naming_server_.init (this->orb_.in (),
                                             this->ns_poa_.in (),
                                             context_size_,
                                             0,
                                             0,
                                             persistence_file_name_);
      if (result == -1)
        return result;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_Naming_Service::init");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  if (this->ior_output_file_ != 0)
    {
      CORBA::String_var str =
        this->my_naming_server_.naming_service_ior ();
      ACE_OS::fprintf (this->ior_output_file_,
                       "%s",
                       str.in ());
      ACE_OS::fclose (this->ior_output_file_);
    }

  if (this->pid_file_name_ != 0)
    {
      FILE *pidf = fopen (this->pid_file_name_, "w");
      if (pidf != 0)
        {
          ACE_OS::fprintf (pidf,
                           "%d\n",
                           ACE_OS::getpid ());
          ACE_OS::fclose (pidf);
        }
    }
  return 0;
}

// Run the ORB event loop.

int
TAO_Naming_Service::run (CORBA_Environment& ACE_TRY_ENV)
{
  int result;

  if (time_ == 0)
    {
      result = this->orb_->run (ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);
    }
  else
    {
      ACE_Time_Value tv (time_);
      result = this->orb_->run (tv, ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);
    }

  return result;
}

// Destructor.

TAO_Naming_Service::~TAO_Naming_Service (void)
{
  ACE_TRY_NEW_ENV
    {
      this->root_poa_->destroy (1, 1, ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // Ignore
    }
  ACE_ENDTRY;
}
