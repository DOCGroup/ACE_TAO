#include "ace/Get_Opt.h"
#include "Event_Logging_Service.h"
#include "orbsvcs/Log/EventLogFactory_i.h"
#include "orbsvcs/CosEvent/CEC_Default_Factory.h"

ACE_RCSID (Event_Logging_Service,
           Event_Logging_Service,
           "$Id$")


Event_Logging_Service::Event_Logging_Service (void)
  : event_log_factory_name_ ("EventLogFactory")
{
  // No-Op.
}

Event_Logging_Service::~Event_Logging_Service (void)
{
  // No-Op.
}

void
Event_Logging_Service::init_ORB  (int& argc, char *argv []
                             ACE_ENV_ARG_DECL)
{
  this->orb_ = CORBA::ORB_init (argc,
                                argv,
                                ""
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Object_var poa_object  =
    this->orb_->resolve_initial_references("RootPOA"
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->poa_ =
    PortableServer::POA::_narrow (poa_object.in ()
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::POAManager_var poa_manager =
    this->poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

int
Event_Logging_Service::startup (int argc, char *argv[]
                          ACE_ENV_ARG_DECL)
{
  ACE_DEBUG ((LM_DEBUG,
              "\nStarting up the Telecom EventLog Service...\n"));

  // initalize the ORB.
  this->init_ORB (argc, argv
                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Resolve the naming service.
  this->resolve_naming_service (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Activate the event log factory
  // CORBA::Object_var obj =
  DsEventLogAdmin::EventLogFactory_var obj =
    this->event_log_factory_.activate (this->poa_.in ()
                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  ACE_ASSERT (!CORBA::is_nil (obj.in ()));

  CORBA::String_var str =
    this->orb_->object_to_string (obj.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_DEBUG ((LM_DEBUG,
              "The Event Log Factory IOR is <%s>\n", str.in ()));

  // Register the Event Log Factory
  ACE_ASSERT(!CORBA::is_nil (this->naming_.in ()));

  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup (this->event_log_factory_name_);

  this->naming_->rebind (name,
                         obj.in ()
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_DEBUG ((LM_DEBUG,
              "Registered with the naming service as: %s\n",
              this->event_log_factory_name_));

  return 0;
}

void
Event_Logging_Service::resolve_naming_service (ACE_ENV_SINGLE_ARG_DECL)
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
Event_Logging_Service::run (void)
{
  ACE_DEBUG ((LM_DEBUG, "%s: Running the Telecom EventLog Service\n", __FILE__));

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->orb_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

void
Event_Logging_Service::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  // Deactivate.
  PortableServer::ObjectId_var oid =
    this->poa_->servant_to_id (&this->event_log_factory_
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // deactivate from the poa.
  this->poa_->deactivate_object (oid.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Unbind from the naming service.
  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup (this->event_log_factory_name_);

  this->naming_->unbind (name
                         ACE_ENV_ARG_PARAMETER);

  // shutdown the ORB.
  if (!CORBA::is_nil (this->orb_.in ()))
    this->orb_->shutdown ();
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  TAO_CEC_Default_Factory::init_svcs ();

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      Event_Logging_Service service;

      service.startup (argc,
                       argv
                       ACE_ENV_ARG_PARAMETER);

      ACE_TRY_CHECK;

      if (service.run () == -1)
        {
          service.shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);

          ACE_ERROR_RETURN ((LM_ERROR,
                             "Failed to run the Telecom Log Service.\n"),
                            1);

          ACE_TRY_CHECK;
        }

      service.shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // no -op
    }
  ACE_ENDTRY;

  return 0;
}
