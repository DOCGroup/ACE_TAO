// $Id$

#include "Container_Base.h"
#include "Swapping_Container.h"
#include "ace/DLL.h"
#include "tao/Utils/PolicyList_Destroyer.h"
#include "ace/OS_NS_stdio.h"
#include "Dynamic_Component_Activator.h"

#if !defined (__ACE_INLINE__)
# include "Swapping_Container.inl"
#endif /* __ACE_INLINE__ */

namespace CIAO
{

  ACE_Atomic_Op <ACE_SYNCH_MUTEX, long>
  Swapping_Container::serial_number_ (0);

  Swapping_Container::Swapping_Container (CORBA::ORB_ptr o)
  : Container (o),
    number_ (0)
  {
  }

  Swapping_Container::~Swapping_Container ()
  {
  }

  int
  Swapping_Container::init (const char *name,
                            const CORBA::PolicyList *more_policies
                            ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    char buffer[MAXPATHLEN];

    if (name == 0)
      {
        this->number_ = ++Swapping_Container::serial_number_;
        ACE_OS::sprintf (buffer, "CIAO::Swapping_Container-%ld",
                         this->number_);
        name = buffer;
      }

    CORBA::Object_var poa_object =
      this->orb_->resolve_initial_references("RootPOA"
                                             ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);

    if (CORBA::is_nil (poa_object.in ()))
      ACE_ERROR_RETURN ((LM_ERROR,
                         " (%P|%t) Unable to initialize the POA.\n"),
                        -1);

    PortableServer::POA_var root_poa =
      PortableServer::POA::_narrow (poa_object.in ()
                                    ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);

    this->create_component_POA (name,
                                more_policies,
                                root_poa.in ()
                                ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);

    PortableServer::POAManager_var poa_manager =
      root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);

    poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);

    return 0;
  }

  void
  Swapping_Container::create_component_POA (const char *name,
                                            const CORBA::PolicyList *p,
                                            PortableServer::POA_ptr root
                                            ACE_ENV_ARG_DECL)
  {
    CORBA::PolicyList policies (0);

    if (p != 0)
      policies = *p;

    PortableServer::POAManager_var poa_manager =
      root->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    CORBA::ULong policy_length = policies.length ();
    policies.length (policy_length + 1);

    policies[policy_length] =
      root->create_id_assignment_policy (PortableServer::USER_ID
                                         ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    policies.length (policy_length + 1);
    policies[policy_length] =
     root->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER
                                              ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    // Servant Retention Policy
    policies.length (policy_length + 1);
    policies[policy_length] =
      root->create_servant_retention_policy (PortableServer::RETAIN
                                             ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    this->component_poa_ =
      root->create_POA (name,
                        poa_manager.in (),
                        policies
                        ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    ACE_NEW_THROW_EX (this->dsa_,
                      Dynamic_Component_Activator (this->orb_.in ()),
                      CORBA::NO_MEMORY ());

    this->component_poa_->set_servant_manager (
        this->dsa_
        ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }
}
