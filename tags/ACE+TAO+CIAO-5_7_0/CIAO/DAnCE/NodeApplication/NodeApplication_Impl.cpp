// $Id$
#include "NodeApplication_Impl.h"

#include "ace/OS_Memory.h"
#include "ace/Log_Msg.h"
#include "ace/streams.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/Object.h"
#include "tao/ORB.h"
#include "ccm/CCM_ObjectC.h"
#include "ccm/CCM_SessionComponentC.h"
#include "ciao/Valuetype_Factories/ConfigValue.h"
#include "ciao/ComponentServer/CIAO_ServerActivator_Impl.h"
#include "ciao/ComponentServer/CIAO_ComponentInstallation_Impl.h"
#include "ciao/ComponentServer/CIAO_PropertiesC.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "Deployment/Deployment_BaseC.h"
#include "Deployment/Deployment_ApplicationC.h"
#include "Deployment/Deployment_common.h"
#include "Deployment/DAnCE_PropertiesC.h"
#include "ComponentAttributesSetter.h"
#include "Name_Utilities.h"

//#include "ComponentInstallation_Impl.h"

#ifdef GEN_OSTREAM_OPS
#include <iostream>
#include <sstream>
#endif /* GEN_OSTREAM_OPS */

using namespace DAnCE;

namespace
{
  template<class T>
  bool get_property_value (const char *name, PROPERTY_MAP &properties, T &val)
  {
    DANCE_TRACE ("NodeApplicion::<anonymous>::get_property_value<T>");
    CORBA::Any any;

    DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplicion::<anonymous>::get_property_value<T> - "
                  "Finding property value for name '%C'\n",
                  name));

    if (properties.find (name, any) == 0)
      {
        if (any >>= val)
                  {
            return true;
                  }
        else
          {
            DANCE_ERROR ((LM_WARNING, DLINFO "NodeApplicion::<anonymous>::get_property_value<T> - "
                          "Failed to extract property value for %C\n", name));
            return false;
          }
      }

    DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplicion::<anonymous>::get_property_value<T> - "
                  "Property value for name '%C' has no value\n", name));

    return false;
  }

  template<>
  bool get_property_value (const char *name, PROPERTY_MAP &properties, bool &val)
  {
    DANCE_TRACE ("NodeApplicion::<anonymous>::get_property_value<bool>");
    CORBA::Any any;

    DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplicion::<anonymous>::get_property_value<bool> - "
                  "Finding property value for name '%C'\n",
                  name));

    if (properties.find (name, any) == 0)
      {
        if (any >>= CORBA::Any::to_boolean(val))
                  {
            return true;
                  }
        else
          {
            DANCE_ERROR ((LM_WARNING, DLINFO "NodeApplicion::<anonymous>::get_property_value<T> - "
                          "Failed to extract property value for %C\n", name));
            return false;
          }
      }

    DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplicion::<anonymous>::get_property_value<bool> - "
                  "Property value for name '%C' has no value\n", name));

    return false;
  }

  template<>
  bool get_property_value (const char *name, PROPERTY_MAP &properties, const char * &val)
  {
    DANCE_TRACE ("NodeApplicion::<anonymous>::get_property_value<const char *>");
    CORBA::Any any;

    DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplicion::<anonymous>::get_property_value<bool> - "
                  "Finding property value for name '%C'\n",
                  name));

    if (properties.find (name, any) == 0)
      {
        if (any >>= CORBA::Any::to_string(val, 0))
                  {
            return true;
                }
        else
          {
            DANCE_ERROR ((LM_WARNING, DLINFO "NodeApplicion::<anonymous>::get_property_value<const char *> - "
                          "Failed to extract property value for %C\n", name));
            return false;
          }
      }

    DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplicion::<anonymous>::get_property_value<bool> - "
                  "Property value for name '%C' has no value\n", name));

    return false;
  }

  template<class T>
  bool get_property_value (const char *name, const Deployment::Properties &properties, T &val)
  {
    DANCE_TRACE ("NodeApplicion::<anonymous>::get_property_value<T>");

    DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplicion::<anonymous>::get_property_value<T> - "
                  "Finding property value for name '%C'\n",
                  name));

    for (CORBA::ULong i = 0; i < properties.length (); ++i)
      {
        if (ACE_OS::strcmp (properties[i].name.in (), name) == 0)
          {
            DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplicion::<anonymous>::get_property_value<T> - "
                          "Found property '%C'\n", name));
            if (properties[i].value >>= val)
              return true;
            else
              {
                DANCE_ERROR ((LM_WARNING, DLINFO "NodeApplicion::<anonymous>::get_property_value<T> - "
                              "Failed to extract property value for %C\n", name));
                return false;
              }
          }
      }


    DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplicion::<anonymous>::get_property_value<T> - "
                  "Property value for name '%C' has no value\n", name));

    return false;
  }

  template<>
  bool get_property_value (const char *name, const Deployment::Properties &properties, const char * &val)
  {
    DANCE_TRACE ("NodeApplicion::<anonymous>::get_property_value<const char *>");

    DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplicion::<anonymous>::get_property_value<T> - "
                  "Finding property value for name '%C'\n",
                  name));

    for (CORBA::ULong i = 0; i < properties.length (); ++i)
      {
        if (ACE_OS::strcmp (properties[i].name.in (), name) == 0)
          {
            DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplicion::<anonymous>::get_property_value<T> - "
                          "Found property '%C'\n", name));
            if (properties[i].value >>= CORBA::Any::to_string (val, 0))
              {
                DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplicion::<anonymous>::get_property_value<T> - "
                              "Value is %C\n", val));
                return true;
              }
            else
              {
                DANCE_ERROR ((LM_WARNING, DLINFO "NodeApplicion::<anonymous>::get_property_value<T> - "
                              "Failed to extract property value for %C\n", name));
                return false;
              }
          }
      }


    DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplicion::<anonymous>::get_property_value<T> - "
                  "Unable to find property named %C\n", name));

    return false;
  }

  /// Tests flag, if false, sets it to true and replaces the name and
  /// reason flags of the exception.
  template <typename EXCEPTION>
  void test_and_set_exception (bool &flag, EXCEPTION &exception,
                               const char *name, const char *reason)
  {
    if (!flag)
      {
        flag = true;
        exception.name = name;
        exception.reason = reason;
      }
  }
  
  const char * get_artifact_location (const char * name, 
                                      const ::Deployment::ArtifactDeploymentDescriptions &art)
  {
    DANCE_TRACE ("NodeApplication::<anonymous>::get_artifact_location");
    
    for (CORBA::ULong i = 0; i < art.length (); ++i)
      {
        if (ACE_OS::strcmp (name, art[0].name.in ()) == 0)
          return art[0].location[0].in ();
      }
    
    return 0;
  }
}


NodeApplication_Impl::NodeApplication_Impl (CORBA::ORB_ptr orb,
                                            PortableServer::POA_ptr poa,
                                            const Deployment::DeploymentPlan& plan,
                                            RedirectionService & redirection,
                                            const ACE_CString& node_name,
                                            const PROPERTY_MAP &properties)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    plan_ (plan),
    redirection_ (redirection),
    node_name_ (node_name),
    properties_ (),
    instances_ (plan.instance.length ())
{
  DANCE_TRACE ("NodeApplication_Impl::NodeApplication_Impl");
  PROPERTY_MAP::const_iterator i = properties.begin ();
  while (!i.done ())
    {
      DANCE_DEBUG ((LM_DEBUG, DLINFO "NodeApplication_Impl::NodeApplication_Impl - "
                    "Binding value for property '%C'\n", i->key ().c_str ()));
      this->properties_.bind (i->key (), i->item ());
      i.advance ();
    }
  this->init ();
}

NodeApplication_Impl::~NodeApplication_Impl()
{
  DANCE_TRACE( "NodeApplication_Impl::~NodeApplication_Impl()");

  using namespace Components;
  ConfigValues config_values;
  config_values.length (1L);
  CORBA::Any feature_any;

  for (size_t i = 0;
       i != this->servers_.size ();
       ++i)
    {
      /* TODO: This is highly suspect.  I believe we should be using get_component_server,
         not calling create_container. */
      DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplication_Impl::~NodeApplication_Impl - "
                    "Deactivating %u ComponentServers\n",
                    this->servers_.size ()));
      for (size_t i = 0; i < this->servers_.size (); ++i)
        {
          ComponentServer &server = this->servers_[i];

          DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplication_Impl::~NodeApplication_Impl - "
                        "In ComponentServer %u, deactivating %u containers\n", i, server.containers.size ()));
          for (size_t j = 0; j < server.containers.size (); ++j)
            {
              Container &container = server.containers[j];

              DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplication_Impl::~NodeApplication_Impl - "
                            "In container %u hosted in server %u\n", j, i));

              try
                {
                  if (!CORBA::is_nil (container.ref))
                    server.ref->remove_container (container.ref.in ());
                  
                  container.ref = CIAO::Deployment::Container::_nil ();
                }
              catch (const CORBA::Exception &ex)
                {
                  DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::~NodeApplication_Impl - "
                                "Caught CORBA exception while removing container %u on server %u: %C\n",
                                j, i, ex._info ().c_str ()));
                }
              catch (...)
                {
                  DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::~NodeApplication_Impl - "
                                "Caught unknown C++ exception while removing container %u on server %u.\n",
                                j, i));
                }
            }

          DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplication_Impl::~NodeApplication_Impl - "
                        "Removing component server %u\n", i));

          try
            {
              if (!CORBA::is_nil (server.ref))
                this->activator_->remove_component_server (server.ref.in ());
            }
          catch (const CORBA::Exception &ex)
            {
              DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::~NodeApplication_Impl - "
                            "Caught CORBA exception while removing server %u: %C\n",
                            i, ex._info ().c_str ()));
            }
          catch (...)
            {
              DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::~NodeApplication_Impl - "
                            "Caught unknown C++ exception while removing server %u.\n",
                            i));
            }

          DANCE_DEBUG ((LM_INFO, DLINFO "NodeApplication_Impl::~NodeApplication_Impl - "
                        "Successfully removed container %u on node %C.\n",
                        i, this->node_name_.c_str ()));
        }
    }
}

