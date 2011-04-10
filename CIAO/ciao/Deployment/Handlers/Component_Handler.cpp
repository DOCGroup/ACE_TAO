// $Id$

#include "Component_Handler.h"
#include "ciao/Logger/Log_Macros.h"
#include "ciao/Base/CIAO_PropertiesC.h"
#include "ciao/Base/CIAO_ExceptionsC.h"
#include "ciao/Containers/Container_BaseC.h"
#include "ccm/CCM_ObjectC.h"
#include "CIAO_State.h"

namespace CIAO
{
  Component_Handler_i::Component_Handler_i (void)
  {
    CIAO_TRACE ("Component_Handler_i::Component_Handler_i");
  }

  // Destructor
  Component_Handler_i::~Component_Handler_i (void)
  {
    CIAO_TRACE ("Component_Handler_i::~Component_Handler_i");
  }

  void
  Component_Handler_i::close (void)
  {
    this->instances_.clear ();
  }

  ::CORBA::StringSeq *
  Component_Handler_i::dependencies (void)
  {
    ::CORBA::StringSeq *retval (0);
    ACE_NEW_THROW_EX (retval,
                      ::CORBA::StringSeq (1),
                      CORBA::NO_MEMORY ());

    retval->length (1);
    (*retval)[0] = CIAO::Deployment::CCM_CONTAINER;

    return retval;
  }

  char *
  Component_Handler_i::instance_type (void)
  {
    CIAO_TRACE ("Component_Handler_i::instance_type");
    return CORBA::string_dup ("edu.dre.vanderbilt.dre.CCM.Component");
  }

