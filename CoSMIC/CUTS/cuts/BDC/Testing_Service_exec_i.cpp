// $Id$

#include "Testing_Service_exec_i.h"
#include "CCM_Component_Registry.h"
#include <typeinfo>

namespace CUTS
{
  //
  // Testing_Service_exec_i
  //
  Testing_Service_exec_i::Testing_Service_exec_i (void)
  {
    CCM_Component_Registry * registry = 0;
    ACE_NEW_THROW_EX (registry,
                      CCM_Component_Registry (),
                      ::CORBA::NO_MEMORY ());

    CUTS_Testing_Service::registry (registry);
  }

  //
  // ~Testing_Service_exec_i
  //
  Testing_Service_exec_i::~Testing_Service_exec_i (void)
  {

  }

  //
  // register_component
  //
  ::CORBA::Long Testing_Service_exec_i::register_component (
    const ::CUTS::Component_Registration & creg
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::CUTS::Registration_Failed,
                     ::CUTS::Registration_Limit))
  {
    // Check for a NIL <agent> and print a warning message if there
    // is one. Usually this means there is something wrong w/ the
    // template code that activates the <agent>!!!
    if (::CORBA::is_nil (creg.agent.in ()))
    {
      ACE_ERROR ((LM_WARNING,
                  "[%M] -%T - component %s has NIL agent\n",
                  creg.name.in ()));
    }

    // Bind the IP-address to the hostname.
    long result = this->host_table().bind(creg.ipaddr.in (),
                                          creg.hostname.in());

    if (result == -1)
    {
      ACE_ERROR ((LM_WARNING,
                  "[%M] -%T - failed to bind %s to %s\n",
                  creg.ipaddr.in (),
                  creg.hostname.in ()));
    }

    // Get the correct implementation of the <registry_>.
    CCM_Component_Registry * registry = this->registry_i ();

    if (registry != 0)
    {
      result = registry->register_component (creg.name.in (),
                                             creg.agent.in ());

    }

    // Check the <result> of the registration. If the result is zero,
    // then we know the registration has failed.
    if (result == 0)
    {
      throw Registration_Failed ();
    }

    return result;
  }

  //
  // unregister_component
  //
  void Testing_Service_exec_i::unregister_component (
    const ::CUTS::Component_Registration & creg
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::CUTS::ID_Not_Found))
  {
    ACE_DEBUG ((LM_TRACE,
                "[%M] -%T - unregistering component %s\n",
                creg.name.in ()));

    CCM_Component_Registry * registry = this->registry_i ();

    if (registry != 0)
    {
      registry->reset_registration (creg.name.in ());
    }
  }

  //
  // registry_i
  //
  const CCM_Component_Registry *
    Testing_Service_exec_i::registry_i (void) const
  {
    try
    {
      return dynamic_cast <const CCM_Component_Registry *> (
        this->registry ());
    }
    catch (std::bad_alloc &)
    {
      ACE_ERROR ((LM_ERROR,
                  "[%M] -%T - failed to get registry\n"));
      return 0;
    }
  }

  //
  // registry_i
  //
  CCM_Component_Registry * Testing_Service_exec_i::registry_i (void)
  {
    try
    {
      return dynamic_cast <CCM_Component_Registry *> (
        this->registry ());
    }
    catch (std::bad_alloc &)
    {
      ACE_ERROR ((LM_ERROR,
                  "[%M] -%T - failed to get registry\n"));
      return 0;
    }
  }
}