void
NodeApplication_Impl::init()
{
  DANCE_TRACE( "NodeApplication_Impl::init()");

  /* TODO:  Lets move this stuff to the constructor, shall we?!? */
  /* TODO:  Might be nice to use a component configurator here to load the proper versions
     of the serveractivator.  */

  /* ServerActivator configuration */
  CORBA::ULong spawn = 0;
  const char *cs_path = 0;
  const char *cs_args = 0;
  CORBA::Boolean multithread = false;

  get_property_value (CIAO::Deployment::SERVER_EXECUTABLE, this->properties_, cs_path);
  DANCE_DEBUG ((LM_DEBUG, DLINFO "NodeApplication_Impl::init - "
                "Component server path: %C\n", cs_path));
  get_property_value (CIAO::Deployment::SERVER_ARGUMENTS, this->properties_, cs_args);
  DANCE_DEBUG ((LM_DEBUG, DLINFO "NodeApplication_Impl::init - "
                "Component server arguments: %C\n", cs_args));
  get_property_value (CIAO::Deployment::SERVER_TIMEOUT, this->properties_, spawn);
  DANCE_DEBUG ((LM_DEBUG, DLINFO "NodeApplication_Impl::init - "
                "Spawn delay: %u\n", spawn));
  get_property_value (CIAO::Deployment::SERVER_MULTITHREAD, this->properties_, multithread);
  DANCE_DEBUG ((LM_DEBUG, DLINFO "NodeApplication_Impl::init - "
                "Threading: %C\n",  multithread ? "Multi" : "Single"));

  DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplication_Impl::init - "
                "Spawning server activator\n"));
  
  CIAO::Deployment::ComponentInstallation_Impl *tmp_ci;

  ACE_NEW_THROW_EX (tmp_ci, 
                    CIAO::Deployment::ComponentInstallation_Impl (),
                    CORBA::NO_MEMORY ());
  
  PortableServer::ServantBase_var safe_servant = tmp_ci;
  
  this->poa_->activate_object (tmp_ci);
  
  for (CORBA::ULong i = 0; i < this->plan_.artifact.length (); ++i)
    {
      tmp_ci->install (this->plan_.artifact[i].name,
                       this->plan_.artifact[i].location[0]);
    }
  
  CIAO::Deployment::CIAO_ServerActivator_i *tmp_act;
  ACE_NEW_THROW_EX (tmp_act,
                    CIAO::Deployment::CIAO_ServerActivator_i (spawn,
                                                              cs_path,
                                                              cs_args,
                                                              multithread,
                                                              tmp_ci->_this (),
                                                              this->orb_.in(),
                                                              this->poa_.in()),
                    CORBA::NO_MEMORY ());
  this->activator_.reset (tmp_act);
  
  PortableServer::ObjectId_var sa_id =
    this->poa_->activate_object (this->activator_.get ());

  DANCE_DEBUG((LM_TRACE, DLINFO "NodeApplication_impl::init - ServerActivator object created\n"));

  const ACE_TCHAR *ior;

  if (get_property_value (DAnCE::INSTANCE_NC, this->properties_, ior) ||
      get_property_value (DAnCE::DOMAIN_NC, this->properties_, ior))
    {
      try
        {
          CORBA::Object_var obj = this->orb_->string_to_object (ior);
          this->instance_nc_ = CosNaming::NamingContext::_narrow (obj);
        }
      catch (const CORBA::Exception &e)
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_impl::init - "
                        "Unable to resolve the instance naming context:%C\n",
                        e._info ().c_str ()));
        }
      DANCE_DEBUG ((LM_DEBUG, DLINFO "NodeApplication_impl::init - "
                    "Successfully resolved the instance naming context.\n"));
    }
  else DANCE_DEBUG ((LM_DEBUG, DLINFO "NodeApplication_impl::init - "
                     "No instance NC was provided\n"));
}

void
NodeApplication_Impl::configuration_complete_components ()
{
  DANCE_TRACE( "NodeApplication_Impl::configuration_complete_components");

  bool error (false);
  Deployment::StartError exception;

  for (size_t k = 0; k < this->instances_.size (); ++k)
    {
      if (this->instances_[k]->type == eHome)
        {
          continue;
        }

      DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplication_Impl::configuration_complete_components - "
                    "Invoking configuration_complete on component instance %C on node %C\n",
                    this->plan_.instance[this->instances_[k]->idd_idx].name.in (),
                    this->node_name_.c_str ()));

      try
        {
          Components::CCMObject_var ccmobj =
            Components::CCMObject::_narrow (this->instances_[k]->ref.in ());
          if (CORBA::is_nil (this->instances_[k]->ref))
            {
              DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::configuration_complete_components - "
                            "Failed to narrow object reference for component instance %C\n",
                            this->plan_.instance[this->instances_[k]->idd_idx].name.in ()));
              continue;
            }

          if (this->instances_[k]->state == eInstalled)
            {
              ccmobj->configuration_complete ();
              this->instances_[k]->state = eConfigured;
            }
          else
            {
              if (!error)
                {
                  error = true;
                  exception.name = this->plan_.instance[this->instances_[k]->idd_idx].name.in ();
                  exception.reason = "Attempting to activate component that has already passed the configure stage.\n";
                  continue;
                }
            }

          DANCE_DEBUG ((LM_INFO, DLINFO "NodeApplication_Impl::configuration_complete_components - "
                        "Component %C successfully configured.\n",
                        this->plan_.instance[this->instances_[k]->idd_idx].name.in ()));
        }
      catch (const CORBA::Exception &ex)
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::configuration_complete_components - "
                        "Caught CORBA exception from ccm_activate on component %C: %C\n",
                        this->plan_.instance[this->instances_[k]->idd_idx].name.in (),
                        ex._info ().c_str ()));
          if (!error)
            {
              error = true;
              exception.name = this->plan_.instance[this->instances_[k]->idd_idx].name.in ();
              exception.reason = ex._info ().c_str ();
            }
        }
      catch (...)
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::configuration_complete_components - "
                        "Caught unknown C++ exception from ccm_activate on component %C\n",
                        this->plan_.instance[this->instances_[k]->idd_idx].name.in ()));
          if (!error)
            {
              error = true;
              exception.name = this->plan_.instance[this->instances_[k]->idd_idx].name.in ();
              exception.reason = "Unknown C++ exception";
            }
        }
    }

  if (error)
    throw exception;
}

void
NodeApplication_Impl::start ()
{
  DANCE_TRACE( "NodeApplication_Impl::start");

  bool error (false);
  Deployment::StartError exception;

  for (size_t k = 0; k < this->instances_.size (); ++k)
    {
      if (this->instances_[k]->type == eHome)
        {
          continue;
        }

      DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplication_Impl::start - "
                    "Invoking start on component instance %C on node %C\n",
                    this->plan_.instance[this->instances_[k]->idd_idx].name.in (),
                    this->node_name_.c_str ()));

      try
        {
          CIAO::Deployment::Container_var cont =
            CIAO::Deployment::Container::_narrow (this->instances_[k]->container->ref.in());

          if (CORBA::is_nil (this->instances_[k]->container->ref.in ()))
            {
              DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::start - "
                            "Failed to narrow object reference for container managing "
                            "component instance %C to a CIAO container reference\n",
                            this->plan_.instance[this->instances_[k]->idd_idx].name.in ()));
              test_and_set_exception (error, exception,
                                      this->plan_.instance[this->instances_[k]->idd_idx].name.in (),
                                      "Failed to narrow managing container to CIAO container type");
              continue;
            }

          if (this->instances_[k]->state == eConfigured ||
              this->instances_[k]->state == ePassive)
            {
              Components::CCMObject_var comp (Components::CCMObject::_narrow (this->instances_[k]->ref));
              cont->activate_component (comp.in ());
              this->instances_[k]->state = eActive;
            }
          else
            {
              if (!error)
                {
                  error = true;
                  exception.name = this->plan_.instance[this->instances_[k]->idd_idx].name.in ();
                  exception.reason = "Attempting to activate component that is not configured or passive.\n";
                }
            }

          DANCE_DEBUG ((LM_INFO, DLINFO "NodeApplication_Impl::start - "
                        "Component %C successfully activated.\n",
                        this->plan_.instance[this->instances_[k]->idd_idx].name.in ()));
        }
      catch (const CORBA::Exception &ex)
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::start - "
                        "Caught CORBA exception from ccm_activate on component %C: %C\n",
                        this->plan_.instance[this->instances_[k]->idd_idx].name.in (),
                        ex._info ().c_str ()));
          if (!error)
            {
              error = true;
              exception.name = this->plan_.instance[this->instances_[k]->idd_idx].name.in ();
              exception.reason = ex._info ().c_str ();
            }
        }
      catch (...)
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::start - "
                        "Caught unknown C++ exception from ccm_activate on component %C\n",
                        this->plan_.instance[this->instances_[k]->idd_idx].name.in ()));
          if (!error)
            {
              error = true;
              exception.name = this->plan_.instance[this->instances_[k]->idd_idx].name.in ();
              exception.reason = "Unknown C++ exception";
            }
        }
    }

  if (error)
    throw exception;
}

