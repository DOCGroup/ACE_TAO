// $Id$

#include "ace/Get_Opt.h"
#include "Notify_Service.h"

Notify_Service::Notify_Service (void)
  : notify_factory_name_ ("NotifyEventChannelFactory")
{
  // No-Op.
}

Notify_Service::~Notify_Service (void)
{
  // No-Op.
}

void
Notify_Service::init_ORB  (int& argc, char *argv [],
                             CORBA::Environment &ACE_TRY_ENV)
{
  this->orb_ = CORBA::ORB_init (argc,
                                argv,
                                "",
                                ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::Object_var poa_object  =
    this->orb_->resolve_initial_references("RootPOA",
                                           ACE_TRY_ENV);
  ACE_CHECK;

  this->poa_ =
    PortableServer::POA::_narrow (poa_object.in (),
                                  ACE_TRY_ENV);
  ACE_CHECK;

  PortableServer::POAManager_var poa_manager =
    this->poa_->the_POAManager (ACE_TRY_ENV);
  ACE_CHECK;

  poa_manager->activate (ACE_TRY_ENV);
  ACE_CHECK;
}

void
Notify_Service::startup (int argc, char *argv[],
                          CORBA::Environment &ACE_TRY_ENV)
{
  ACE_DEBUG ((LM_DEBUG,
              "\nStarting up the Notification Service...\n"));

  // initalize the ORB.
  this->init_ORB (argc, argv,
                  ACE_TRY_ENV);
  ACE_CHECK;

  // Resolve the naming service.
  this->resolve_naming_service (ACE_TRY_ENV);
  ACE_CHECK;

  // Activate the factory
  obj
    = this->notify_factory_.get_ref (ACE_TRY_ENV);
  ACE_CHECK;
  ACE_ASSERT (!CORBA::is_nil (obj.in ()));

  CORBA::String_var str =
    this->orb_->object_to_string (obj.in (), ACE_TRY_ENV);

  ACE_DEBUG ((LM_DEBUG,
              "The Notification Event Channel Factory IOR is <%s>\n",
              str.in ()));

  // Register the Factory
  ACE_ASSERT(!CORBA::is_nil (this->naming_.in ()));

  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup (this->notify_factory_name_);

  this->naming_->rebind (name,
                         obj.in (),
                         ACE_TRY_ENV);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "Registered with the naming service as: %s\n",
              this->notify_factory_name_));
}

void
Notify_Service::resolve_naming_service (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Object_var naming_obj =
    this->orb_->resolve_initial_references ("NameService",
                                            ACE_TRY_ENV);
  ACE_CHECK;

  // Need to check return value for errors.
  if (CORBA::is_nil (naming_obj.in ()))
    ACE_THROW (CORBA::UNKNOWN ());

  this->naming_ =
    CosNaming::NamingContext::_narrow (naming_obj.in (),
                                       ACE_TRY_ENV);
  ACE_CHECK;
}

int
Notify_Service::run (void)
{
  ACE_DEBUG ((LM_DEBUG, "%s: Running the Notification Service\n",
              __FILE__));
  if (this->orb_->run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "run"), 1);

  return 0;
}

void
Notify_Service::shutdown (CORBA::Environment &ACE_TRY_ENV)
{
  // Deactivate.
  PortableServer::ObjectId_var oid =
    this->poa_->servant_to_id (&this->notify_factory_,
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
      service.startup (argc,
                       argv,
                       ACE_TRY_ENV);
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
