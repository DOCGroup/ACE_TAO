#include "Notify_Logging_Service.h"

#include "ace/Dynamic_Service.h"
#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include "tao/debug.h"
#include "orbsvcs/Notify/Service.h"

ACE_RCSID (Notify_Logging_Service,
           Notify_Logging_Service,
           "$Id$")


Notify_Logging_Service::Notify_Logging_Service (void)
  : service_name_ (NOTIFY_KEY),
    ior_file_name_ (0),
    pid_file_name_ (0),
    bind_to_naming_service_ (1)
{
  // No-Op.
}

Notify_Logging_Service::~Notify_Logging_Service (void)
{
  // No-Op.
}

int
Notify_Logging_Service::init_ORB (int& argc, char *argv []
                              ACE_ENV_ARG_DECL)
{
  this->orb_ = CORBA::ORB_init (argc,
                                argv,
                                ""
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->notify_service_ = ACE_Dynamic_Service<TAO_Notify_Service>::instance (TAO_NOTIFY_DEF_EMO_FACTORY_NAME);

  if (this->notify_service_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Notify Service not found! check conf. file\n"));
      return -1;
    }

  CORBA::Object_var poa_obj  =
    this->orb_->resolve_initial_references("RootPOA"
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (poa_obj.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Unable to resolve the RootPOA.\n"),
                      -1);

  this->poa_ =
    PortableServer::POA::_narrow (poa_obj.in ()
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  PortableServer::POAManager_var poa_manager =
    this->poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
Notify_Logging_Service::parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_LIB_TEXT("n:o:p:x"));
  int opt;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'n':
          service_name_ = get_opt.opt_arg();
          break; 
 
        case 'o':
          ior_file_name_ = get_opt.opt_arg();
          break;
  
        case 'p':
          pid_file_name_ = get_opt.opt_arg();
          break;
 
        case 'x':
          bind_to_naming_service_ = 0;
          break;

        case '?':              
        default:
          ACE_DEBUG ((LM_DEBUG,
                      "Usage: %s "
                      "-n service_name "
                      "-o ior_file_name "
                      "-p pid_file_name "
                      "-x [disable naming service bind] "
                      "\n",
                      argv[0]));
          return -1;            
        }                       
    }
      
  return 0;
}

int
Notify_Logging_Service::init (int argc, char *argv[]
                          ACE_ENV_ARG_DECL)
{
  // initalize the ORB.
  if (this->init_ORB (argc, argv
                      ACE_ENV_ARG_PARAMETER) != 0)
  return -1;

  if (this->parse_args (argc, argv) == -1)
    return -1;

  this->notify_service_->init (this->orb_.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_DEBUG ((LM_DEBUG,
              "\nStarting up the Notification Logging Service...\n"));

  // Activate the factory
  this->notify_factory_ =
    notify_service_->create (this->poa_.in ()
                            ACE_ENV_ARG_PARAMETER);

  ACE_NEW_THROW_EX (this->notify_log_factory_,
                      TAO_NotifyLogFactory_i (this->notify_factory_.in ()),
                      CORBA::NO_MEMORY ());

  DsNotifyLogAdmin::NotifyLogFactory_var obj =
    notify_log_factory_->activate (this->poa_.in () ACE_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN (-1);

  CORBA::String_var str =
    this->orb_->object_to_string (obj.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_DEBUG ((LM_DEBUG,
              "The Notify Log Factory IOR is <%s>\n", str.in()));

  if (ior_file_name_ != 0) 
    {
      FILE* iorf = ACE_OS::fopen (ior_file_name_, ACE_LIB_TEXT("w"));
      if (iorf == 0) 
	{
	  ACE_ERROR_RETURN ((LM_ERROR,
			     "Cannot open output file for writing IOR: %s",
			     ior_file_name_),
			    -1);
	}

      ACE_OS::fprintf (iorf, "%s\n", str.in ());
      ACE_OS::fclose (iorf);
    }

  if (pid_file_name_ != 0)
    {
      FILE* pidf = ACE_OS::fopen (pid_file_name_, ACE_LIB_TEXT("w"));
      if (pidf != 0)
	{
	  ACE_OS::fprintf (pidf,
			   "%ld\n",
			   ACE_static_cast (long, ACE_OS::getpid ()));
	  ACE_OS::fclose (pidf);
	}
    }

  if (bind_to_naming_service_) 
    {
      // Resolve the naming service.
      resolve_naming_service (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      // Register the Factory
      ACE_ASSERT (!CORBA::is_nil (this->naming_.in ()));

      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup (this->service_name_);
      ACE_CHECK_RETURN (-1);

      this->naming_->rebind (name,
			     obj.in ()
			     ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      ACE_DEBUG ((LM_DEBUG,
		  "Registered with the naming service as: %s\n",
		  this->service_name_));
    }

  return 0;
}

void
Notify_Logging_Service::resolve_naming_service (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::Object_var naming_obj =
    this->orb_->resolve_initial_references ("NameService"
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Need to check return value for errors.
  if (CORBA::is_nil (naming_obj.in ()))
    ACE_THROW (CORBA::UNKNOWN ());

  this->naming_ =
    CosNaming::NamingContext::_narrow (naming_obj.in ()
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

int
Notify_Logging_Service::run ()
{
  if (TAO_debug_level > 0 )
    ACE_DEBUG ((LM_DEBUG, "%s: Running the Notification Logging Service\n",
                __FILE__));

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->orb_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "run"), -1);
    }
  ACE_ENDTRY;

  return 0;
}

void
Notify_Logging_Service::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  // Deactivate.
  PortableServer::ObjectId_var oid =
    this->poa_->reference_to_id (this->notify_factory_.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // deactivate from the poa.
  this->poa_->deactivate_object (oid.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (bind_to_naming_service_) 
    {
      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup (this->service_name_);
      ACE_CHECK;

      this->naming_->unbind (name
			     ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  // shutdown the ORB.
  if (!CORBA::is_nil (this->orb_.in ()))
    this->orb_->shutdown ();

}