void
NodeApplication_Impl::install_home (Container &cont, Instance &inst)
{
  DANCE_TRACE( "NodeApplication_Impl::install_home");

  const ::Deployment::MonolithicDeploymentDescription &mdd = this->plan_.implementation[inst.mdd_idx];
  const ::Deployment::InstanceDeploymentDescription &idd = this->plan_.instance[inst.idd_idx];

  DANCE_DEBUG ((LM_DEBUG, DLINFO "NodeApplication_Impl::install_home - "
                "Starting installation of home %C on node %C\n",
                idd.name.in (), idd.node.in ()));

  this->instances_[inst.idd_idx] = &inst;

  // need to get significant property values
  const char *entrypt = 0;
  get_property_value (DAnCE::HOME_FACTORY, mdd.execParameter, entrypt);
  
  if (entrypt == 0)
    {
      DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::install_home - "
                    "Unable to find home factory property on home %C\n",
                    idd.name.in ()));
      throw ::Deployment::InvalidComponentExecParameter (mdd.name.in (),
                                                         "No 'home factory' property present on MDD\n");
    }

  // @@TODO: Perhaps need better way to do this.
  Components::ConfigValues config;
  config.length (mdd.execParameter.length () + idd.configProperty.length ());
  CORBA::ULong pos (0);

  for (CORBA::ULong i = 0; i < mdd.execParameter.length (); ++i)
    {
      DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplication_Impl::install_home - "
                    "Inserting value for execParameter %C\n", mdd.execParameter[i].name.in ()));
      config[pos++] = new CIAO::ConfigValue_impl (mdd.execParameter[i].name.in (),
                                                  mdd.execParameter[i].value);
    }

  for (CORBA::ULong i = 0; i < idd.configProperty.length (); ++i)
    {
      DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplication_Impl::install_home - "
                    "Inserting value for configProperty %C\n", idd.configProperty[i].name.in ()));
      config[pos++] =  new CIAO::ConfigValue_impl (idd.configProperty[i].name.in (),
                                                   idd.configProperty[i].value);
    }

  try
    {
      DANCE_DEBUG ((LM_DEBUG, DLINFO "NodeApplication_Impl::install_home - "
                    "Calling install_home on container.  Home id '%C', entrypt '%C', "
                    "length of config values is %u\n",
                    idd.name.in (), entrypt, config.length ()));

      ::Components::CCMHome_var home = cont.ref->install_home (idd.name.in (),
                                                               entrypt,
                                                               config);

      if (CORBA::is_nil (home))
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::install_home - "
                        "Got nil object reference from container while installing home %C on node %C,"
                        "throwing PlanError\n",
                        idd.name.in (), idd.node.in ()));
          throw ::Deployment::PlanError (idd.name.in (),
                                         "Nil object reference returned from conainer");
        }

      DANCE_DEBUG ((LM_INFO, DLINFO  "NodeApplication_Impl::install_home - "
                    "Home '%C' on node '%C' successfully installed\n",
                    idd.name.in (), idd.node.in ()));

      inst.ref = CORBA::Object::_narrow (home);

      DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplication_Impl::install_home - "
                    "Populating attributes for home %C\n",
                    idd.name.in ()));


      ComponentAttributesSetter::SetComponentAttributes (idd.name.in (),
                                                         inst.ref.in (),
                                                         idd.configProperty,
                                                         this->orb_.in ());

      inst.state = eInstalled;
    }
  catch (const Components::InvalidConfiguration &)
    {
      DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::install_home - "
                    "Error creating home %C on node %C, caught InvalidConfiguration.  Throwing exception\n",
                    idd.name.in (), idd.node.in ()));
      throw ::Deployment::InvalidProperty (idd.name.in (),
                                     "Invalid configuration exception from container");
    }
  catch (const CORBA::Exception &ex)
    {
      DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::install_home - "
                    "Caught CORBA exception while installing home %C: %C\n",
                    idd.name.in (),
                    ex._info ().c_str ()));
      throw ::Deployment::StartError (idd.name.in (),
                                      ex._info ().c_str ());
    }
  catch (...)
    {
      DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::install_home - "
                    "Caught unknown C++ exception while installing home %C\n",
                    idd.name.in ()));
      throw ::Deployment::StartError (idd.name.in (),
                                      "Unknown C++ exception");
    }
}

void
NodeApplication_Impl::install_component (Container &cont, Instance &inst)
{
  DANCE_TRACE( "NodeApplication_Impl::install_component");

  const ::Deployment::MonolithicDeploymentDescription &mdd = this->plan_.implementation[inst.mdd_idx];
  const ::Deployment::InstanceDeploymentDescription &idd = this->plan_.instance[inst.idd_idx];

  DANCE_DEBUG ((LM_DEBUG, DLINFO "NodeApplication_Impl::install_home - "
                "Starting installation of home %C on node %C\n",
                idd.name.in (), idd.node.in ()));

  this->instances_[inst.idd_idx] = &inst;

  const char *entrypt = 0;
  get_property_value (DAnCE::COMPONENT_FACTORY, mdd.execParameter, entrypt);

  if (entrypt == 0)
    {
      DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::install_component - "
                    "Unable to find component factory property on component %C\n",
                    idd.name.in ()));
      throw ::Deployment::InvalidComponentExecParameter (mdd.name.in (),
                                                         "No 'component factory' property present on MDD\n");
    }

  // @@TODO: Perhaps need better way to do this.
  Components::ConfigValues config;
  config.length (mdd.execParameter.length () + idd.configProperty.length ());
  CORBA::ULong pos (0);

    for (CORBA::ULong i = 0; i < mdd.execParameter.length (); ++i)
    {
      DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplication_Impl::install_component - "
                    "Inserting value for execParameter %C\n", mdd.execParameter[i].name.in ()));
      config[pos++] = new CIAO::ConfigValue_impl (mdd.execParameter[i].name.in (),
                                                  mdd.execParameter[i].value);
    }

  for (CORBA::ULong i = 0; i < idd.configProperty.length (); ++i)
    {
      DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplication_Impl::install_component - "
                    "Inserting value for configProperty %C\n", idd.configProperty[i].name.in ()));
      config[pos++] =  new CIAO::ConfigValue_impl (idd.configProperty[i].name.in (),
                                                   idd.configProperty[i].value);
    }

  ::CIAO::Deployment::Container_var ciao_cont = ::CIAO::Deployment::Container::_narrow (cont.ref.in ());

  if (CORBA::is_nil (ciao_cont.in ()))
    {
      DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::install_component - "
                    "Unable to narrow container assigned for component instance %C to one that supports "
                    "un-homed components.\n"));
      throw ::Deployment::PlanError (idd.name.in (),
                                     "Hosting container does not support unhomed components.\n");
    }

  try
    {
      DANCE_DEBUG ((LM_DEBUG, DLINFO "NodeApplication_Impl::install_component - "
                    "Calling install_component on container.  Component id '%C', entrypt '%C', "
                    "length of config values is %u\n",
                    idd.name.in (), entrypt, config.length ()));

      ::Components::CCMObject_var comp = ciao_cont->install_component (idd.name.in (),
                                                                       entrypt,
                                                                       config);

      if (CORBA::is_nil (comp))
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::install_component - "
                        "Got nil object reference from container while installing component %C on node %C,"
                        "throwing PlanError\n",
                        idd.name.in (), idd.node.in ()));
          throw ::Deployment::PlanError (idd.name.in (),
                                         "Nil object reference returned from install_component on conainer");
        }

      DANCE_DEBUG ((LM_INFO, DLINFO  "NodeApplication_Impl::install_component - "
                    "Component '%C' on node '%C' successfully installed\n",
                    idd.name.in (), idd.node.in ()));

      inst.ref = CORBA::Object::_narrow (comp);

      DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplication_Impl::install_component - "
                    "Populating attributes for home %C\n",
                    idd.name.in ()));


      ComponentAttributesSetter::SetComponentAttributes (idd.name.in (),
                                                         inst.ref.in (),
                                                         idd.configProperty,
                                                         this->orb_.in ());

      inst.state = eInstalled;
    }
    catch (const Components::InvalidConfiguration &)
    {
      DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::install_component - "
                    "Error creating component %C on node %C, caught InvalidConfiguration.  Throwing exception\n",
                    idd.name.in (), idd.node.in ()));
      throw ::Deployment::InvalidProperty (idd.name.in (),
                                           "Invalid configuration exception from container");
    }
  catch (const CORBA::Exception &ex)
    {
      DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::install_component - "
                    "Caught CORBA exception while installing component %C: %C\n",
                    idd.name.in (),
                    ex._info ().c_str ()));
      throw ::Deployment::StartError (idd.name.in (),
                                      ex._info ().c_str ());
    }
  catch (...)
    {
      DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::install_component - "
                    "Caught unknown C++ exception while installing component %C\n",
                    idd.name.in ()));
      throw ::Deployment::StartError (idd.name.in (),
                                      "Unknown C++ exception");
    }
}

