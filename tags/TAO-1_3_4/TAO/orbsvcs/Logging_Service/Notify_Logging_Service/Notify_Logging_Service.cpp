#include "Notify_Logging_Service.h"

#include "ace/Dynamic_Service.h"
#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include "tao/debug.h"
#include "tao/IORTable/IORTable.h"
#include "orbsvcs/Notify/Service.h"

ACE_RCSID (Notify_Logging_Service,
           Notify_Logging_Service,
           "$Id$")


Notify_Logging_Service::Notify_Logging_Service (void)
: notify_factory_name_ (NOTIFY_KEY)
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
Notify_Logging_Service::init (int argc, char *argv[]
                          ACE_ENV_ARG_DECL)
{
  // initalize the ORB.
  if (this->init_ORB (argc, argv
                      ACE_ENV_ARG_PARAMETER) != 0)
  return -1;

  this->notify_service_->init (this->orb_.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Resolve the naming service.
  resolve_naming_service (ACE_ENV_SINGLE_ARG_PARAMETER);
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

  // Register the Factory
  ACE_ASSERT (!CORBA::is_nil (this->naming_.in ()));

  CosNaming::Name_var name =
    this->naming_->to_name (this->notify_factory_name_.c_str ()
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);


  this->naming_->rebind (name.in (),
                         obj.in ()
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_DEBUG ((LM_DEBUG,
              "Registered with the naming service as: %s\n",
              this->notify_factory_name_.c_str()));

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
    CosNaming::NamingContextExt::_narrow (naming_obj.in ()
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

  CosNaming::Name_var name =
    this->naming_->to_name (this->notify_factory_name_.c_str ()
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->naming_->unbind (name.in ()
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // shutdown the ORB.
  if (!CORBA::is_nil (this->orb_.in ()))
    this->orb_->shutdown ();

}
