// $Id$

#include "Swapping_Container.h"
#include "ciao/Servant_Activator.h"
#include "ciao/Dynamic_Component_Activator.h"

#include "tao/Utils/PolicyList_Destroyer.h"

#if !defined (__ACE_INLINE__)
# include "Swapping_Container.inl"
#endif /* __ACE_INLINE__ */

namespace CIAO
{

  ACE_Atomic_Op <TAO_SYNCH_MUTEX, unsigned long>
  Swapping_Container::serial_number_ (0);

  Swapping_Container::Swapping_Container (CORBA::ORB_ptr o,
                                          Container_Impl *container_impl)
  : Session_Container (o, container_impl),
    number_ (0)
  {
  }

  Swapping_Container::~Swapping_Container ()
  {
  }

  PortableServer::POA_ptr
  Swapping_Container::the_home_servant_POA (void) const
  {
    return this->home_servant_poa_.in ();
  }

  CORBA::Object_ptr
  Swapping_Container::get_home_objref (PortableServer::Servant p)
  {
    return this->the_home_servant_POA ()->servant_to_reference (p);
  }

  void
  Swapping_Container::deactivate_facet (const PortableServer::ObjectId &oid)
  {
    this->the_facet_cons_POA ()->deactivate_object (oid);
  }

  int
  Swapping_Container::init (const char *name,
                            const CORBA::PolicyList *more_policies)
  {
    char buffer[MAXPATHLEN];

    if (name == 0)
      {
        this->number_ = ++Swapping_Container::serial_number_;
        ACE_OS::sprintf (buffer,
                         "CIAO::Swapping_Container-%ld",
                         this->number_);
        name = buffer;
      }

    CORBA::Object_var poa_object =
      this->orb_->resolve_initial_references("RootPOA");

    if (CORBA::is_nil (poa_object.in ()))
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          -1);
      }

    PortableServer::POA_var root_poa =
      PortableServer::POA::_narrow (poa_object.in ());

    this->create_servant_POA (name, more_policies, root_poa.in ());

    this->create_home_servant_POA ("home servant POA",
                                   more_policies,
                                   root_poa.in ());

    this->create_connections_POA (root_poa.in ());

    PortableServer::POAManager_var poa_manager = root_poa->the_POAManager ();

    poa_manager->activate ();

    return 0;
  }

  void
  Swapping_Container::add_servant_to_map (
    PortableServer::ObjectId &oid,
    Dynamic_Component_Servant_Base* servant)
  {
    this->dsa_->add_servant_to_map (oid, servant);
  }

  void
  Swapping_Container::delete_servant_from_map (
    PortableServer::ObjectId &oid)
  {
    this->dsa_->delete_servant_from_map (oid);
  }

  void
  Swapping_Container::create_home_servant_POA (
    const char *name,
    const CORBA::PolicyList *p,
    PortableServer::POA_ptr root)
  {
    CORBA::PolicyList policies (0);

    if (p != 0)
      {
        policies = *p;
      }

    PortableServer::POAManager_var poa_manager = root->the_POAManager ();

    this->home_servant_poa_ =
      root->create_POA (name, poa_manager.in (), policies);
  }

  void
  Swapping_Container::create_connections_POA (PortableServer::POA_ptr root)
  {
    PortableServer::POAManager_var poa_manager =
      root->the_POAManager ();

    TAO::Utils::PolicyList_Destroyer policies (3);
    policies.length (3);

    policies[0] =
      root->create_id_assignment_policy (PortableServer::USER_ID);

    policies[1] =
      root->create_request_processing_policy (
        PortableServer::USE_SERVANT_MANAGER);


    // Servant Retention Policy.
    policies[2] =
      root->create_servant_retention_policy (PortableServer::RETAIN);

    this->facet_cons_poa_ =
      root->create_POA ("facet_consumer_poa", poa_manager.in (), policies);

    ACE_NEW_THROW_EX (this->sa_,
                      Servant_Activator (this->orb_.in ()),
                      CORBA::NO_MEMORY ());

    this->facet_cons_poa_->set_servant_manager (this->sa_);
  }

  void
  Swapping_Container::create_servant_POA (const char *name,
                                          const CORBA::PolicyList *p,
                                          PortableServer::POA_ptr root)
  {
    // @@ Jai, see how this method more or less does things (like
    // setting policies)  like create_connections_POA (). Could you
    // please refactor them into a seperate method?
    CORBA::PolicyList policies (0);

    if (p != 0)
      {
        policies = *p;
      }

    PortableServer::POAManager_var poa_manager =
      root->the_POAManager ();

    CORBA::ULong policy_length = policies.length ();
    policies.length (policy_length + 1);
    policies[policy_length] =
      root->create_id_assignment_policy (PortableServer::USER_ID);

    policy_length = policies.length ();
    policies.length (policy_length + 1);
    policies[policy_length] =
     root->create_request_processing_policy (
       PortableServer::USE_SERVANT_MANAGER);

    policy_length = policies.length ();
    policies.length (policy_length + 1);
    policies[policy_length] =
      root->create_servant_retention_policy (PortableServer::RETAIN);

    this->component_poa_ = root->create_POA (name, poa_manager.in (), policies);

    ACE_NEW_THROW_EX (this->dsa_,
                      Dynamic_Component_Activator (this->orb_.in ()),
                      CORBA::NO_MEMORY ());

    this->component_poa_->set_servant_manager (this->dsa_);
  }

  void
  Swapping_Container::ciao_uninstall_home (Components::CCMHome_ptr homeref)
  {
    PortableServer::POA_ptr tmp = this->home_servant_poa_.in ();
    PortableServer::ObjectId_var oid =
      tmp->reference_to_id (homeref);

    tmp->deactivate_object (oid.in ());
  }

  CORBA::Object_ptr
  Swapping_Container::install_servant (PortableServer::Servant p,
                                       Container::OA_Type t)
  {
    PortableServer::POA_ptr tmp = 0;

    if (t == Container::Component)
      {
        tmp = this->home_servant_poa_.in ();
      }
    else
      {
        tmp = this->facet_cons_poa_.in ();
      }

    PortableServer::ObjectId_var oid = tmp->activate_object (p);

    CORBA::Object_var objref = tmp->id_to_reference (oid.in ());

    return objref._retn ();
  }
}