void
NodeApplication_Impl::install_homed_component (Container &cont, Instance &inst)
{
  DANCE_TRACE("NodeApplication_Impl::install_homed_component (unsigned int index)");

  //const ::Deployment::MonolithicDeploymentDescription &mdd = this->plan_.implementation[inst.mdd_idx];
  const ::Deployment::InstanceDeploymentDescription &idd = this->plan_.instance[inst.idd_idx];
  this->instances_[inst.idd_idx] = &inst;

  DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplication_Impl::install_homed_component - "
                "Starting installation of homed component %C on node %C\n",
                idd.name.in (),
                idd.node.in ()));

  const char *home_id = 0;
  get_property_value (DAnCE::EXPLICIT_HOME, idd.configProperty, home_id);

  DANCE_DEBUG ((LM_DEBUG, DLINFO "NodeApplication_Impl::install_homed_component - "
                "Property %C has value %C\n",
                DAnCE::EXPLICIT_HOME, home_id));


  if (home_id == 0)
    {
      DANCE_ERROR ((LM_ERROR, DLINFO "Nodeapplication_Impl::install_homed_component - "
                    "Apparent homed component %C lacks a %C configProperty, aborting installation\n",
                    idd.name.in (), DAnCE::EXPLICIT_HOME));
      throw ::Deployment::PlanError (idd.name.in (),
                                     "No explicit home  property on component requiring explicit home.");
    }

  DANCE_DEBUG ((LM_DEBUG, DLINFO "NodeApplication_Impl::install_homed_component - "
                "Found explicit home property '%C' for component '%C'\n",
                home_id,
                idd.name.in ()));

  Instance *home_inst (0);

  for (size_t i = 0; i < cont.homes.size (); ++i)
    {
      if (ACE_OS::strcmp (home_id,
                          this->plan_.instance[cont.homes[i].idd_idx].name.in ()) == 0)
        {
          DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplication_Impl::install_homed_component - "
                        "Found home designated for component '%C'\n",
                        idd.name.in ()));
          home_inst = &cont.homes[i];
          break;
        }
    }

  if (home_inst == 0)
    {
      DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::install_homed_component - "
                    "Unable to find home instance matching %C allocated to same container as component %C\n",
                    home_id,
                    idd.name.in ()));
      throw Deployment::InvalidProperty (idd.name.in (),
                                         "Unable to find valid home allocated to same container.");
    }

  Components::KeylessCCMHome_var home =
    Components::KeylessCCMHome::_narrow (home_inst->ref.in ());

  if (CORBA::is_nil (home.in ()))
    {
      DANCE_ERROR ((LM_ERROR,  DLINFO "NodeApplication_Impl::install_homed_component - "
                    "Object reference for home '%C' for homed component '%C' is nil\n",
                    home_id,
                    idd.name.in ()));
      throw Deployment::PlanError (idd.name.in (),
                                   "Installed home for explicitly homed component has nil object reference\n");
    }

  try
    {
      inst.home = home_inst;
      DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplication_Impl::install_homed_component - "
                    "Invoking create_component on home %C for component %C\n",
                    home_id,
                    idd.name.in ()));

      Components::CCMObject_var ccm_obj = home->create_component ();

      if (CORBA::is_nil (ccm_obj))
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::install_homed_component - "
      "Received nil component reference from create_component on home %C "
      " while creating component %C\n",
                        home_id, idd.name.in ()));
          throw Deployment::StartError (idd.name.in (),
                                        "Home for explicitly homed component returned nil");
        }

      inst.ref = CORBA::Object::_narrow (ccm_obj.in ());
      DANCE_DEBUG ((LM_INFO, DLINFO  "NodeApplication_Impl::install_homed_component - "
                    "Component %C successfully installed in home %C\n",
                    idd.name.in (),
                    home_id));

      DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplication_Impl::install_home - "
                    "Populating attributes for component %C\n",
                    idd.name.in ()));
      ComponentAttributesSetter::SetComponentAttributes (idd.name.in (),
                                                        inst.ref.in (),
                                                        idd.configProperty,
                                                        this->orb_.in ());
      inst.state = eInstalled;
    }
  catch (const Components::CreateFailure &)
    {
      DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::install_homed_component - "
                    "Caught CreateFailure exception from home '%C' while installing component '%C'\n",
                    home_inst,
                    idd.name.in ()));
      throw Deployment::StartError (idd.name.in (),
                                    "Caught CreateFailure exception");
    }
  catch (const CORBA::Exception &ex)
    {
      DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::install_homed_component - "
                    "Caught CORBA exception while installing component %C in home %C: %C\n",
                    idd.name.in (),
                    home_id,
                    ex._info ().c_str ()));
      throw ::Deployment::StartError (idd.name.in (),
                                      ex._info ().c_str ());
    }
  catch (...)
    {
      DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::install_homed_component - "
                    "Caught unknown C++ exception while installing component %C in home  %C\n",
                    idd.name.in (),
                    home_id));
      throw ::Deployment::StartError (idd.name.in (),
                                      "Unknown C++ exception");
    }
}

void
NodeApplication_Impl::create_component_server (size_t index)
{
  DANCE_TRACE ("NodeApplication_Impl::create_component_server");

  ComponentServer &server = this->servers_[index];

    try
    {
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_component_Server - "
                   "creating component server %u\n", index));
      ::Components::ConfigValues config_values;
      server.ref = this->activator_->create_component_server (config_values);
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_component_server - "
                   "component server created\n"));
    }
  catch (const ::Components::CreateFailure& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO "NodeApplication_impl::create_container - "
                   "Components::Deployment::ServerActivator_var::create_component_server() "
                   "returned ::Components::CreateFailure exception\n"));
      throw ::Deployment::StartError("",
                                     "Received a ::Components::CreateFailure exception while creating component server.");
    }
  catch (const ::Components::Deployment::InvalidConfiguration& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO "NodeApplication_impl::create_container - "
                   "Components::Deployment::ServerActivator_var::create_component_server() "
                   "returned ::Components::Deployment::InvalidConfiguration exception\n"));
      throw ::Deployment::InvalidProperty("",
                                          "::Components::Deployment::InvalidConfiguration exception caught while creating server");
    }

    try
      {
        for (size_t i = 0; i < server.containers.size (); ++i)
          {
            this->create_container (index, i);
          }
      }
    catch (...)
      {
        DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_impl::create_container - "
                      "Caught exception whilst creating container; re-throwing.\n"));
        throw;
      }
}

void
NodeApplication_Impl::store_instance_ior (Instance &inst)
{
  DANCE_TRACE ("NodeApplication_impl::store_instance_ior");

  const char *name = 0;

  if (get_property_value (DAnCE::REGISTER_NAMING,
                          this->plan_.instance[inst.idd_idx].configProperty,
                          name))
    {
      DANCE_DEBUG ((LM_DEBUG, DLINFO "NodeApplication_impl::store_instance_ior - "
                    "Storing instance '%C' object reference in Naming Service as %C",
                    this->plan_.instance[inst.idd_idx].name.in (),
                    name));

      Name_Utilities::bind_object (name, inst.ref.in (), this->instance_nc_.in ());
    }

  if (get_property_value (DAnCE::INSTANCE_IOR_FILE,
                          this->plan_.instance[inst.idd_idx].configProperty,
                          name))
    {
      CORBA::String_var ior = this->orb_->object_to_string (inst.ref.in ());
      Name_Utilities::write_ior (name, ior.in ());
    }
}

void
NodeApplication_Impl::create_container (size_t server, size_t cont_idx)
{
  DANCE_TRACE ("NodeApplication_impl::create_container");

  Container &container = this->servers_[server].containers[cont_idx];

  DANCE_DEBUG ((LM_TRACE, "NodeApplication_impl::create_container - "
                "Creating container\n"));
  // TODO: Need to create configvalues
  Components::ConfigValues cvs;

  container.ref = this->servers_[server].ref->create_container (cvs);

  DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplication_impl::create_container - "
                "Configuring %u homes on container %u on server %u\n",
                container.homes.size (),
                server, cont_idx));

  // Configure homes first
  for (size_t i = 0; i < container.homes.size (); ++i)
    {
      this->install_home (container, container.homes[i]);
      this->store_instance_ior (container.homes[i]);
    }

  DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplication_impl::create_container - "
                "Configuring %u components on container %u on server %u\n",
                container.components.size (),
                server, cont_idx));

  // Configure components
  for (size_t i = 0; i < container.components.size (); ++i)
    {
      switch (container.components[i].type)
        {
        case eComponent:
          this->install_component (container, container.components[i]);
          break;
        case eHomedComponent:
          this->install_homed_component (container, container.components[i]);
          break;
        default:
          break;
        }
      this->store_instance_ior (container.components[i]);
    }
}

