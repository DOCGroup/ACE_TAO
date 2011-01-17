// $Id$

#ifndef CIAO_CONTAINERBASE_IMPL_T_C
#define CIAO_CONTAINERBASE_IMPL_T_C

#include "ciao/Logger/Log_Macros.h"
#include "tao/Utils/PolicyList_Destroyer.h"
#include "ciao/Containers/Servant_Activator.h"

namespace CIAO
{
  template <typename BASE>
  Container_i<BASE>::Container_i (CORBA::ORB_ptr o,
                                  PortableServer::POA_ptr root_poa)
    : orb_ (::CORBA::ORB::_duplicate (o)),
      root_poa_ (::PortableServer::POA::_duplicate (root_poa)),
      sa_ (0)
  {
  }

  template <typename BASE>
  Container_i<BASE>::~Container_i (void)
  {
  }

template <typename BASE>
  void
  Container_i<BASE>::fini (void)
  {
    this->sa_ = ::CIAO::Servant_Activator::_nil ();

    if (! CORBA::is_nil (this->component_poa_.in ()))
      {
        this->component_poa_->destroy (1, 1);
        this->component_poa_ = ::PortableServer::POA::_nil ();
      }

    if (! CORBA::is_nil (this->facet_cons_poa_.in ()))
      {
        this->facet_cons_poa_->destroy (1, 1);
        this->facet_cons_poa_ = ::PortableServer::POA::_nil ();
      }

    this->root_poa_ = ::PortableServer::POA::_nil ();
    this->orb_ = ::CORBA::ORB::_nil ();
  }

  template <typename BASE>
  void
  Container_i<BASE>::init (const char* name)
  {
    CIAO_TRACE ("Container_i::init");

    CIAO_DEBUG (9,
                (LM_TRACE,
                 CLINFO
                 "Container_i::init - "
                 "Initializing a container with name <%C>\n",
                 name));

    if (CORBA::is_nil (this->root_poa_.in ()))
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "CIAO::Container_i: Unable "
                     "to initialize the POA.\n"));

        throw Components::CreateFailure ();
      }

    ACE_CString component_poa_name (name);
    component_poa_name += ":Component_POA";
    this->create_component_POA (
      component_poa_name.c_str (), this->root_poa_.in ());

    ACE_CString port_poa_name (name);
    port_poa_name += ":Port_POA";
    this->create_facet_consumer_POA (
      port_poa_name.c_str (), this->root_poa_.in ());
  }

  template <typename BASE>
  CORBA::Object_ptr
  Container_i<BASE>::resolve_service_reference(const char *service_id)
  {
    if (ACE_OS::strcmp (service_id, "POA") == 0)
      {
        return ::PortableServer::POA::_duplicate (this->component_poa_.in ());
      }
    throw Components::CCMException (Components::OBJECT_NOT_FOUND);
  }

  template <typename BASE>
  PortableServer::POA_ptr
  Container_i<BASE>::the_POA (void)
  {
    return this->component_poa_.in ();
  }

  template <typename BASE>
  PortableServer::POA_ptr
  Container_i<BASE>::the_port_POA (void)
  {
    return this->facet_cons_poa_.in ();
  }

  template <typename BASE>
  CORBA::ORB_ptr
  Container_i<BASE>::the_ORB (void) const
  {
    return this->orb_.in ();
  }

  template <typename BASE>
  void
  Container_i<BASE>::create_component_POA (const char *name,
                                           PortableServer::POA_ptr root)
  {
    CIAO_TRACE ("Container_i::create_component_POA");

    PortableServer::POAManager_var poa_manager =
      root->the_POAManager ();

    CORBA::PolicyList policies;
    this->component_poa_ =
      root->create_POA (name, poa_manager.in (), policies);
  }

  template <typename BASE>
  void
  Container_i<BASE>::create_facet_consumer_POA (const char *name,
                                                PortableServer::POA_ptr root)
  {
    CIAO_TRACE ("Container_i::create_facet_consumer_POA");

    PortableServer::POAManager_var poa_manager = root->the_POAManager ();

    TAO::Utils::PolicyList_Destroyer policies (3);
    policies.length (3);

    policies[0] =
      root->create_id_assignment_policy (PortableServer::USER_ID);

    // Servant Manager Policy
    policies[1] =
      root->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER);

    // Servant Retention Policy
    policies[2] =
      root->create_servant_retention_policy (PortableServer::RETAIN);

    this->facet_cons_poa_ =
      root->create_POA (name,
                        poa_manager.in (),
                        policies);

    Servant_Activator_i *sa = 0;
    ACE_NEW_THROW_EX (sa,
                      Servant_Activator_i (this->orb_.in ()),
                      CORBA::NO_MEMORY ());
    this->sa_ = sa;

    this->facet_cons_poa_->set_servant_manager (this->sa_.in ());
  }
}

#endif /* CIAO_CONTAINERBASE_IMPL_T_C */

