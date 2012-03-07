// $Id$

#include "Container_Handler.h"
#include "ciao/Logger/Log_Macros.h"
#include "ciao/Logger/Logger_Service.h"
#include "ciao/Base/CIAO_PropertiesC.h"
#ifdef DANCE_DEPLOYS_EXTENSION_CONTAINER
  #include "ciao/Containers/Extension/Extension_Container.h"
#else
  #include "ciao/Containers/Session/Session_Container.h"
#endif
#include "ciao/Base/Server_init.h"
#include "dance/DAnCE_Utility.h"
#include "dance/DAnCE_PropertiesC.h"
#include "dance/LocalityManager/Scheduler/Plugin_Manager.h"
#include "CIAO_State.h"

namespace CIAO
{
  Container_Handler_i::Container_Handler_i (void)
  {
    // Also initialize CIAO logger since we reuse parts of CIAO in the locality manager
    CIAO::Logger_Service
      * clf = ACE_Dynamic_Service<CIAO::Logger_Service>::instance ("CIAO_Logger");

    int argc = 0;
    ACE_TCHAR **argv = 0;

    if (clf)
      {
        clf->init (argc, argv);
      }

    CIAO_TRACE ("Container_Handler_i::Container_Handler_i");
  }

  // Destructor
  Container_Handler_i::~Container_Handler_i (void)
  {
    CIAO_TRACE ("Container_Handler_i::~Container_Handler_i");
  }

  void
  Container_Handler_i::close (void)
  {
    try
      {
        // For the time being, we are going to go ahead and delete
        // the default container
        ::Deployment::DeploymentPlan plan;
        plan.instance.length (1);
        plan.instance[0].name = "";

        CORBA::Any any;
        this->remove_instance (plan, 0, any);
      }
    catch (...)
      { //swallow
      }

    DEPLOYMENT_STATE::instance ()->close ();
    DEPLOYMENT_STATE::close ();

    this->orb_ = CORBA::ORB::_nil ();
    this->poa_ = PortableServer::POA::_nil ();
  }

  ::CORBA::StringSeq *
  Container_Handler_i::dependencies (void)
  {
    ::CORBA::StringSeq *retval (0);
    ACE_NEW_THROW_EX (retval,
                      ::CORBA::StringSeq (0),
                      CORBA::NO_MEMORY ());

    return retval;
  }

  char *
  Container_Handler_i::instance_type (void)
  {
    CIAO_TRACE ("Container_Handler_i::instance_type");
    return CORBA::string_dup (CIAO::Deployment::CCM_CONTAINER);
  }

  void
  Container_Handler_i::install_instance (const ::Deployment::DeploymentPlan &plan,
                                         ::CORBA::ULong instanceRef,
                                         ::CORBA::Any_out instance_reference)
  {
    CIAO_TRACE ("Container_Handler_i::install_instance");

    ACE_NEW_THROW_EX (instance_reference,
                      ::CORBA::Any (),
                      CORBA::NO_MEMORY ());

    const char *name (plan.instance[instanceRef].name.in ());
#ifdef DANCE_DEPLOYS_EXTENSION_CONTAINER
    CIAO::Extension_Container *cont (0);

    CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                    "Container_Handler_i::install_instance - "
                    "Creating extension container with id <%C>\n",
                    name));
    ACE_NEW_THROW_EX (cont,
                      CIAO::Extension_Container_i (this->orb_,
                                                   this->poa_),
                      CORBA::NO_MEMORY ());
#else
    CIAO::Session_Container *cont (0);

    CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                    "Container_Handler_i::install_instance - "
                    "Creating session container with id <%C>\n",
                    name));
    ACE_NEW_THROW_EX (cont,
                      CIAO::Session_Container_i (this->orb_,
                                                 this->poa_),
                      CORBA::NO_MEMORY ());