/*
void
NodeApplication_Impl::create_container (unsigned int index)
{
  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_container - started\n"));

  Components::ConfigValues config_values;
  ACE_CString processDest;
  CORBA::Any_var feature_any;

  this->create_config_values (this->plan_.instance[index].configProperty,
                              eCreateComponentServer,
                              config_values);

  ::Components::Deployment::ComponentServer_var compServer;


  // COMPONENT_KIND
  this->create_config_values (this->plan_.implementation[this->plan_.instance[index].implementationRef].execParameter,
                              eCreateContainer,
                              config_values);
  try
    {
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_container - "
                   "creating container for destination: %C\n", processDest.c_str()));
      this->containers_.rebind (processDest.c_str(), compServer->create_container (config_values));
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_container - container created\n"));
    }
  catch (::Components::CreateFailure& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_container - "
                   "::Components::Deployment::ComponentServer_var::create_container() "
                   "returned ::Components::CreateFailure exception\n"));
      throw Deployment::StartError();
    }
  catch (::Components::Deployment::InvalidConfiguration& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_container - "
                   "::Components::Deployment::ComponentServer_var::create_container() "
                   "returned ::Components::Deployment::InvalidConfiguration exception\n"));
      throw ::Deployment::InvalidProperty();
    }

  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_container - finished\n"));
}
*/

void
NodeApplication_Impl::init_components()
{
  DANCE_TRACE ("NodeApplication_impl::init_components");

  Components::ConfigValues config_values;
  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::init_components - "
               "Configuring %u component/home instances\n",
               this->plan_.instance.length()));

  // @@TODO:  For the moment, we are only going to support a single component server and container.
  // in the future, we will need to determine how many component servers we need.
  if (this->plan_.instance.length () > 0)
    {
      ComponentServer server;
      server.containers.size (1);
      this->servers_.size (1);
      this->servers_[0] = server;
    }

  for (unsigned int i = 0; i < this->plan_.instance.length(); i++)
    {
      try
        {
          CORBA::ULong impl = this->plan_.instance[i].implementationRef;
          // Determine type from implementation properties, then from instance properties.
          EInstanceType type =
            this->get_instance_type (this->plan_.implementation[impl].execParameter);
          if (type == eInvalid)
            type = this->get_instance_type (this->plan_.instance[i].configProperty);

          switch (type)
            {
            case eHome:
              {
                DANCE_DEBUG ((LM_DEBUG, DLINFO "NodeApplication_impl::init_components - "
                              "Allocating instance %C as a home\n",
                              this->plan_.instance[i].name.in ()));

                size_t pos = this->servers_[0].containers[0].homes.size ();
                this->servers_[0].containers[0].homes.size (pos + 1);
                this->servers_[0].containers[0].homes[pos] = Instance (eHome,
                                                                       &this->servers_[0].containers[0],
                                                                       i,
                                                                       this->plan_.instance[i].implementationRef);
                //this->instances_[i] = &this->servers_[0].containers[0].homes[pos];
                break;
              }
            case eComponent:
              {
                DANCE_DEBUG ((LM_DEBUG, DLINFO "NodeApplication_impl::init_components - "
                              "Allocating instance %C as a standalone component\n",
                              this->plan_.instance[i].name.in ()));
                size_t pos = this->servers_[0].containers[0].homes.size ();
                this->servers_[0].containers[0].components.size (pos + 1);
                this->servers_[0].containers[0].components[pos] = Instance (eComponent,
                                                                            &this->servers_[0].containers[0],
                                                                            i,
                                                                            this->plan_.instance[i].implementationRef);
                //this->instances_[i] = &this->servers_[0].containers[0].components[pos];
                break;
              }
            case eHomedComponent:
              {
                DANCE_DEBUG ((LM_DEBUG, DLINFO "NodeApplication_impl::init_components - "
                              "Allocating instance %C as a home managed component\n",
                              this->plan_.instance[i].name.in ()));
                size_t pos = this->servers_[0].containers[0].components.size ();
                this->servers_[0].containers[0].components.size (pos + 1);
                this->servers_[0].containers[0].components[pos] = Instance (eHomedComponent,
                                                                            &this->servers_[0].containers[0],
                                                                            i,
                                                                            this->plan_.instance[i].implementationRef);
                //this->instances_[i] = &this->servers_[0].containers[0].components[pos];
                break;
              }
            default:
              {
                DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::init_components - "
                             "get_instance_type function returned invalid instance type\n"));
                throw ::Deployment::InvalidProperty (this->plan_.instance[i].name.in (),
                                                     "Unable to affirmatively determine instance type");
              }
            } // switch
        } catch (...)
        {
          DANCE_ERROR((LM_ERROR, DLINFO "Exception was thrown while sorting instance \"%C\".\n", this->plan_.instance[i].name.in()));
          throw;
        }
    }

  DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplication_impl::init_components - "
                "Creating component servers and installing components.\n"));
  for (size_t i = 0; i < this->servers_.size (); ++i)
    {
      DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplication_impl::init_components - "
                    "Creating component server with index %u\n", i));
      this->create_component_server (i);
    }


  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::init_components - finished\n"));
}

void
NodeApplication_Impl::passivate_components()
{
  DANCE_TRACE ("NodeApplication_Impl::passivate_components()");

  bool error (false);
  Deployment::StopError exception ("unfilled", "unfilled passivate components");

  for (size_t k = 0; k < this->instances_.size (); ++k)
    {
      if (this->instances_[k]->type == eHome ||
          this->instances_[k]->type == eInvalid)
        continue;


      try
        {
          CIAO::Deployment::Container_var cont =
            CIAO::Deployment::Container::_narrow (this->instances_[k]->container->ref.in());

          if (CORBA::is_nil (this->instances_[k]->container->ref.in ()))
            {
              DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::start - "
                            "Failed to narrow object reference for container managing "
                            "component instance %C to a CIAO container reference\n",
                            this->plan_.instance[this->instances_[k]->idd_idx].name.in ()));
              test_and_set_exception (error, exception,
                                      this->plan_.instance[this->instances_[k]->idd_idx].name.in (),
                                      "Failed to narrow managing container to CIAO container type");
              continue;
            }

          if (this->instances_[k]->state == eActive)
            {
              Components::CCMObject_var comp (Components::CCMObject::_narrow (this->instances_[k]->ref));
              cont->passivate_component (comp.in ());
              this->instances_[k]->state = ePassive;
            }
          else
            {
              DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::start - "
                            "Attempting to passivate non-active component %C\n",
                            this->plan_.instance[this->instances_[k]->idd_idx].name.in ()));
              test_and_set_exception (error, exception,
                                      this->plan_.instance[this->instances_[k]->idd_idx].name.in (),
                                      "Attempting to passivate non-active component.");
              continue;
            }

          DANCE_DEBUG ((LM_INFO, DLINFO "NodeApplication_Impl::passivate_components - "
                        "Component %C successfully passivated.\n",
                        this->plan_.instance[this->instances_[k]->idd_idx].name.in ()));
        }
      catch (const CORBA::Exception &ex)
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::passivate_components - "
                        "Caught CORBA exception from ccm_passivate on component %C: %C\n",
                        this->plan_.instance[this->instances_[k]->idd_idx].name.in (),
                        ex._info ().c_str ()));
          if (!error)
            {
              error = true;
              exception.name = this->plan_.instance[this->instances_[k]->idd_idx].name.in ();
              exception.reason = ex._info ().c_str ();
            }
        }
      catch (...)
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::passivate_components - "
                        "Caught unknown C++ exception from ccm_passivate on component %C\n",
                        this->plan_.instance[this->instances_[k]->idd_idx].name.in ()));
          if (!error)
            {
              error = true;
              exception.name = this->plan_.instance[this->instances_[k]->idd_idx].name.in ();
              exception.reason = "Unknown C++ exception";
            }
        }
    }

  if (error)
    throw exception;
}

