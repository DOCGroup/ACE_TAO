// $Id$

#include "Notify_Service.h"
#include "orbsvcs/Notify/Notify_EventChannelFactory_i.h"
#include "ace/Get_Opt.h"

Notify_Service::Notify_Service (void)
  : notify_factory_name_ ("NotifyEventChannelFactory")
{
  // No-Op.
}

Notify_Service::~Notify_Service (void)
{
  // No-Op.
}

int
Notify_Service::init_ORB  (int& argc, char *argv [],
                             CORBA::Environment &ACE_TRY_ENV)
{
  this->orb_ = CORBA::ORB_init (argc,
                                argv,
                                "",
                                ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  CORBA::Object_var poa_obj  =
    this->orb_->resolve_initial_references("RootPOA",
                                           ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (poa_obj.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Unable to resolve the RootPOA.\n"),
                      -1);

  this->poa_ =
    PortableServer::POA::_narrow (poa_obj.in (),
                                  ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  PortableServer::POAManager_var poa_manager =
    this->poa_->the_POAManager (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  poa_manager->activate (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
Notify_Service::startup (int argc, char *argv[],
                          CORBA::Environment &ACE_TRY_ENV)
{
  ACE_DEBUG ((LM_DEBUG,
              "\nStarting up the Notification Service...\n"));

  // initalize the ORB.
  if (this->init_ORB (argc, argv,
                      ACE_TRY_ENV) != 0)
  return -1;

  ACE_CHECK_RETURN (-1);

  // Resolve the naming service.
  if (this->resolve_naming_service (ACE_TRY_ENV) != 0)
    return -1;

  ACE_CHECK_RETURN (-1);

  // Activate the factory
  this->notify_factory_ =
    TAO_Notify_EventChannelFactory_i::create (this->poa_.in (),
                                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  ACE_ASSERT (!CORBA::is_nil (this->notify_factory_.in ()));

  CORBA::String_var str =
    this->orb_->object_to_string (this->notify_factory_.in (), ACE_TRY_ENV);

  ACE_DEBUG ((LM_DEBUG,
              "The Notification Event Channel Factory IOR is <%s>\n",
              str.in ()));

  // Register the Factory
  ACE_ASSERT(!CORBA::is_nil (this->naming_.in ()));

  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup (this->notify_factory_name_);

  this->naming_->rebind (name,
                         this->notify_factory_.in (),
                         ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  ACE_DEBUG ((LM_DEBUG,
              "Registered with the naming service as: %s\n",
              this->notify_factory_name_));

  return 0;
}

int
Notify_Service::resolve_naming_service (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Object_var naming_obj =
    this->orb_->resolve_initial_references ("NameService",
                                            ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  // Need to check return value for errors.
  if (CORBA::is_nil (naming_obj.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Unable to resolve the Naming Service.\n"),
                      -1);

  this->naming_ =
    CosNaming::NamingContext::_narrow (naming_obj.in (),
                                       ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
Notify_Service::run (void)
{
  ACE_DEBUG ((LM_DEBUG, "%s: Running the Notification Service\n",
              __FILE__));
  if (this->orb_->run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "run"), -1);

  return 0;
}

void
Notify_Service::shutdown (CORBA::Environment &ACE_TRY_ENV)
{
  // Deactivate.
  PortableServer::ObjectId_var oid =
    this->poa_->reference_to_id (this->notify_factory_.in (),
                                 ACE_TRY_ENV);
  ACE_CHECK;

  // deactivate from the poa.
  this->poa_->deactivate_object (oid.in (),
                                 ACE_TRY_ENV);
  ACE_CHECK;

  // Unbind from the naming service.
  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup (this->notify_factory_name_);

  this->naming_->unbind (name,
                         ACE_TRY_ENV);

  // shutdown the ORB.
  if (!CORBA::is_nil (this->orb_.in ()))
    this->orb_->shutdown ();
}

int
main (int argc, char *argv[])
{
  Notify_Service service;

  ACE_TRY_NEW_ENV
    {
      if (service.startup (argc,
                           argv,
                           ACE_TRY_ENV) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Failed to start the Notification Service.\n"),
                          1);
      ACE_TRY_CHECK;

      if (service.run () == -1)
        {
          service.shutdown ();
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Failed to run the Notification Service.\n"),
                            1);
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Failed to start the Notification Service\n");
      return 1;
    }
  ACE_ENDTRY;

  service.shutdown ();

  return 0;
}
