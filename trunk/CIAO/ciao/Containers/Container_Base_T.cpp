// $Id$

#ifndef CIAO_CONTAINERBASE_IMPL_T_C
#define CIAO_CONTAINERBASE_IMPL_T_C

#include "ciao/Logger/Log_Macros.h"
#include "tao/Utils/PolicyList_Destroyer.h"
#include "ciao/Containers/Servant_Activator.h"
#include "ciao/Servants/Connector_Servant_Impl_Base.h"
#include "ciao/Base/CIAO_ExceptionsC.h"

#include <sstream>

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
  Container_i<BASE>::install_servant (PortableServer::Servant p,
                                      Container_Types::OA_Type t,
                                      PortableServer::ObjectId_out oid)
  {
    CIAO_TRACE ("Container_i::install_servant");

    PortableServer::POA_ptr tmp = PortableServer::POA::_nil();

    if (t == Container_Types::COMPONENT_t ||
        t == Container_Types::HOME_t)
      {
        tmp = this->component_poa_.in ();
      }
    else
      {
        tmp = this->facet_cons_poa_.in ();
      }

    PortableServer::ObjectId_var tmp_id = tmp->activate_object (p);
    CORBA::Object_var objref = tmp->id_to_reference (tmp_id.in ());
    oid = tmp_id._retn ();

    return objref._retn ();
  }

  template <typename BASE>
  void
  Container_i<BASE>::uninstall (CORBA::Object_ptr objref,
                                Container_Types::OA_Type y)
  {
    CIAO_TRACE ("Container_i::uninstall");

    PortableServer::ServantBase_var svnt;

    switch (y)
      {
        case Container_Types::COMPONENT_t:
        case Container_Types::HOME_t:
          svnt = this->component_poa_->reference_to_servant (objref);
          break;
        default:
          svnt = this->facet_cons_poa_->reference_to_servant (objref);
          break;
      }

    PortableServer::ObjectId_var oid;
    this->uninstall_servant (svnt.in (), y, oid.out ());
  }

  template <typename BASE>
  void
  Container_i<BASE>::prepare_installation (const char *entity,
                                           const char *primary_artifact,
                                           const char *entry_point,
                                           const char *servant_artifact,
                                           const char *servant_entrypoint,
                                           const char *name,
                                           ACE_DLL &executor_dll,
                                           ACE_DLL &servant_dll)
  {
    CIAO_TRACE ("Container_i::prepare_installation");

    CIAO_DEBUG (6,
                (LM_DEBUG,
                  CLINFO
                  "Container_i::prepare_installation <%C> - "
                  "Loading %C [%C] from shared libraries\n",
                  entity, entity, name));

    CIAO_DEBUG (6,
                (LM_DEBUG,
                  CLINFO
                  "Container_i::prepare_installation <%C> - "
                  "Executor library [%C] with entrypoint [%C]\n",
                  entity,
                  primary_artifact,
                  entry_point));

    CIAO_DEBUG (6,
                (LM_DEBUG,
                  CLINFO
                  "Container_i::prepare_installation <%C> - "
                  "Servant library [%C] with entrypoint [%C]\n",
                  entity,
                  servant_artifact,
                  servant_entrypoint));

    if (!primary_artifact)
      {
        std::ostringstream err;
        err << "Component [" << name << "] has a nil component executor DLL name." ;

        CIAO_ERROR (1,
                    (LM_ERROR,
                      CLINFO
                      "Container_i::prepare_installation <%C> - "
                     "ERROR: %C\n",
		     entity,
                     err.str ().c_str ()));

        throw CIAO::Installation_Failure (name, err.str ().c_str ());
      }

    if (!servant_artifact)
      {
        std::ostringstream err;
        err << "Component [" << name << "] has a nil component servant DLL name." ;

        CIAO_ERROR (1,
                    (LM_ERROR,
                      CLINFO
                      "Container_i::prepare_installation <%C> - "
                     "ERROR: %C\n",
		     entity,
                     err.str ().c_str ()));

        throw CIAO::Installation_Failure (name, err.str ().c_str ());
      }

    if (!entry_point)
      {
        std::ostringstream err;
        err << "Component [" << name << "] has a nil executor entrypoint." ;

        CIAO_ERROR (1,
                    (LM_ERROR,
                      CLINFO
                      "Container_i::prepare_installation <%C> - "
                     "ERROR: %C\n",
		     entity,
                     err.str ().c_str ()));

        throw CIAO::Installation_Failure (name, err.str ().c_str ());
      }

    if (!servant_entrypoint)
      {
        std::ostringstream err;
        err << "Component [" << name << "] has a nil servant entrypoint." ;

        CIAO_ERROR (1,
                    (LM_ERROR,
                      CLINFO
                      "Container_i::prepare_installation <%C> - "
                     "ERROR: %C\n",
		     entity,
                     err.str ().c_str ()));

        throw CIAO::Installation_Failure (name, err.str ().c_str ());
      }

    if (executor_dll.open (ACE_TEXT_CHAR_TO_TCHAR (primary_artifact),
                           ACE_DEFAULT_SHLIB_MODE,
                           false) != 0)
      {
        std::ostringstream err;
        const ACE_TCHAR* error = executor_dll.error ();

        err << "Unable to open executor DLL for component [" << name
            << "]: " << error;

        CIAO_ERROR (1,
                    (LM_ERROR,
                      CLINFO
                      "Container_i::prepare_installation <%C> - "
                     "ERROR: %C\n",
		     entity,
                     err.str ().c_str ()));

        throw CIAO::Installation_Failure (name, err.str ().c_str ());
      }
    else
      {
        CIAO_DEBUG (9,
                    (LM_TRACE,
                      CLINFO
                      "Container_i::prepare_installation <%C> - "
                      "Executor DLL [%C] successfully opened\n",
                       entity,
                       primary_artifact));
      }

    if (servant_dll.open (ACE_TEXT_CHAR_TO_TCHAR (servant_artifact),
                          ACE_DEFAULT_SHLIB_MODE,
                          false) != 0)
      {
        std::ostringstream err;
        const ACE_TCHAR* error = servant_dll.error ();

        err << "Unable to open servant DLL for component [" << name
            << "]: " << error;

        CIAO_ERROR (1,
                    (LM_ERROR,
                      CLINFO
                      "Container_i::prepare_installation <%C> - "
                     "ERROR: %C\n",
		     entity,
                     err.str ().c_str ()));

        throw CIAO::Installation_Failure (name, err.str ().c_str ());
      }
    else
      {
        CIAO_DEBUG (9,
                    (LM_TRACE,
                      CLINFO
                      "Container_i::prepare_installation <%C> - "
                      "Servant DLL [%C] successfully openend\n",
                      entity,
                      servant_artifact));
      }
  }

  template <typename BASE>
  void
  Container_i<BASE>::uninstall_home (
    Components::CCMHome_ptr homeref)
  {
    CIAO_TRACE ("Container_i::uninstall_home");

    this->uninstall (homeref, Container_Types::HOME_t);
  }

  template <typename BASE>
  void
  Container_i<BASE>::uninstall_component (
    Components::CCMObject_ptr homeref)
  {
    CIAO_TRACE ("Container_i::uninstall_component");

    PortableServer::ServantBase_var srv_tmp =
      this->component_poa_->reference_to_servant (homeref);
    CIAO::Connector_Servant_Impl_Base * svnt =
      dynamic_cast <CIAO::Connector_Servant_Impl_Base *> (
        srv_tmp.in ());

    if (!svnt)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Container_i::uninstall_component - "
                     "Unable to convert provided servant "
                     "reference to servant implementation."));

        throw ::Components::RemoveFailure ();
      }
    else
      {
        svnt->remove ();
      }
  }

  template <typename BASE>
  void
  Container_i<BASE>::uninstall_servant (PortableServer::Servant svnt,
                                        Container_Types::OA_Type t,
                                        PortableServer::ObjectId_out oid)
  {
    CIAO_TRACE ("Container_i::uninstall_servant");

    PortableServer::POA_ptr tmp = PortableServer::POA::_nil();

    if ((t == Container_Types::COMPONENT_t) ||
        (t == Container_Types::HOME_t))
      {
        CIAO_DEBUG (9,
                    (LM_TRACE,
                     CLINFO
                     "Container_i::uninstall_servant - "
                     "Removing component or home servant\n"));

        tmp = this->component_poa_.in ();
      }
    else
      {
        CIAO_DEBUG (9,
                   (LM_TRACE,
                    CLINFO
                    "Container_i::uninstall_servant - "
                    "Removing facet or consumer servant\n"));

        tmp = this->facet_cons_poa_.in ();
      }

    try
      {
        PortableServer::ObjectId_var tmp_id = tmp->servant_to_id (svnt);
        tmp->deactivate_object (tmp_id);

        CIAO_DEBUG (9,
                    (LM_TRACE,
                     CLINFO
                     "Container_i::uninstall_servant - "
                     "Servant successfully removed, "
                     "reference count is %u\n",
                     svnt->_refcount_value () - 1));

        oid = tmp_id._retn ();
      }
    catch (const CORBA::Exception &ex)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Container_i::uninstall_servant - "
                     "Caught CORBA exception while "
                     "uninstalling servant: %C\n",
                     ex._info ().c_str ()));

        throw Components::RemoveFailure ();
      }
  }

  template <typename BASE>
  void
  Container_i<BASE>::activate_component (
    Components::CCMObject_ptr compref)
  {
    CIAO_TRACE("Container_i::activate_component");

    try
      {

        CIAO::Connector_Servant_Impl_Base * svt = 0;
        PortableServer::ServantBase_var servant_from_reference;

        try
          {
            servant_from_reference =
              this->component_poa_->reference_to_servant (compref);
            svt =
              dynamic_cast<CIAO::Connector_Servant_Impl_Base *> (
                servant_from_reference.in ());
          }
        catch (...)
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO
                            "Container_i::activate_component - "
                            "Caught unknown while retrieving servant\n"));
            throw CIAO::InvalidComponent ();
          }

        if (!svt)
          {
            throw CIAO::InvalidComponent  ();
          }
        else
          {
            CIAO_DEBUG (9,
                        (LM_TRACE,
                         CLINFO
                         "Container_i::activate_component - "
                         "Invoking CCM activate on provided "
                         "component object reference.\n"));

            svt->activate_component ();
          }
      }
    catch (const CIAO::InvalidComponent &)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Container_i::activate_component - "
                     "Failed to retrieve servant and/or cast "
                     "to servant pointer.\n"));
        throw;
      }
    catch (const CORBA::Exception &ex)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Container_i::activate_component - "
                     "Caught CORBA exception while activating "
                     "a component: %C\n",
                     ex._info ().c_str ()));
        throw;
      }
    catch (...)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Container_i::activate_component - "
                     "Caught unknown C++ exception while "
                     "activating a component.\n"));

        throw;
      }
  }

  template <typename BASE>
  void
  Container_i<BASE>::passivate_component (Components::CCMObject_ptr compref)
  {
    CIAO_TRACE ("Container_i::passivate_component");

    try
      {
        CIAO::Connector_Servant_Impl_Base * svt = 0;
        PortableServer::ServantBase_var servant_from_reference;

        try
          {
            servant_from_reference =
              this->component_poa_->reference_to_servant (compref);
            svt =
              dynamic_cast<CIAO::Connector_Servant_Impl_Base *> (
                servant_from_reference.in ());
          }
        catch (...)
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO
                            "Container_i::passivate_component - "
                            "Caught unknown while retrieving servant\n"));
            throw InvalidComponent ();
          }

        if (!svt)
          {
            throw CIAO::InvalidComponent  ();
          }
        else
          {
            CIAO_DEBUG (9,
                        (LM_TRACE,
                         CLINFO
                         "Container_i::passivate_component - "
                         "Invoking CCM passivate on provided "
                         "component object reference.\n"));

            svt->passivate_component ();
          }
      }
    catch (const CORBA::Exception &ex)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Container_i::passivate_component - "
                     "Caught CORBA exception while passivating "
                     "a component: %C\n",
                     ex._info ().c_str ()));

        throw;
      }
    catch (...)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Container_i::passivate_component - "
                     "Caught unknown C++ exception while "
                     "passivating a component.\n"));

        throw;
      }
  }

  template <typename BASE>
  void
  Container_i<BASE>::set_attributes (CORBA::Object_ptr compref,
                                     const ::Components::ConfigValues & values)
  {
    CIAO_TRACE("Container_i::set_attributes");

    try
      {
        PortableServer::ServantBase_var svt;

        try
          {
            svt = this->component_poa_->reference_to_servant (compref);
          }
        catch (CORBA::Exception &ex)
          {
            std::ostringstream err;
            err << "Internal Container Error: "
                << "Caught CORBA Exception while retrieving servant: "
                << ex._info ().c_str ();

            CIAO_ERROR (1, (LM_ERROR, CLINFO
                            "Container_i::set_attributes - %C\n",
                            err.str ().c_str ()));
            throw CIAO::InvalidComponent (0,
                                          err.str ().c_str ());
          }
        catch (...)
          {
            CIAO_ERROR (1, (LM_EMERGENCY, CLINFO
                            "Container_i::set_attributes - %C\n"
                            "Internal Container Error: Unknown C++ exception "
                            "while retrieving servant.\n"));
            throw CIAO::InvalidComponent (0,
                                          "Internal Container Error: Unknown C++ "
                                          "exception while retrieving servant.");
          }

        if (!svt)
          {
            CIAO_ERROR (1, (LM_EMERGENCY,  CLINFO
                            "Container_i::set_attributes - %C\n"
                            "Internal Container Error: "
                            "Invalid servant reference from reference_to_servant\n"));

            throw CIAO::InvalidComponent  (0,
                                           "Internal Container Error: "
                                           "Invalid servant reference from reference_to_servant");
          }
        else
          {
            CIAO::Connector_Servant_Impl_Base * comp = 0;
            CIAO::Home_Servant_Impl_Base *home = 0;

            if ((comp = dynamic_cast <CIAO::Connector_Servant_Impl_Base *> (svt.in ())))
              {
                CIAO_DEBUG (9,
                            (LM_TRACE,
                             CLINFO
                             "Container_i::set_attributes - "
                             "Configuring attribute values on "
                             "component object reference.\n"));

                comp->set_attributes (values);
              }
            else if ((home = dynamic_cast <CIAO::Home_Servant_Impl_Base *> (svt.in ())))
              {
                CIAO_DEBUG (9,
                            (LM_TRACE,
                             CLINFO
                             "Container_i::set_attributes - "
                             "Configuring attribute values on "
                             "home object reference.\n"));

                home->set_attributes (values);
              }
            else
              {
                CIAO_ERROR (1, (LM_EMERGENCY, CLINFO
                                "Container_i::set_attributes - "
                                "Internal Container Error: Instance isn't a home or component\n"));
                throw CIAO::InvalidComponent (0,
                                              "Internal Container Error: "
                                              "Instance isn't a home or component\n");
              }
          }
      }
    catch (const CIAO::InvalidComponent &)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Container_i::set_attributes - "
                     "Failed to retrieve servant and/or cast "
                     "to servant pointer.\n"));
        throw;
      }
    catch (const CORBA::BAD_PARAM &)
      {
        std::ostringstream err;
        err << "Caught BAD_PARAM while setting attributes.  Likely indicates incorrect "
            << "data type in directive.\n";

        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Container_i::set_attributes - "
                     "Error: %C\n",
                     err.str ().c_str ()));

        throw Installation_Failure (0,
                                    err.str ().c_str ());
      }
    catch (const CORBA::Exception &ex)
      {
        std::ostringstream err;
        err << "Caught CORBA exception while configuring attributes: "
            << ex._info ().c_str ();

        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Container_i::set_attributes - %C\n",
                     err.str ().c_str ()));
        throw Installation_Failure (0,err.str ().c_str ());
      }
    catch (...)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Container_i::set_attributes - "
                     "Caught unknown C++ exception while "
                     "configuring component attributes.\n"));

        throw Installation_Failure (0,
                                    "Unknown C++ exception while configuring attributes\n");
      }
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
    return ::PortableServer::POA::_duplicate (this->component_poa_.in ());
  }

  template <typename BASE>
  PortableServer::POA_ptr
  Container_i<BASE>::the_port_POA (void)
  {
    return ::PortableServer::POA::_duplicate (this->facet_cons_poa_.in ());
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

  template <typename BASE>
  ::Components::Cookie *
  Container_i<BASE>::connect_local_facet (::Components::CCMObject_ptr provider,
                                          const char * provider_port,
                                          ::Components::CCMObject_ptr user,
                                          const char * user_port)
  {
    CIAO_TRACE ("Container_i::connect_local_facet");

    if (!provider_port || !user_port)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Container_i::connect_local_facet - "
                     "Nil port name provided to connect local "
                     "facet, throwing exception\n"));

        throw ::Components::InvalidConnection ();
      }

    try
      {
        PortableServer::ServantBase_var provider_tmp =
          this->component_poa_->reference_to_servant (provider);

        CIAO_DEBUG (9,
                    (LM_TRACE,
                     CLINFO
                     "Container_i::connect_local_facet - "
                     "Successfully fetched provider servant"
                     " [%C] from POA\n",
                     provider_port));

        CIAO::Connector_Servant_Impl_Base *prov_serv =
          dynamic_cast<CIAO::Connector_Servant_Impl_Base *> (provider_tmp.in ());

        if (!prov_serv)
          {
            CIAO_ERROR (1,
                        (LM_ERROR,
                         CLINFO
                         "Container_i::connect_local_facet - "
                         "Unable to cast to provider servant "
                         "implementation\n"));
            throw ::Components::InvalidConnection ();
          }

        PortableServer::ServantBase_var user_tmp =
          this->component_poa_->reference_to_servant (user);

        CIAO_DEBUG (9, (LM_TRACE, CLINFO "Container_i::connect_local_facet - "
                     "Successfully fetched user servant [%C] from POA\n", user_port));

        CIAO::Connector_Servant_Impl_Base *user_serv =
          dynamic_cast<CIAO::Connector_Servant_Impl_Base *> (user_tmp.in ());

        if (user_serv == 0)
          {
            CIAO_ERROR (1,
                        (LM_ERROR,
                         CLINFO
                         "Container_i::connect_local_facet - "
                         "Unable to cast to user "
                         "servant implementation\n"));

            throw ::Components::InvalidConnection ();
          }

        ::CORBA::Object_var exec =
          prov_serv->get_facet_executor (provider_port);

        // Note: Spec says that facet executor provided by component MAY BE NIL
        if (!::CORBA::is_nil (exec.in ()))
          {
            CIAO_DEBUG (6,
                        (LM_DEBUG,
                         CLINFO
                         "Container_i::connect_local_facet - "
                         "Create connection between [%C]"
                         " and [%C]\n",
                         user_port,
                         provider_port));
            ::Components::Cookie_var cookie =
              user_serv->connect (user_port, exec.in ());
            return cookie._retn ();
          }
        else
          {
            CIAO_DEBUG (6,
                        (LM_DEBUG,
                         CLINFO
                         "Container_i::connect_local_facet - "
                         "Got nil facet executor for [%C]\n",
                         provider_port));
          }
      }
    catch (const ::Components::InvalidConnection &ex)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Container_i::connect_local_facet - "
                     "Caught exception InvalidConnection while connecting "
                     "<%C> to <%C>\n",
                     provider_port,
                     user_port));

        throw ex;
      }
    catch (const ::Components::InvalidName &ex)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Container_i::connect_local_facet - "
                     "Caught exception InvalidName while connecting "
                     "<%C> to <%C>\n",
                     provider_port,
                     user_port));

        throw ex;
      }
    catch (const ::CORBA::Exception &ex)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Container_i::connect_local_facet - "
                     "Caught exception %C.\n",
                     ex._info ().c_str ()));
        throw;
      }
    catch (...)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Container_i::connect_local_facet - "
                     "Attempting to connect components not "
                     "managed by this container.\n"));

        throw ::Components::InvalidConnection ();
      }
    return 0;
  }

  template <typename BASE>
  void
  Container_i<BASE>::disconnect_local_facet (
    ::Components::Cookie * cookie,
    ::Components::CCMObject_ptr provider,
    const char * provider_port,
    ::Components::CCMObject_ptr user,
    const char * user_port)
  {
    CIAO_TRACE ("Container_i::disconnect_local_facet");

    try
      {
        PortableServer::ServantBase_var srv_tmp =
          this->component_poa_->reference_to_servant (provider);

        CIAO_DEBUG (9,
                    (LM_TRACE,
                     CLINFO
                     "Container_i::disconnect_local_facet - "
                     "Successfully fetched provider servant from POA\n"));

        CIAO::Connector_Servant_Impl_Base *prov_serv =
          dynamic_cast<CIAO::Connector_Servant_Impl_Base *> (srv_tmp.in ());

        if (prov_serv == 0)
          {
            CIAO_ERROR (1,
                        (LM_ERROR,
                         CLINFO
                         "Container_i::disconnect_local_facet - "
                         "Unable to cast to provider servant "
                         "implementation\n"));

            throw ::Components::InvalidConnection ();
          }

        srv_tmp = this->component_poa_->reference_to_servant (user);

        CIAO_DEBUG (9,
                    (LM_TRACE,
                     CLINFO
                     "Container_i::disconnect_local_facet - "
                     "Successfully fetched user servant from POA\n"));

        CIAO::Connector_Servant_Impl_Base *user_serv =
          dynamic_cast<CIAO::Connector_Servant_Impl_Base *> (srv_tmp.in ());

        if (user_serv == 0)
          {
            CIAO_ERROR (1,
                        (LM_ERROR,
                         CLINFO
                         "Container_i::disconnect_local_facet - "
                         "Unable to cast to user servant "
                         "implementation\n"));

            throw ::Components::InvalidConnection ();
          }

        ::CORBA::Object_var exec =
          prov_serv->get_facet_executor (provider_port);

        // Note: Spec says that facet executor provided by component MAY BE NIL
        if (!::CORBA::is_nil (exec.in ()))
          {
            ::CORBA::Object_var port = user_serv->disconnect (user_port, cookie);
          }
        else
          {
            CIAO_DEBUG (6,
                        (LM_DEBUG,
                         CLINFO
                         "Container_i::disconnect_local_facet - "
                         "Got nil facet executor for [%C]\n",
                         provider_port));
          }
      }
    catch (const ::Components::InvalidName &ex)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Container_i::disconnect_local_facet - "
                     "Caught exception InvalidName while connecting "
                     "<%C> to <%C>\n",
                     provider_port,
                     user_port));

        throw ex;
      }
    catch (const ::CORBA::Exception &ex)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Container_i::disconnect_local_facet - "
                     "Caught exception %C.\n",
                     ex._info ().c_str ()));

        throw;
      }
    catch (...)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Container_i::disconnect_local_facet - "
                     "Attempting to connect components not "
                     "managed by this container.\n"));

        throw ::Components::InvalidConnection ();
      }
  }

  template <typename BASE>
  ::CORBA::Object_ptr
  Container_i<BASE>::get_objref (PortableServer::Servant p)
  {
    return this->component_poa_->servant_to_reference (p);
  }

  template <typename BASE>
  ::CIAO::Servant_Activator_ptr
  Container_i<BASE>::ports_servant_activator (void)
  {
    return Servant_Activator::_duplicate(this->sa_.in ());
  }

  template <typename BASE>
  CORBA::Object_ptr
  Container_i<BASE>::generate_reference (const char *obj_id,
                                         const char *repo_id,
                                         Container_Types::OA_Type t)
  {
    CIAO_TRACE ("Container_i::generate_reference");

    PortableServer::POA_ptr tmp = PortableServer::POA::_nil();

    if (t == Container_Types::COMPONENT_t
        || t == Container_Types::HOME_t)
      {
        tmp = this->component_poa_.in ();
      }
    else
      {
        tmp = this->facet_cons_poa_.in ();
      }

    PortableServer::ObjectId_var oid =
      PortableServer::string_to_ObjectId (obj_id);

    CORBA::String_var str =
      PortableServer::ObjectId_to_string (oid.in ());

    CORBA::Object_var objref =
      tmp->create_reference_with_id (oid.in (), repo_id);

    return objref._retn ();
  }
}

#endif /* CIAO_CONTAINERBASE_IMPL_T_C */