void
NodeApplication_Impl::remove_components()
{
  DANCE_TRACE ("NodeApplication_Impl::remove_components()");

  bool error (false);
  Deployment::StopError exception ("unfilled", "unfilled remove_components");

  // Removing components first.
  for (size_t k = 0; k < this->instances_.size (); ++k)
    {
      try
        {
          if (this->instances_[k]->type == eInvalid ||
              this->instances_[k]->type == eHome)
            continue;

          if (this->instances_[k]->state != ePassive)
            DANCE_DEBUG ((LM_WARNING, DLINFO "NodeApplication_Impl::remove_components - "
                          "Attempting to remove component that is not passive.\n"));

          if (this->instances_[k]->type == eComponent)
            {
              if (this->instances_[k]->container == 0)
                {
                  DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::remove_components - "
                                "Container element in component data structure for '%C' is nill\n",
                                this->plan_.instance[this->instances_[k]->idd_idx].name.in ()
                                ));

                  test_and_set_exception (error,
                                          exception,
                                          this->plan_.instance[this->instances_[k]->idd_idx].name.in (),
                                          "Container element in component data structure is nil\n");
                  this->instances_[k]->state = eInvalidState;
                  continue;
                }

              CIAO::Deployment::Container_var ciao_container =
                CIAO::Deployment::Container::_narrow (this->instances_[k]->container->ref.in ());

              if (CORBA::is_nil (ciao_container.in ()))
                {
                  DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::remove_components - "
                                "Attempted to remove un-homed component from unsupported container\n"));

                  test_and_set_exception (error, exception,
                                          this->plan_.instance[this->instances_[k]->idd_idx].name.in (),
                                          "Attempted to remove un-homed component from unsupported container\n");
                  this->instances_[k]->state = eInvalidState;
                  continue;
                }

              ciao_container->remove_component (Components::CCMObject::_narrow (this->instances_[k]->ref.in ()));
            }
          else
            {
              Components::CCMHome_var home =
                Components::CCMHome::_narrow (this->instances_[k]->home->ref.in ());

              if (CORBA::is_nil (home))
                {
                  DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::remove_components - "
                                "Object reference for home %C, managing component %C is nil.\n",
                                this->plan_.instance[this->instances_[k]->home->idd_idx].name.in (),
                                this->plan_.instance[this->instances_[k]->idd_idx].name.in ()));
                  test_and_set_exception (error,
                                          exception,
                                          this->plan_.instance[this->instances_[k]->idd_idx].name.in (),
                                          "Managing home's object reference is invalid.");
                  this->instances_[k]->state = eInvalidState;
                  continue;
                }

              home->remove_component (Components::CCMObject::_narrow (this->instances_[k]->ref.in ()));
            }


          this->instances_[k]->state = eRemoved;

          DANCE_DEBUG ((LM_INFO, DLINFO "NodeApplication_Impl::remove_components - "
                        "Component %C successfully removed.\n",
                        this->plan_.instance[this->instances_[k]->idd_idx].name.in ()));

        }
      catch (const CORBA::Exception &ex)
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::remove_components - "
                        "Caught CORBA exception removing on component %C: %C\n",
                        this->plan_.instance[this->instances_[k]->idd_idx].name.in (),
                        ex._info ().c_str ()));
          test_and_set_exception (error,
                                  exception,
                                  this->plan_.instance[this->instances_[k]->idd_idx].name.in (),
                                  exception.reason = ex._info ().c_str ());
        }
      catch (...)
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::remove_components - "
                        "Caught unknown C++ exception from ccm_remove on component %C\n",
                        this->plan_.instance[this->instances_[k]->idd_idx].name.in ()));
          test_and_set_exception (error,
                                  exception,
                                  this->plan_.instance[this->instances_[k]->idd_idx].name.in (),
                                  "Unknown C++ exception");
        }
    }

  for (size_t k = 0; k < this->instances_.size (); ++k)
    {
      try
        {
          if (this->instances_[k]->type != eHome)
            continue;

          DANCE_ERROR ((LM_TRACE, DLINFO "NodeApplication_Impl::remove_components - "
                        "Removing home %C\n",
                        this->plan_.instance[this->instances_[k]->idd_idx].name.in ()));

          if (this->instances_[k]->container == 0 ||
              CORBA::is_nil (this->instances_[k]->container->ref.in ()))
            {
              DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::remove_components - "
                            "Home %C lacks an associated container reference\n",
                            this->plan_.instance[this->instances_[k]->idd_idx].name.in ()));
              test_and_set_exception (error,
                                      exception,
                                      this->plan_.instance[this->instances_[k]->idd_idx].name.in (),
                                      "Didn't have a valid container reference");
              continue;
            }

          Components::CCMHome_var home = Components::CCMHome::_narrow (this->instances_[k]->ref.in ());
          this->instances_[k]->container->ref->remove_home (home.in ());

          DANCE_DEBUG ((LM_INFO, DLINFO "NodeApplication_Impl::remove_components - "
                       "Successfully uninstalled home %C\n",
                       this->plan_.instance[this->instances_[k]->idd_idx].name.in ()));
        }
      catch (const CORBA::Exception &ex)
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::remove_components - "
                        "Caught CORBA exception removing home %C: %C\n",
                        this->plan_.instance[this->instances_[k]->idd_idx].name.in (),
                        ex._info ().c_str ()));
          test_and_set_exception (error,
                                  exception,
                                  this->plan_.instance[this->instances_[k]->idd_idx].name.in (),
                                  exception.reason = ex._info ().c_str ());
        }
      catch (...)
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::remove_components - "
                        "Caught unknown C++ exception from while removing home %C\n",
                        this->plan_.instance[this->instances_[k]->idd_idx].name.in ()));
          test_and_set_exception (error,
                                  exception,
                                  this->plan_.instance[this->instances_[k]->idd_idx].name.in (),
                                  "Unknown C++ exception");
        }
    }

if (error)
  throw exception;
}

NodeApplication_Impl::EInstanceType
NodeApplication_Impl::get_instance_type (const Deployment::Properties& prop) const
{
  DANCE_TRACE ("NodeApplication_Impl::get_instance_type");

  for (CORBA::ULong i = 0; i < prop.length (); ++i)
    {
      DANCE_DEBUG ((LM_TRACE, DLINFO  "NodeApplication_Impl::get_instance_type - "
                    "Checking property %C\n",
                    prop[i].name.in ()));

      if (ACE_OS::strcmp (prop[i].name.in (),
                          DAnCE::HOME_FACTORY) == 0)
        {
          DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplication_Impl::get_instance_type - "
                        "Found Home type\n"));
          return eHome;
        }
      if (ACE_OS::strcmp (prop[i].name.in (),
                          DAnCE::COMPONENT_FACTORY) == 0)
        {
          DANCE_DEBUG  ((LM_TRACE, DLINFO "NodeApplication_Impl::get_instance_type - "
                         "Found unmanaged component type.\n"));
          return eComponent;
        }
      if (ACE_OS::strcmp (prop[i].name.in (),
                          DAnCE::EXPLICIT_HOME) == 0)
        {
          DANCE_DEBUG ((LM_TRACE, DLINFO  "NodeApplication_Impl::get_instance_type - "
                        "Found explicit home component type.\n"));
            return eHomedComponent;
        }
    }

  DANCE_ERROR ((LM_INFO, DLINFO  "NodeApplication_Impl::get_instance_type - "
                "Unable to determine instance type\n"));
  return eInvalid;
}

void
NodeApplication_Impl::create_config_values (const Deployment::Properties& /*prop*/,
                                            const ERequestType request,
                                            Components::ConfigValues& /*cfg*/) const
{
  DANCE_TRACE ("NodeApplication_Impl::create_config_values");

  //unsigned int ind = 0;
  CORBA::Any_var feature_any;
  switch (request)
    {
    case eCreateComponentServer:
      {
        break;
      }
    case eCreateContainer:
      {

        break;
      }
    case eInstallHome:
      {
        break;
      }
    default:
      {
        DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_config_values - "
                     "request is not a know type: eCreateComponentServer, eCreateContainer, "
                     "eInstallHome, eCreateComponentWithConfigValues\n"));
        throw ::Deployment::InvalidProperty("",
                                            "Invalid creation type for filling in config values");
      }
    }
}

