// $Id$

#include "Testing_Service_Ex_exec_i.h"
#include "cuts/svcs/dbase/DBase_Service.h"

namespace CUTS
{
  //
  // Testing_Service_Ex_exec_i
  //
  Testing_Service_Ex_exec_i::Testing_Service_Ex_exec_i (
    CUTS_Database_Service & database_service)
    : database_service_ (database_service)
  {

  }

  //
  // ~Testing_Service_Ex_exec_i
  //
  Testing_Service_Ex_exec_i::~Testing_Service_Ex_exec_i (void)
  {

  }

  //
  // register_component
  //
  ::CORBA::Long
    Testing_Service_Ex_exec_i::register_component (
      const ::CUTS::Component_Registration & creg
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException,
                       ::CUTS::Registration_Failed,
                       ::CUTS::Registration_Limit))
  {
    // Insert the IP-address -> hostname mapping into the database
    // before continuing w/ the registration of the component.
    int hostid = 0;
    this->database_service_.register_host (creg.ipaddr,
                                           creg.hostname.in (),
                                           &hostid);

    long regid = Testing_Service_exec_i::register_component (creg);

    if (regid != 0)
    {
      // We need to register the component with the database
      // service since the registration was successful.
      if (this->database_service_.register_component (regid,
                                                      creg.name.in ()))
      {
        this->database_service_.set_component_uptime (regid, hostid);
      }
      else
      {
        ACE_ERROR ((LM_ERROR,
                    "[%M] -%T - failed to register %s with database\n",
                    creg.name.in ()));
      }
    }

    return regid;
  }

  //
  // unregister_component
  //
  void Testing_Service_Ex_exec_i::unregister_component (
    const ::CUTS::Component_Registration & creg
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::CUTS::ID_Not_Found))
  {
    // Set the <downtime> for the component.
    long instance_id = 0;
    if (this->database_service_.get_instance_id (creg.name.in (),
                                                 instance_id))
    {
      this->database_service_.set_component_downtime (instance_id);
    }

    // Pass control to the base class the super class to finish
    // the unregistration process.
    Testing_Service_exec_i::unregister_component (creg);
  }
}