  void
  Component_Handler_i::install_instance (const ::Deployment::DeploymentPlan &plan,
                                         ::CORBA::ULong instanceRef,
                                         ::CORBA::Any_out instance_reference)
  {
    CORBA::Any *retval = 0;
    ACE_NEW_THROW_EX (retval,
                      CORBA::Any (),
                      CORBA::NO_MEMORY ());

    instance_reference = retval;

    CIAO_TRACE ("Component_Handler_i::install_instance");

    const ::Deployment::InstanceDeploymentDescription &idd (plan.instance[instanceRef]);
    const ::Deployment::MonolithicDeploymentDescription &mdd (plan.implementation[idd.implementationRef]);

    DAnCE::Utility::PROPERTY_MAP *pmap = 0;

    ACE_NEW_THROW_EX (pmap,
                      DAnCE::Utility::PROPERTY_MAP (idd.configProperty.length () +
                                                    mdd.execParameter.length ()),
                      CORBA::NO_MEMORY ());

    Deployment_Common::Instance_Info info;
    info.name = idd.name.in ();
    info.pmap.reset (pmap);

    DAnCE::Utility::build_property_map (*pmap,
                                        mdd.execParameter);
    DAnCE::Utility::build_property_map (*pmap,
                                        idd.configProperty);

    CORBA::String_var exec_art, exec_entry, svnt_art, svnt_entry, cont_id;

    using namespace CIAO::Deployment;
    CORBA::Any val;
    const char *tmp = 0;

    if (pmap->find (SVNT_ENTRYPT, val) == 0)
        {
          val >>= tmp;
          svnt_entry = tmp;
          CIAO_DEBUG (9, (LM_TRACE, CLINFO
                          "Component_Handler_i::install_instance - "
                          "Found Servant entrypoint %C\n", svnt_entry.in ()));
        }
      else
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO
                       "Component_Handler_i::install_instance - "
                       "Error: No Servant entrypoint provided, aborting installation\n"));
          throw ::Deployment::PlanError (idd.name.in (),
                                         "No servant entrypoint identified.");
        }

      if (pmap->find (SVNT_ARTIFACT, val) == 0)
        {
          val >>= tmp;
          svnt_art = Deployment_Common::get_implementation (tmp, plan);
          CIAO_DEBUG (9, (LM_TRACE, CLINFO
                       "Component_Handler_i::install_instance - "
                       "Found Servant artifact %C\n", svnt_art.in ()));
        }
      else
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO
                       "Component_Handler_i::install_instance - "
                       "Error: No Servant artifact provided, aborting installation\n"));
          throw ::Deployment::PlanError (idd.name.in (),
                                         "No servant artifact identified.");
        }


      if (pmap->find (EXEC_ARTIFACT, val) == 0)
        {
          val >>= tmp;
          exec_art = Deployment_Common::get_implementation (tmp, plan);
          CIAO_DEBUG (9, (LM_TRACE, CLINFO
                       "Component_Handler_i::install_instance - "
                       "Found executor artifact: %C\n", exec_art.in ()));
        }
      else
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO
                       "Component_Handler_i::install_instance - "
                       "Error: No Executor artifact provided, aborting installation\n"));
          throw ::Deployment::PlanError (idd.name.in (),
                                         "No executory artifact identified.\n");
        }

      if (pmap->find (COMPONENT_FACTORY, val) == 0)
        {
          val >>= tmp;
          exec_entry = tmp;
          CIAO_DEBUG (9, (LM_TRACE, CLINFO
                       "Component_Handler_i::install_instance - "
                          "Found executor entrypoint: %C\n", exec_entry.in ()));
        }
      else
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO
                       "Component_Handler_i::install_instance - "
                       "Error: No Executor artifact provided, aborting installation\n"));
          throw ::Deployment::PlanError (idd.name.in (),
                                         "No executor entrypoint provided\n");
        }

      if (pmap->find (CONTAINER_ID, val) == 0)
        {
          val >>= tmp;
          cont_id = tmp;
          CIAO_DEBUG (9, (LM_TRACE, CLINFO
                          "Component_Handler_i::install_instance - "
                          "Found executor entrypoint: %C\n", exec_entry.in ()));
        }
      else
        {
          CIAO_ERROR (4, (LM_INFO, CLINFO
                          "Component_Handler_i::install_instance - "
                          "No container provided, using default container for instance <%C>\n",
                          idd.name.in  ()));
          cont_id = "";
        }

      ::CIAO::Container_var container = DEPLOYMENT_STATE::instance ()->fetch_container (cont_id);

      if (CORBA::is_nil (container))
        {
          CIAO_ERROR (1, (LM_INFO, CLINFO
                          "Component_Handler_i::install_instance - "
                          "Container with Id <%C> not installed, aborting.\n",
                          cont_id.in ()));
          throw ::Deployment::PlanError (idd.name.in (),
                                         "Invalid container\n");
        }

      Components::CCMObject_var comp_ref;

      try
        {
          comp_ref = container->install_component (exec_art,
                                                   exec_entry,
                                                   svnt_art,
                                                   svnt_entry,
                                                   idd.name.in ());

          ::Components::ConfigValues attr_config;
          Deployment_Common::create_attribute_configuration (idd.configProperty,
                                                             attr_config);

          container->set_attributes (comp_ref.in (),
                                     attr_config);
        }
      catch (::CIAO::InvalidComponent &ex)
        {
          if (ex.name.in () == 0)
            {
              ex.name = idd.name;
            }

          CIAO_ERROR (1, (LM_ERROR, CLINFO
                          "Component_Handler::install_instance - "
                          "Caught InvalidComponent exception: %C:%C\n",
                          ex.name.in (), ex.reason.in ()));
          throw ::Deployment::StartError (ex.name.in (),
                                          ex.reason.in ());
        }
      catch (::CIAO::Installation_Failure &ex)
        {
          if (ex.name.in () == 0)
            {
              ex.name = idd.name;
            }

          CIAO_ERROR (1, (LM_ERROR, CLINFO
                          "Component_Handler::install_instance - "
                          "Caught Installation_Failure exception: %C:%C\n",
                          ex.name.in (), ex.reason.in ()));
          throw ::Deployment::StartError (ex.name.in (),
                                          ex.reason.in ());
        }
      catch (const ::CORBA::Exception &ex)
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO
                          "Component_Handler_i::install_instance - "
                          "Caught CORBA exception <%C>\n",
                          ex._info ().c_str ()));
          throw ::Deployment::StartError (idd.name.in (),
                                          ex._info ().c_str ());
        }
      catch (...)
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO
                          "Component_Handler_i::install_instance - "
                          "Caught unknown C++ exception\n"));
          throw ::Deployment::StartError (idd.name.in (),
                                          "Unknown C++ exception\n");
        }

      if (CORBA::is_nil (comp_ref))
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO
                          "Component_Handler_i::install_instance - "
                          "Container provided nil object reference for component.\n"));
          throw ::Deployment::StartError (idd.name.in (),
                                          "Container provided nil object reference");
        }

      this->instances_[idd.name.in ()] = info;

      DEPLOYMENT_STATE::instance ()->add_component (idd.name.in (),
                                                    cont_id,
                                                    comp_ref.in ());

      *instance_reference <<= comp_ref;
  }

  void
  Component_Handler_i::activate_instance (const ::Deployment::DeploymentPlan &plan,
                                     ::CORBA::ULong instanceRef,
                                     const ::CORBA::Any &)
  {
    CIAO_TRACE ("Component_Handler_i::activate_instance");

    const char *name = plan.instance[instanceRef].name.in ();

    const char *container =
      DEPLOYMENT_STATE::instance ()->instance_to_container (name);

    CIAO::Container_var cont =
      DEPLOYMENT_STATE::instance ()->fetch_container (container);

    Components::CCMObject_var comp =
      DEPLOYMENT_STATE::instance ()->fetch_component (name);

    if (CORBA::is_nil (cont))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Component_Handler_i::instance_configured - "
                        "Object reference for container of instance <%C> is nil!\n",
                        plan.instance[instanceRef].name.in ()));
        throw ::Deployment::StartError (plan.instance[instanceRef].name.in (),
                                        "Unable to activate due to nil container reference");
      }

    if (CORBA::is_nil (comp))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Component_Handler_i::instance_configured - "
                        "Object reference for instance <%C> is nil!\n",
                        plan.instance[instanceRef].name.in ()));
        throw ::Deployment::StartError (plan.instance[instanceRef].name.in (),
                                        "Unable to activate due to nil reference");
      }

    try
      {
        cont->activate_component (comp.in ());
      }
    catch (const CORBA::Exception &ex)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Component_Handler_i::activate_instance - "
                        "Caught CORBA Exception while activating instance <%C>: %C\n",
                        name,
                        ex._info ().c_str ()));
        throw ::Deployment::StartError (name,
                                        ex._info ().c_str ());
      }
    catch (...)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Component_Handler_i::activate_instance - "
                        "Caught C++ Exception while activating instance <%C>\n",
                        name));
        throw ::Deployment::StartError (name,
                                        "Unknown C++ exception during activation");
      }
  }

  void
  Component_Handler_i::passivate_instance (const ::Deployment::DeploymentPlan &plan,
                                           ::CORBA::ULong instanceRef,
                                           const ::CORBA::Any &)
  {
    CIAO_TRACE ("Component_Handler_i::passivate_instance");

    const char *name = plan.instance[instanceRef].name.in ();

    const char *container =
      DEPLOYMENT_STATE::instance ()->instance_to_container (name);

    CIAO::Container_var cont =
      DEPLOYMENT_STATE::instance ()->fetch_container (container);

    Components::CCMObject_var comp =
      DEPLOYMENT_STATE::instance ()->fetch_component (name);

    if (CORBA::is_nil (cont))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Component_Handler_i::instance_configured - "
                        "Object reference for container of instance <%C> is nil!\n",
                        plan.instance[instanceRef].name.in ()));
        throw ::Deployment::StopError (plan.instance[instanceRef].name.in (),
                                        "Unable to activate due to nil container reference");
      }

    if (CORBA::is_nil (comp))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Component_Handler_i::instance_configured - "
                        "Object reference for instance <%C> is nil!\n",
                        plan.instance[instanceRef].name.in ()));
        throw ::Deployment::StopError (plan.instance[instanceRef].name.in (),
                                        "Unable to activate due to nil reference");
      }

    try
      {
        cont->passivate_component (comp.in ());
      }
    catch (const CORBA::Exception &ex)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Component_Handler_i::passivate_instance - "
                        "Caught CORBA Exception while passivating instance <%C>: %C\n",
                        name,
                        ex._info ().c_str ()));
        throw ::Deployment::StopError (name,
                                        ex._info ().c_str ());
      }
    catch (...)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Component_Handler_i::passivate_instance - "
                        "Caught C++ Exception while passivating instance <%C>\n",
                        name));
        throw ::Deployment::StartError (name,
                                        "Unknown C++ exception during passivation");
      }
  }

  void
  Component_Handler_i::remove_instance (const ::Deployment::DeploymentPlan & plan,
                                        ::CORBA::ULong instanceRef,
                                        const ::CORBA::Any &)
  {
    CIAO_TRACE ("Component_Handler_i::remove_instance");

    const char *name = plan.instance[instanceRef].name.in ();
    Deployment_Common::INSTANCES::iterator instance
      = this->instances_.find (name);

    if (instance == this->instances_.end ())
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Component_Handler_i::remove_instance - "
                        "Instructed to remove unknown component instance <%C>\n",
                        name));
        throw ::Deployment::StopError (name,
                                       "Wrong instance handler for component instance\n");
      }

    CIAO_DEBUG (8, (LM_DEBUG, CLINFO
                    "Component_Handler_i::remove_instance - "
                    "Attempting removal of component instance <%C>\n",
                    name));

    CORBA::Any val;
    const char *cont_id = 0;

    if (instance->second.pmap->find (Deployment::CONTAINER_ID, val) == 0)
        {
          val >>= cont_id;
          CIAO_DEBUG (9, (LM_TRACE, CLINFO
                          "Component_Handler_i::remove_instance - "
                          "Found component container: %C\n", cont_id));
        }
      else
        {
          CIAO_ERROR (4, (LM_INFO, CLINFO
                          "Component_Handler_i::remove_instance - "
                          "No container provided, using default container for instance <%C>\n",
                          name));
          cont_id = "";
        }

    ::CIAO::Container_var container = DEPLOYMENT_STATE::instance ()->fetch_container (cont_id);

      if (CORBA::is_nil (container.in ()))
        {
          CIAO_ERROR (1, (LM_INFO, CLINFO
                          "Component_Handler_i::remove_instance - "
                          "Container with Id <%C> not installed, aborting.\n",
                          cont_id));
          throw ::Deployment::StopError (name,
                                         "Invalid container\n");
        }

      try
        {
          Components::CCMObject_var ref
            = DEPLOYMENT_STATE::instance ()->fetch_component (name);

          if (!CORBA::is_nil (ref.in ()))
            {
              container->uninstall_component (ref.in ());
            }
        }
      catch (const CORBA::Exception &ex)
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO
                          "Component_Handler_i::remove_instance - "
                          "Caught CORBA exception <%C>\n",
                          ex._info ().c_str ()));
          throw ::Deployment::StopError (name,
                                         ex._info ().c_str ());
        }
      catch (...)
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO
                          "Component_Handler_i::remove_instance - "
                          "Caught unknown C++ exception\n"));
          throw ::Deployment::StopError (name,
                                          "Unknown C++ exception\n");
        }

      this->instances_.erase (instance);
      DEPLOYMENT_STATE::instance ()->remove_component (name);

      CIAO_DEBUG (4, (LM_INFO, CLINFO
                      "Component_Handler_i::remove_instance - "
                      "Instance <%C> successfully removed\n",
                      name));
  }

  void
  Component_Handler_i::provide_endpoint_reference (const ::Deployment::DeploymentPlan & plan,
                                                   ::CORBA::ULong connectionRef,
                                                   ::CORBA::Any_out endpoint_reference)
  {
    CIAO_TRACE ("Component_Handler_i::provide_endpoint_reference");

    DEPLOYMENT_STATE::instance ()->connection_handler.provide_endpoint_reference (plan,
                                                                                  connectionRef,
                                                                                  endpoint_reference);
  }

  void
  Component_Handler_i::connect_instance (const ::Deployment::DeploymentPlan & plan,
                                         ::CORBA::ULong connectionRef,
                                         const ::CORBA::Any & provided_reference)
  {
    CIAO_TRACE ("Component_Handler_i::connect_instance");

    DEPLOYMENT_STATE::instance ()->connection_handler.connect_instance (plan,
                                                                        connectionRef,
                                                                        provided_reference);
  }

  void
  Component_Handler_i::disconnect_instance (const ::Deployment::DeploymentPlan & plan,
                                            ::CORBA::ULong connectionRef)
  {
    CIAO_TRACE ("Component_Handler_i::disconnect_instance");
    DEPLOYMENT_STATE::instance ()->connection_handler.disconnect_instance (plan,
                                                                           connectionRef);

  }

  void
  Component_Handler_i::instance_configured (const ::Deployment::DeploymentPlan & plan,
                                            ::CORBA::ULong instanceRef)
  {
    CIAO_TRACE ("Component_Handler_i::instance_configured");

    Components::CCMObject_var ref =
      DEPLOYMENT_STATE::instance ()->fetch_component (plan.instance[instanceRef].name.in ());

    if (CORBA::is_nil (ref))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Component_Handler_i::instance_configured - "
                        "Object reference for instance <%C> is nil!\n",
                        plan.instance[instanceRef].name.in ()));
        throw ::Deployment::StartError (plan.instance[instanceRef].name.in (),
                                        "Unable to configure due to nil reference");
      }

    try
      {
        ref->configuration_complete ();
      }
    catch (const CORBA::Exception &ex)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Component_Handler_i::instance_configured - "
                        "Caught CORBA Exception: %C\n",
                        ex._info ().c_str ()));
        throw ::Deployment::StartError (plan.instance[instanceRef].name.in (),
                                        ex._info ().c_str ());
      }
    catch (...)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Component_Handler_i::instance_configured - "
                        "Caught C++ Exception\n"));
        throw ::Deployment::StartError (plan.instance[instanceRef].name.in (),
                                        "Unknown C++ exception");

      }

  }

  void
  Component_Handler_i::configure (const ::Deployment::Properties & )
  {

  }
}

extern "C"
{
  ::DAnCE::InstanceDeploymentHandler_ptr
  CIAO_Locality_Handler_Export create_Component_Handler (void)
  {
    return new CIAO::Component_Handler_i ();
  }
}