Deployment::Connections*
NodeApplication_Impl::getAllConnections()
{
  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::getAllConnections - started\n"));

  Deployment::Connections_var conn;
  ACE_NEW_THROW_EX (conn,
                    Deployment::Connections (),
                    CORBA::NO_MEMORY ());
  unsigned int index = 0;
  /*
  this->redirection_.registration_start (this->node_name_, this->plan_.UUID.in());

  CCMObjects registration
  for (TComponents::iterator it = this->components_.begin();
       !it.done();
       ++it)
    {
      this->redirection_.registration (this->node_name_,
                                       this->plan_.UUID.in(),
                                       (*it).ext_id_,
                                       (*it).int_id_.in());

    }*/
  for (unsigned int i = 0; i < this->plan_.connection.length(); i++)
    {
      for (unsigned int j = 0; j < this->plan_.connection[i].internalEndpoint.length(); j++)
        {
          if (this->plan_.connection[i].internalEndpoint[j].provider)
            {
              index = conn->length ();
              conn->length (index + 1);
              (*conn) [index].name = CORBA::string_dup (this->plan_.connection[i].name.in());

              ACE_CString inst_name =
                this->plan_.instance[this->plan_.connection[i].internalEndpoint[j].instanceRef].name.in();

              DANCE_DEBUG ((LM_DEBUG, DLINFO "NodeApplication_Impl::getAllConnections() - "
                            "Found provider '%C' for connection '%C'\n",
                            this->plan_.connection[i].name.in (),
                            inst_name.c_str ()));

              Components::CCMObject_var obj =
                Components::CCMObject::
                _narrow (this->instances_[this->plan_.connection[i].internalEndpoint[j].instanceRef]->ref.in ());

              (*conn) [index].endpoint.length (1L);
              //              ACE_CString path = CCMObjectLocator::TreeNode::mergePath (inst_name.c_str()
              //                                                                        , this->plan_.connection[i].internalEndpoint[j].portName.in());
              switch (this->plan_.connection[i].internalEndpoint[j].kind)
                {
                case Deployment::Facet:
                  {
                    try
                      {
                        ACE_CString name = this->plan_.connection[i].internalEndpoint[j].portName.in();
                        if (name.compare ("supports") != 0)
                          {
                            DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::getAllConnections - "
                                         "provide_facet for connection %C endpoint %C started\n",
                                         this->plan_.connection[i].name.in(),
                                         this->plan_.connection[i].internalEndpoint[j].portName.in()));
                            CORBA::String_var facet_name = CORBA::string_dup (name.c_str());
                            (*conn) [index].endpoint[0] = obj->provide_facet (facet_name.in());
                            DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::getAllConnections - "
                                         "provide_facet for connection %C endpoint %C finished\n",
                                         this->plan_.connection[i].name.in(),
                                         this->plan_.connection[i].internalEndpoint[j].portName.in()));
                          }
                        else
                          {
                            DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::getAllConnections - "
                                         "provide_facet for connection %C endpoint %C started\n",
                                         this->plan_.connection[i].name.in(),
                                         this->plan_.connection[i].internalEndpoint[j].portName.in()));
                            (*conn) [index].endpoint[0] = CORBA::Object::_duplicate (obj.in());
                          }
                        /*                        this->redirection_.registration (this->node_name_,
                                                         this->plan_.UUID.in(),
                                                         inst_name,
                                                         name,
                                                         (*conn) [index].endpoint[0].in());*/
                        //app_node.addChild(path.c_str(), obj->provide_facet(facet_name));
                        DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::getAllConnections - provide_facet finished\n"));
                      }
                    catch (const ::Components::InvalidName& )
                      {
                        DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::getAllConnections - "
                                     "Components::CCMObject_var::provide_facet() returned "
                                     "::Components::InvalidName exception for connection %C and port %C\n",
                                     this->plan_.connection[i].name.in (),
                                     this->plan_.connection[i].internalEndpoint[j].portName.in ()));
                        throw ::Deployment::InvalidProperty(this->plan_.connection[i].name.in (),
                                                            "Container returned InvalidName");
                      }
                    break;
                  }
                case Deployment::EventConsumer:
                  {
                    try
                      {
                        DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::getAllConnections - "
                                     "get_consumer for connection %C endpoint %C started\n",
                                     this->plan_.connection[i].name.in(),
                                     this->plan_.connection[i].internalEndpoint[j].portName.in()));
                        (*conn) [index].endpoint[0] = obj->get_consumer (this->plan_.connection[i].internalEndpoint[j].portName.in());
                        /*this->redirection_.registration (this->node_name_,
                                                         this->plan_.UUID.in(),
                                                         inst_name,
                                                         this->plan_.connection[i].internalEndpoint[j].portName.in(),
                                                         (*conn) [index].endpoint[0].in());*/

                        //app_node.addChild(path.c_str(), obj->get_consumer(this->plan_.connection[i].internalEndpoint[j].portName));
                        DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::getAllConnections - "
                                     "get_consumer finished\n"));
                      }
                    catch (const ::Components::InvalidName& )
                      {
                        DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::getAllConnections - "
                                     "Components::CCMObject_var::get_consumer() returned "
                                     "::Components::InvalidName exception for connection %C and port %C\n",
                                     this->plan_.connection[i].name.in (),
                                     this->plan_.connection[i].internalEndpoint[j].portName.in ()));
                        throw ::Deployment::InvalidProperty(this->plan_.connection[i].name.in (),
                                                            "Container returned InvalidName exception");
                      }
                    break;
                  }
                default:
                  {
                    DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::getAllConnections - "
                                 "Connection.InternalEndPoint.Kind is not a "
                                 "Deployment::Facet or Deployment::EventConsumer\n"));
                    throw ::Deployment::InvalidProperty(this->plan_.connection[i].name.in (),
                                                        "Invalid connection type, should be Facet or EventConsumer");
                  }
                }
              //              index++;
            }
        }
    }
  /*this->redirection_.registration_finish (this->node_name_, this->plan_.UUID.in());*/
  //this->node_.registerObjects(this->plan_.UUID.in(), app_node);
  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::getAllConnections - finished\n"));
  return conn._retn();
}

void
NodeApplication_Impl::finishLaunch (const Deployment::Connections & providedReference,
                                    ::CORBA::Boolean start)
{
  DANCE_TRACE ("NodeApplication_Impl::finishLaunch");

  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - "
               "started for connections sequence with length: %d\n",
               providedReference.length()));

#ifdef GEN_OSTREAM_OPS
  {
    std::ostringstream pr_stream;
    pr_stream << providedReference << std::endl;
    DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplication_impl::finishLaunch - Provided references: %s",
                  pr_stream.str ().c_str ()));
  }
#endif /* GEN_OSTREAM_OPS */

  for (unsigned int j = 0; j < this->plan_.connection.length(); ++j)
    {
      CORBA::ULong inst (this->plan_.connection[j].internalEndpoint[0].instanceRef);

      DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplication_impl::finishLaunch - "
                    "Connection %C, instance %u\n",
                    this->plan_.connection[j].name.in (),
                    inst));

#ifdef GEN_OSTREAM_OPS
      {
        std::ostringstream conn_stream;
        conn_stream << this->plan_.connection[j] << std::endl;
        DANCE_DEBUG ((LM_TRACE, "NodeApplication_impl::finishLaunch - Local connections: %s", conn_stream.str ().c_str ()));
      }
#endif /* GEN_OSTREAM_OPS  */

      Components::CCMObject_var obj =
        Components::CCMObject::
        _narrow (this->instances_[inst]->ref.in ());

      if (CORBA::is_nil (obj.in ()))
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_impl::finishLaunch - "
                        "Unable to narrow apparent component instance reference to CCMObject for instance '%C'\n",
                        this->plan_.instance[inst].name.in ()));
          throw Deployment::InvalidConnection (this->plan_.instance[inst].name.in (),
                                               "Unable to narrow apparent component instance reference to CCMObject\n");
        }

      const ::Deployment::PlanConnectionDescription &conn = this->plan_.connection[j];

      ACE_CString name = conn.name.in();

      for (unsigned int i = 0; i < providedReference.length(); ++i)
        {
          /*DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - "
                       "loop on all connections iteration %d for connection %C\n",
                       i,
                       providedReference[i].name.in()));*/

          if (name.compare (providedReference[i].name.in()) == 0)
            {
              try
                {
                  switch (conn.internalEndpoint[0].kind)
                    {
                    case Deployment::Facet:
                      {
                        DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - "
                                     "set for facet %C\n", name.c_str ()));
                        Components::CCMObject_var ext_inst;
                        try
                          {
                            if (0 == conn.externalReference.length())
                              {
                                if (conn.internalEndpoint.length () == 2 &&
                                    (conn.internalEndpoint[1].kind == Deployment::MultiplexReceptacle ||
                                     conn.internalEndpoint[1].kind == Deployment::SimplexReceptacle))
                                  {
                                    obj = Components::CCMObject::
                                      _narrow (this->instances_[conn.internalEndpoint[1].instanceRef]->ref.in ());

                                    this->connect_receptacle (obj.in (),
                                                              conn.internalEndpoint[1].portName.in(),
                                                              providedReference[i].endpoint[0].in());
                                  }
                                /*
                                  DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_impl::finishLaunch - "
                                  "Unsupported facet connection; lacks either external reference or "
                                  "multiple internalEndpoints.\n"));
                                  throw ::Deployment::StartError (name.c_str (),
                                  "Unsupported facet connection; lacks either external reference "
                                  "or multiple internalEndpoints.\n");
                                */
                                break;
                              }
                            CORBA::Object_var tmp =
                              this->orb_->string_to_object (conn.externalReference[0].location.in());
                            ext_inst = Components::CCMObject::_narrow (tmp);
                            if (CORBA::is_nil (ext_inst.in()))
                              {
                                DANCE_ERROR((LM_ERROR, DLINFO "NodeApplication_impl::finishLaunch - "
                                             "facet for %C can't be narrowed\n", name.c_str ()));
                                break;
                              }
                            this->connect_receptacle_ext (ext_inst,
                                                          conn.externalReference[0].portName.in(),
                                                          providedReference[i].endpoint[0].in());
                          }
                        catch (const CORBA::OBJECT_NOT_EXIST&)
                          {
                            // @@TODO: Shouldn't this be an error?!?
                            break;
                          }
                        catch (const CORBA::TRANSIENT&)
                          {
                            // @@TODO: Shouldn't this be an error?!?
                            break;
                          }

                        break;
                      }
                    case Deployment::EventConsumer:
                      {
                        DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - set for consumer\n"));
                        Components::CCMObject_var ext_inst;
                        try
                          {
                            if (0 == conn.externalReference.length())
                              {
                                break;
                              }
                            CORBA::Object_var tmp =
                              this->orb_->string_to_object (conn.externalReference[0].location.in());
                            ext_inst = Components::CCMObject::_narrow (tmp);
                            if (CORBA::is_nil (ext_inst.in()))
                              {
                                DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                                             "reference for %C can't be narrowed\n", name.c_str ()));
                                throw ::Deployment::InvalidConnection(conn.name.in (),
                                                                      "Couldn't narrow reference for external reference");
                                break;
                              }
                            try
                              {
                                // Check is connection kind is consumer to emitter?
                                this->connect_emitter_ext (ext_inst,
                                                           conn.externalReference[0].portName.in(),
                                                           providedReference[i].endpoint[0].in());
                              }
                            catch (const ::Components::InvalidName&)
                              {
                                // No this is consumer to publisher
                                this->connect_publisher (ext_inst,
                                                         conn.externalReference[0].portName.in(),
                                                         providedReference[i].endpoint[0].in());
                              }
                          }
                        catch (const CORBA::OBJECT_NOT_EXIST&)
                          {
                            break;
                          }
                        catch (const CORBA::TRANSIENT&)
                          {
                            break;
                          }
                        break;
                      }
                    case Deployment::MultiplexReceptacle:
                    case Deployment::SimplexReceptacle:
                      {
                        // What we should do with Cookie, returned from connect call???
                        DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - set for receptacle\n"));
                        this->connect_receptacle (obj.in(),
                                                  conn.internalEndpoint[0].portName.in(),
                                                  providedReference[i].endpoint[0].in());
                        break;
                      }
                    case Deployment::EventEmitter:
                      {
                        DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - set for emitter\n"));
                        this->connect_emitter (obj.in(),
                                               conn.internalEndpoint[0].portName.in(),
                                               providedReference[i].endpoint[0].in());
                        break;
                      }
                    case Deployment::EventPublisher:
                      {
                        DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - set for publisher\n"));
                        this->connect_publisher (obj.in(),
                                                 conn.internalEndpoint[0].portName.in(),
                                                 providedReference[i].endpoint[0].in());
                        break;
                      }
                    default:
                      {
                        DANCE_ERROR((LM_ERROR, DLINFO "NodeApplication_impl::finishLaunch - currect Connection.InternalEndPoint.Kind "
                                     "is not a Deployment::SimplexReceptacle, Deployment::EventEmitter, Deployment::EventPublisher "
                                     "(Connection:%C Kind:%i PortName:%C)\n",
                                     conn.name.in(),
                                     conn.internalEndpoint[0].kind,
                                     conn.internalEndpoint[0].portName.in()
                                     ));
                        throw ::Deployment::InvalidConnection(conn.name.in (),
                                                              "Invalid connection type, should be Receptacle or even producer.");
                      }//default
                    }//switch
                }
              catch (::Deployment::StartError &ex)
                {
                  DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_impl::finishLaunch - "
                                "Intercepted StartError exception while configuring %C conneciton, rethrowing\n",
                                name.c_str ()));
                  ex.name = name.c_str ();
                  throw;
                }
              catch (::Deployment::InvalidConnection &ex)
                {
                  DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_impl::finishLaunch - "
                                "Intercepted InvalidConnection exception while configuring %C conneciton, rethrowing\n",
                                name.c_str ()));
                  ex.name = name.c_str ();
                  throw;
                }
            }//if(name.compare(providedReference[i].name.in()) == 0)
        }//for ( unsigned int i = 0; i < providedReference.length(); ++i )
    }//for ( unsigned int j = 0; j < this->plan_.connection.length(); ++j )

  this->configuration_complete_components ();

  if (start)
    {
      this->start();
    }

  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - finished\n"));
}