#endif

    CIAO::Container_var container_ref (cont);

    CIAO_DEBUG (8, (LM_DEBUG, CLINFO
                    "Container_Handler_i::install_instance - "
                    "Container <%C> successfully created\n",
                    name));

    container_ref->init (name);

    CIAO_DEBUG (8, (LM_DEBUG, CLINFO
                    "Container_Handler_i::install_instance - "
                    "Container <%C> successfully initialized\n",
                    name));

    DEPLOYMENT_STATE::instance ()->add_container (name, container_ref.in ());

    *instance_reference <<= container_ref;
  }

  void
  Container_Handler_i::activate_instance (const ::Deployment::DeploymentPlan & ,
                                     ::CORBA::ULong ,
                                     const ::CORBA::Any &)
  {
    CIAO_TRACE ("Container_Handler_i::activate_instance");
    // no activation needed.
  }

  void
  Container_Handler_i::passivate_instance (const ::Deployment::DeploymentPlan & ,
                                      ::CORBA::ULong ,
                                      const ::CORBA::Any &)
  {
    CIAO_TRACE ("Container_Handler_i::passivate_instance");
    // no passivation needed.
  }

  void
  Container_Handler_i::provide_endpoint_reference (const ::Deployment::DeploymentPlan &,
                                                   ::CORBA::ULong,
                                                   ::CORBA::Any_out)
  {
    CIAO_TRACE ("Container_Handler_i::provide_endpoint_reference");
    CIAO_ERROR (1, (LM_ERROR, CLINFO
                    "Container_Handler_i::provide_endpoint_reference - "
                    "Unable to provide any endpoints.\n"));
    throw CORBA::NO_IMPLEMENT ();
  }

  void
  Container_Handler_i::remove_instance (
    const ::Deployment::DeploymentPlan & plan,
    ::CORBA::ULong instanceRef,
    const ::CORBA::Any &)
  {
    CIAO_TRACE ("Container_Handler_i::remove_instance");

    const char *name = plan.instance[instanceRef].name.in ();

    ::CIAO::Container_var cont =
        DEPLOYMENT_STATE::instance ()->fetch_container (name);

    if (::CORBA::is_nil (cont.in ()))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Container_Handler_i::remove_instance - "
                        "Error, no container with id <%C>\n",
                        name));
        throw ::Deployment::StopError (name,
                                       "No container with ID");
      }

    CIAO_DEBUG (8, (LM_TRACE, CLINFO
                    "Container_Handler_i::remove_instance - "
                    "Finalizing container with Id <%C>\n",
                    name));

    // Instructing container to cleanup its state
    cont->fini ();

    CIAO_DEBUG (8, (LM_TRACE, CLINFO
                    "Container_Handler_i::remove_instance - "
                    "Removing container with Id <%C>\n",
                    name));

    DEPLOYMENT_STATE::instance ()->remove_container (name);

    CIAO_DEBUG (5, (LM_TRACE, CLINFO
                    "Container_Handler_i::remove_instance - "
                    "Container with Id <%C> removed.\n",
                    name));
  }

  void
  Container_Handler_i::connect_instance (const ::Deployment::DeploymentPlan &,
                                         ::CORBA::ULong,
                                         const ::CORBA::Any &)
  {
    CIAO_TRACE ("Container_Handler_i::connect_instance");

    CIAO_ERROR (1, (LM_ERROR, CLINFO
                    "Container_Handler_i::connect_instance - ",
                    "No connections allowed for containers.\n"));
    throw CORBA::NO_IMPLEMENT ();
  }


  void
  Container_Handler_i::disconnect_instance (const ::Deployment::DeploymentPlan &,
                                            ::CORBA::ULong)
  {
    CIAO_TRACE ("Container_Handler_i::disconnect_instance");

    CIAO_ERROR (1, (LM_ERROR, CLINFO
                    "Container_Handler_i::disconnect_instance - ",
                    "No connections allowed for containers.\n"));
    throw CORBA::NO_IMPLEMENT ();
  }

  void
  Container_Handler_i::instance_configured (const ::Deployment::DeploymentPlan &,
                                            ::CORBA::ULong)
  {
    CIAO_TRACE ("Container_Handler_i::instance_configured");
    // nothing to be done
  }

  void
  Container_Handler_i::configure (const ::Deployment::Properties &props)
  {
    CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                    "Container_Handler_i::configure - "
                    "Received %u properties for configuration\n",
                    props.length ()));

    this->orb_ = DAnCE::PLUGIN_MANAGER::instance ()->get_orb ();

    if (CORBA::is_nil (this->orb_))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Container_Handler_i::configure - "
                        "Unable to locate ORB.\n"));
        throw ::Deployment::StartError ("CIAO Container Handler ",
                                        "Unable to locate ORB");
      }

    CORBA::Object_var object =
      this->orb_->resolve_initial_references ("RootPOA");

    this->poa_ =
      PortableServer::POA::_narrow (object.in ());

    if (CORBA::is_nil (this->poa_))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                        "Container_Handler_i::configure - "
                        "Unable to locate POA.\n"));
        throw ::Deployment::StartError ("CIAO Container Handler ",
                                        "Unable to locate POA");
      }

    CIAO::Server_init (this->orb_);

    // For the time being, we are going to go ahead and construct a container.
    ::Deployment::DeploymentPlan plan;
    plan.instance.length (1);
    plan.instance[0].name = "";

    ::CORBA::Any_var any;
    this->install_instance (plan, 0, any.out ());
  }
}

extern "C"
{
  ::DAnCE::InstanceDeploymentHandler_ptr
  CIAO_Locality_Handler_Export create_Container_Handler (void)
  {
    return new CIAO::Container_Handler_i ();
  }
}