Components::Cookie*
NodeApplication_Impl::connect_receptacle (Components::CCMObject_ptr inst,
                                          const ACE_CString& port_name,
                                          CORBA::Object_ptr facet)
{
  Components::Cookie* res = 0;
  try
    {
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - "
                   "connect SimplexReceptacle for %C started\n", port_name.c_str()));
      res = inst->connect (port_name.c_str(), facet);
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - connect finished\n"));
    }
  catch (const ::Components::InvalidName& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect() returned ::Components::InvalidName exception\n"));
      throw ::Deployment::StartError("",
                                     "Received InvalidName exception while connecting receptacle.");
    }
  catch (const ::Components::InvalidConnection& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect() returned ::Components::InvalidConnection exception\n"));
      throw ::Deployment::InvalidConnection("",
                                            "InvalidConnection caught while connecting receptacle.");
    }
  catch (const ::Components::AlreadyConnected& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect() returned ::Components::AlreadyConnected exception\n"));
      throw ::Deployment::InvalidConnection("",
                                            "Caught AlredyConnected exception while connecting receptacle");
    }
  catch (const ::Components::ExceededConnectionLimit& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect() returned ::Components::ExceededConnectionLimit exception\n"));
      throw ::Deployment::InvalidConnection("",
                                            "Caught ExceededConnectionLimit exception while connecting receptacle.");
    }
  return res;
}

Components::Cookie*
NodeApplication_Impl::connect_receptacle_ext (Components::CCMObject_ptr inst,
                                              const ACE_CString& port_name,
                                              CORBA::Object_ptr facet)
{
  Components::Cookie* res = 0;
  try
    {
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - "
                   "connect SimplexReceptacle for %C started\n", port_name.c_str()));
      res = inst->connect (port_name.c_str(), facet);
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - connect finished\n"));
    }
  catch (const ::Components::InvalidName& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect() returned ::Components::InvalidName exception\n"));
      throw ::Deployment::StartError("",
                                     "Caught InvalidName exception while connecting external receptacle.");
    }
  catch (const ::Components::InvalidConnection& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect() returned ::Components::InvalidConnection exception\n"));
      throw ::Deployment::InvalidConnection("",
                                            "Caught InvalidConnection exception while connecting external receptacle.");
    }
  catch (const ::Components::AlreadyConnected& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect() returned ::Components::AlreadyConnected exception\n"));
      throw ::Deployment::InvalidConnection("",
                                            "Caught AlreadyConnected exception while connecting external receptacle.");
    }
  catch (const ::Components::ExceededConnectionLimit& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect() returned ::Components::ExceededConnectionLimit exception\n"));
      throw ::Deployment::InvalidConnection("",
                                            "Caught ExceededConnectionLimit while connecting external receptacle.");
    }
  return res;
}

void
NodeApplication_Impl::connect_emitter (Components::CCMObject_ptr inst,
                                       const ACE_CString& port_name,
                                       CORBA::Object_ptr consumer)
{
  Components::EventConsumerBase_var event = Components::EventConsumerBase::_unchecked_narrow (consumer);
  try
    {
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - "
                   "connect_consumer for %C started\n", port_name.c_str()));
      inst->connect_consumer (port_name.c_str(), event);
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - "
                   "connect_consumer finished\n"));
    }
  catch (const ::Components::InvalidName& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect_consumer() returned ::Components::InvalidName exception\n"));
      throw ::Deployment::StartError("",
                                     "Caught InvalidName while connecting emitter.");
    }
  catch (const ::Components::AlreadyConnected& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect_consumer() returned ::Components::AlreadyConnected exception\n"));
      throw ::Deployment::InvalidConnection("",
                                            "Caught AlreadyConnected exception while connecting emitter");
    }
  catch (const ::Components::InvalidConnection& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect_consumer() returned ::Components::InvalidConnection exception\n"));
      throw ::Deployment::InvalidConnection("",
                                            "Caught InvalidConnection while connecting emitter.");
    }
}

void
NodeApplication_Impl::connect_emitter_ext (Components::CCMObject_ptr inst,
                                           const ACE_CString& port_name,
                                           CORBA::Object_ptr consumer)
{
  Components::EventConsumerBase_var event = Components::EventConsumerBase::_unchecked_narrow (consumer);
  try
    {
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - "
                   "connect_emitter_ext for %C started\n", port_name.c_str()));
      inst->connect_consumer (port_name.c_str(), event);
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - connect_emitter_ext finished\n"));
    }
  catch (const ::Components::AlreadyConnected& )
    {
      DANCE_DEBUG ( (LM_WARNING, "NodeApplication_impl::finishLaunch - "
                     "Components::CCMObject_var::connect_consumer() returned ::Components::AlreadyConnected exception\n"));
      throw ::Deployment::InvalidConnection("",
                                            "Caught AlreadyConnected exception while connecting external emitter.");
    }
  catch (const ::Components::InvalidConnection& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect_consumer() returned ::Components::InvalidConnection exception\n"));
      throw ::Deployment::InvalidConnection("",
                                            "Caught InvalidConnection exception while connecting external emitter.");
    }
}

Components::Cookie*
NodeApplication_Impl::connect_publisher (Components::CCMObject_ptr inst,
                                         const ACE_CString& port_name,
                                         CORBA::Object_ptr consumer)
{
  DANCE_TRACE ("NodeApplication_Impl::connect_publisher");

  if (CORBA::is_nil (inst))
    {
      DANCE_ERROR ((LM_ERROR, DLINFO "NodeApplication_Impl::connect_publisher - "
                    "Provided a nil CCMObject reference while connecting port %C\n",
                    port_name.c_str ()));
      throw ::Deployment::InvalidConnection ();
    }
  Components::Cookie* res = 0;
  Components::EventConsumerBase_var event = Components::EventConsumerBase::_unchecked_narrow (consumer);
  try
    {
      res = inst->subscribe (port_name.c_str(), event);
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - successfully subscribed %C\n",
                   port_name.c_str ()));
    }
  catch (const ::Components::InvalidName& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::subscribe() returned ::Components::InvalidName exception\n"));
      throw ::Deployment::StartError("", "Caught InvalidName exception while connecting publisher");
    }
  catch (const ::Components::InvalidConnection& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::subscribe() returned ::Components::InvalidConnection exception\n"));
      throw ::Deployment::InvalidConnection("", "Caught InvalidConnection exception while connecting publisher.");
    }
  catch (const ::Components::ExceededConnectionLimit& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::subscribe() returned ::Components::ExceededCOnnectionLimit exception\n"));
      throw ::Deployment::InvalidConnection("", "Caught ExceededConnectionLimit exception while connecting publisher.");
    }
  return res;
}

void NodeApplication_Impl::create_config_values(const Deployment::Properties& prop,
                                                Components::ConfigValues& cfg) const
{
  ACE_CString cdmw_name = "cdmw.config.";
  CORBA::ULong len = prop.length();
  unsigned int ind = 0;
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      ACE_CString s = prop[i].name.in();
      if (0 == s.find(cdmw_name))
        {
          cfg.length(ind+1);
          cfg[ind++] = new CIAO::ConfigValue_impl (s.substring(cdmw_name.length()).c_str(), prop[i].value);
        }
    }
}

