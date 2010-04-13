// $Id$
#include "NodeApplication_Impl.h"

#include "ace/OS_Memory.h"

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
#include "Deployment/Deployment_PlanErrorC.h"
#include "Deployment/Deployment_ApplicationManagerC.h"
#include "DAnCE/Deployment_common.h"
#include "DAnCE/DAnCE_PropertiesC.h"
#include "ComponentAttributesSetter.h"
#include "Name_Utilities.h"

#include <string>

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
    DANCE_TRACE ("NodeApplication::<anonymous>::get_property_value<T>");
    CORBA::Any any;

    DANCE_DEBUG (9, (LM_TRACE, DLINFO
                  ACE_TEXT("NodeApplication::<anonymous>::get_property_value<T> - ")
                  ACE_TEXT("Finding property value for name '%C'\n"),
                  name));

    if (properties.find (name, any) == 0)
      {
        if (any >>= val)
          {
            return true;
          }
        else
          {
            DANCE_ERROR (1, (LM_WARNING, DLINFO
                          ACE_TEXT("NodeApplication::<anonymous>::get_property_value<T> - ")
                          ACE_TEXT("Failed to extract property value for %C\n"), name));
            return false;
          }
      }

    DANCE_DEBUG (9, (LM_TRACE, DLINFO
                  ACE_TEXT("NodeApplication::<anonymous>::get_property_value<T> - ")
                  ACE_TEXT("Property value for name '%C' has no value\n"), name));

    return false;
  }

  template<>
  bool get_property_value (const char *name, PROPERTY_MAP &properties, bool &val)
  {
    DANCE_TRACE ("NodeApplication::<anonymous>::get_property_value<bool>");

    DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication::<anonymous>::get_property_value<bool> - ")
                  ACE_TEXT("Finding property value for name '%C'\n"),
                  name));

    CORBA::Any any;
    if (properties.find (name, any) == 0)
      {
        if (any >>= CORBA::Any::to_boolean(val))
          {
            return true;
          }
        else
          {
            DANCE_ERROR (1, (LM_WARNING, DLINFO ACE_TEXT("NodeApplication::<anonymous>::get_property_value<T> - ")
                          ACE_TEXT("Failed to extract property value for %C\n"), name));
            return false;
          }
      }

    DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication::<anonymous>::get_property_value<bool> - ")
                  ACE_TEXT("Property value for name '%C' has no value\n"), name));

    return false;
  }

  template<>
  bool get_property_value (const char *name, PROPERTY_MAP &properties, const char * &val)
  {
    DANCE_TRACE ("NodeApplication::<anonymous>::get_property_value<const char *>");
    CORBA::Any any;

    DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication::<anonymous>::get_property_value<bool> - ")
                  ACE_TEXT("Finding property value for name '%C'\n"),
                  name));

    if (properties.find (name, any) == 0)
      {
        if (any >>= CORBA::Any::to_string(val, 0))
          {
            return true;
          }
        else
          {
            DANCE_ERROR (1, (LM_WARNING, DLINFO ACE_TEXT("NodeApplication::<anonymous>::get_property_value<const char *> - ")
                          ACE_TEXT("Failed to extract property value for %C\n"), name));
            return false;
          }
      }

    DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication::<anonymous>::get_property_value<bool> - ")
                  ACE_TEXT("Property value for name '%C' has no value\n"), name));

    return false;
  }

  template<class T>
  bool get_property_value (const char *name, const ::Deployment::Properties &properties, T &val)
  {
    DANCE_TRACE ("NodeApplication::<anonymous>::get_property_value<T>");

    DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication::<anonymous>::get_property_value<T> - ")
                  ACE_TEXT("Finding property value for name '%C'\n"),
                  name));

    for (CORBA::ULong i = 0; i < properties.length (); ++i)
      {
        if (ACE_OS::strcmp (properties[i].name.in (), name) == 0)
          {
            DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication::<anonymous>::get_property_value<T> - ")
                          ACE_TEXT("Found property '%C'\n"), name));
            if (properties[i].value >>= val)
              return true;
            else
              {
                DANCE_ERROR (1, (LM_WARNING, DLINFO ACE_TEXT("NodeApplication::<anonymous>::get_property_value<T> - ")
                              ACE_TEXT("Failed to extract property value for %C\n"), name));
                return false;
              }
          }
      }


    DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication::<anonymous>::get_property_value<T> - ")
                  ACE_TEXT("Property value for name '%C' has no value\n"), name));

    return false;
  }

  template<>
  bool get_property_value (const char *name, const ::Deployment::Properties &properties, const char * &val)
  {
    DANCE_TRACE ("NodeApplication::<anonymous>::get_property_value<const char *>");

    DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication::<anonymous>::get_property_value<T> - ")
                  ACE_TEXT("Finding property value for name '%C'\n"),
                  name));

    for (CORBA::ULong i = 0; i < properties.length (); ++i)
      {
        if (ACE_OS::strcmp (properties[i].name.in (), name) == 0)
          {
            DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication::<anonymous>::get_property_value<T> - ")
                          ACE_TEXT("Found property '%C'\n"), name));
            if (properties[i].value >>= CORBA::Any::to_string (val, 0))
              {
                DANCE_DEBUG (9, (LM_TRACE,
                              DLINFO ACE_TEXT("NodeApplication::<anonymous>::get_property_value<T> - ")
                              ACE_TEXT("Value is %C\n"), val));
                return true;
              }
            else
              {
                DANCE_ERROR (1, (LM_WARNING, DLINFO ACE_TEXT("NodeApplication::<anonymous>::get_property_value<T> - ")
                              ACE_TEXT("Failed to extract property value for %C\n"), name));
                return false;
              }
          }
      }


    DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication::<anonymous>::get_property_value<T> - ")
                  ACE_TEXT("Unable to find property named %C\n"), name));

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

  void append_properties (::Deployment::Properties &dest,
        const ::Deployment::Properties &src)
  {
    const char* edu_prop = "edu.vanderbilt.dre.CIAO.ComponentServer.";
    for (CORBA::ULong i = 0; i < src.length (); ++i)
      {
        if (ACE_OS::strncmp (src[i].name.in (), edu_prop, ACE_OS::strlen (edu_prop)) == 0)
          {
            DANCE_DEBUG (9, (LM_TRACE, DLINFO
                      ACE_TEXT("NodeApplication::append_properties - ")
                      ACE_TEXT("Adding property %C\n"), src[i].name.in ()));
            CORBA::ULong const dest_length = dest.length ();
            dest.length (dest_length + 1);
            dest[dest_length].name = CORBA::string_dup (src[i].name.in ());
            dest[dest_length].value = src[i].value;
          }
      }
  }
}


NodeApplication_Impl::NodeApplication_Impl (CORBA::ORB_ptr orb,
                                            PortableServer::POA_ptr poa,
                                            const ::Deployment::DeploymentPlan& plan,
                                            const ACE_CString& node_name,
                                            const PROPERTY_MAP &properties)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    plan_ (plan),
    node_name_ (node_name),
    properties_ (),
    instances_ (plan.instance.length ())
{
  DANCE_TRACE ("NodeApplication_Impl::NodeApplication_Impl");
  PROPERTY_MAP::const_iterator i = properties.begin ();
  while (!i.done ())
    {
      DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::NodeApplication_Impl - ")
                    ACE_TEXT("Binding value for property '%C'\n"), i->key ().c_str ()));
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

  /* TODO: This is highly suspect.  I believe we should be using get_component_server,
     not calling create_container. */
  DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication_Impl::~NodeApplication_Impl - ")
                ACE_TEXT("Deactivating %u ComponentServers\n"),
                this->servers_.size ()));
  for (size_t i = 0; i < this->servers_.size (); ++i)
    {
      ComponentServer &server = this->servers_[i];

      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication_Impl::~NodeApplication_Impl - ")
                    ACE_TEXT("In ComponentServer %u, deactivating %u containers\n"), i, server.containers.size ()));
      for (size_t j = 0; j < server.containers.size (); ++j)
        {
          Container &container = server.containers[j];

          DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication_Impl::~NodeApplication_Impl - ")
                        ACE_TEXT("In container %u hosted in server %u\n"), j, i));

          try
            {
              if (!CORBA::is_nil (container.ref))
                server.ref->remove_container (container.ref.in ());

              container.ref = CIAO::Deployment::Container::_nil ();
            }
          catch (const CORBA::Exception &ex)
            {
              DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::~NodeApplication_Impl - ")
                            ACE_TEXT("Caught CORBA exception while removing container %u on server %u: %C\n"),
                            j, i, ex._info ().c_str ()));
            }
          catch (...)
            {
              DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::~NodeApplication_Impl - ")
                            ACE_TEXT("Caught unknown C++ exception while removing container %u on server %u.\n"),
                            j, i));
            }
        }

      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication_Impl::~NodeApplication_Impl - ")
                    ACE_TEXT("Removing component server %u\n"), i));

      try
        {
          if (!CORBA::is_nil (server.ref))
            this->activator_->remove_component_server (server.ref.in ());
        }
      catch (const CORBA::Exception &ex)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::~NodeApplication_Impl - ")
                        ACE_TEXT("Caught CORBA exception while removing server %u: %C\n"),
                        i, ex._info ().c_str ()));
        }
      catch (...)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::~NodeApplication_Impl - ")
                        ACE_TEXT("Caught unknown C++ exception while removing server %u.\n"),
                        i));
        }

      DANCE_DEBUG (8, (LM_INFO, DLINFO ACE_TEXT("NodeApplication_Impl::~NodeApplication_Impl - ")
                    ACE_TEXT("Successfully removed container %u on node %C.\n"),
                    i, this->node_name_.c_str ()));
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
  DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::init - ")
                ACE_TEXT("Component server path: %C\n"), cs_path));
  get_property_value (CIAO::Deployment::SERVER_ARGUMENTS, this->properties_, cs_args);
  DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::init - ")
                ACE_TEXT("Component server arguments: %C\n"), cs_args));
  get_property_value (CIAO::Deployment::SERVER_TIMEOUT, this->properties_, spawn);
  DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::init - ")
                ACE_TEXT("Spawn delay: %u\n"), spawn));
  get_property_value (CIAO::Deployment::SERVER_MULTITHREAD, this->properties_, multithread);
  DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::init - ")
                ACE_TEXT("Threading: %C\n"), multithread ? "Multi" : "Single"));

  DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication_Impl::init - ")
                ACE_TEXT("Spawning server activator\n")));

  CIAO::Deployment::ComponentInstallation_Impl *tmp_ci = 0;

  ACE_NEW_THROW_EX (tmp_ci,
                    CIAO::Deployment::ComponentInstallation_Impl (),
                    CORBA::NO_MEMORY ());

  PortableServer::ServantBase_var safe_servant = tmp_ci;

  PortableServer::ObjectId_var id = this->poa_->activate_object (tmp_ci);
  CORBA::Object_var ci_object = this->poa_->id_to_reference (id.in ());
  Components::Deployment::ComponentInstallation_var ci =
      Components::Deployment::ComponentInstallation::_narrow (ci_object.in ());

  for (CORBA::ULong i = 0; i < this->plan_.artifact.length (); ++i)
    {
      tmp_ci->install (this->plan_.artifact[i].name,
                       this->plan_.artifact[i].location[0]);
    }

  CIAO::Deployment::CIAO_ServerActivator_i *tmp_act = 0;
  ACE_NEW_THROW_EX (tmp_act,
                    CIAO::Deployment::CIAO_ServerActivator_i (spawn,
                                                              cs_path,
                                                              cs_args,
                                                              multithread,
                                                              ci.in (),
                                                              this->orb_.in(),
                                                              this->poa_.in()),
                    CORBA::NO_MEMORY ());
  this->activator_.reset (tmp_act);

  PortableServer::ObjectId_var sa_id =
    this->poa_->activate_object (this->activator_.get ());

  DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication_Impl::init - ServerActivator object created\n")));

  const ACE_TCHAR *ior = 0;

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
          DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::init - ")
                        ACE_TEXT("Unable to resolve the instance naming context:%C\n"),
                        e._info ().c_str ()));
        }
      DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::init - ")
                    ACE_TEXT("Successfully resolved the instance naming context.\n")));
    }
  else DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::init - ")
                     ACE_TEXT("No instance NC was provided\n")));
}

void
NodeApplication_Impl::configuration_complete_components ()
{
  DANCE_TRACE( "NodeApplication_Impl::configuration_complete_components");

  bool error = false;
  ::Deployment::StartError exception;

  for (INSTANCES::size_type k = 0; k < this->instances_.size (); ++k)
    {
      if (!this->instances_[k] ||
          this->instances_[k]->type == eHome ||
          this->instances_[k]->type == eComponentServer)
        {
          DANCE_DEBUG (9, (LM_TRACE, DLINFO
                           ACE_TEXT("NodeApplication_Impl::configuration_complete_components - ")
                           ACE_TEXT("Skipping non-component instance\n")));
          continue;
        }

      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication_Impl::configuration_complete_components - ")
                    ACE_TEXT("Invoking configuration_complete on component instance %C on node %C\n"),
                    this->plan_.instance[this->instances_[k]->idd_idx].name.in (),
                    this->node_name_.c_str ()));

      try
        {
          Components::CCMObject_var ccmobj =
            Components::CCMObject::_narrow (this->instances_[k]->ref.in ());
          if (CORBA::is_nil (this->instances_[k]->ref))
            {
              DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::configuration_complete_components - ")
                            ACE_TEXT("Failed to narrow object reference for component instance %C\n"),
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

          DANCE_DEBUG (8, (LM_INFO, DLINFO ACE_TEXT("NodeApplication_Impl::configuration_complete_components - ")
                        ACE_TEXT("Component %C successfully configured.\n"),
                        this->plan_.instance[this->instances_[k]->idd_idx].name.in ()));
        }
      catch (const CORBA::Exception &ex)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::configuration_complete_components - ")
                        ACE_TEXT("Caught CORBA exception from ccm_activate on component %C: %C\n"),
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
          DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::configuration_complete_components - ")
                        ACE_TEXT("Caught unknown C++ exception from ccm_activate on component %C\n"),
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
  ::Deployment::StartError exception;

  for (INSTANCES::size_type k = 0; k < this->instances_.size (); ++k)
    {
      if (this->instances_[k]->type == eHome ||
          this->instances_[k]->type == eComponentServer)
        {
          continue;
        }

      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication_Impl::start - ")
                    ACE_TEXT("Invoking start on component instance %C on node %C\n"),
                    this->plan_.instance[this->instances_[k]->idd_idx].name.in (),
                    this->node_name_.c_str ()));

      try
        {
          CIAO::Deployment::Container_var cont =
            CIAO::Deployment::Container::_narrow (this->instances_[k]->container->ref.in());

          if (CORBA::is_nil (this->instances_[k]->container->ref.in ()))
            {
              DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::start - ")
                            ACE_TEXT("Failed to narrow object reference for container managing ")
                            ACE_TEXT("component instance %C to a CIAO container reference\n"),
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

          DANCE_DEBUG (8, (LM_INFO, DLINFO ACE_TEXT("NodeApplication_Impl::start - ")
                        ACE_TEXT("Component %C successfully activated.\n"),
                        this->plan_.instance[this->instances_[k]->idd_idx].name.in ()));
        }
      catch (const CORBA::Exception &ex)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::start - ")
                        ACE_TEXT("Caught CORBA exception from ccm_activate on component %C: %C\n"),
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
          DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::start - ")
                        ACE_TEXT("Caught unknown C++ exception from ccm_activate on component %C\n"),
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

  const ::Deployment::MonolithicDeploymentDescription &mdd =
    this->plan_.implementation[inst.mdd_idx];
  const ::Deployment::InstanceDeploymentDescription &idd =
    this->plan_.instance[inst.idd_idx];

  DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::install_home - ")
                ACE_TEXT("Starting installation of home %C on node %C\n"),
                idd.name.in (), idd.node.in ()));

  this->instances_[inst.idd_idx] = &inst;

  // Need to get significant property values
  const char *entrypt = 0;
  get_property_value (DAnCE::HOME_FACTORY, mdd.execParameter, entrypt);

  if (entrypt == 0)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::install_home - ")
                    ACE_TEXT("Unable to find home factory property on home %C\n"),
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
      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication_Impl::install_home - ")
                    ACE_TEXT("Inserting value for execParameter %C\n"), mdd.execParameter[i].name.in ()));
      config[pos++] = new CIAO::ConfigValue_impl (mdd.execParameter[i].name.in (),
                                                  mdd.execParameter[i].value);
    }

  for (CORBA::ULong i = 0; i < idd.configProperty.length (); ++i)
    {
      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication_Impl::install_home - ")
                    ACE_TEXT("Inserting value for configProperty %C\n"), idd.configProperty[i].name.in ()));
      config[pos++] =  new CIAO::ConfigValue_impl (idd.configProperty[i].name.in (),
                                                   idd.configProperty[i].value);
    }

  try
    {
      DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::install_home - ")
                    ACE_TEXT("Calling install_home on container.  Home id '%C', entrypt '%C', ")
                    ACE_TEXT("length of config values is %u\n"),
                    idd.name.in (), entrypt, config.length ()));

      ::Components::CCMHome_var home = cont.ref->install_home (idd.name.in (),
                                                               entrypt,
                                                               config);

      if (CORBA::is_nil (home))
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::install_home - ")
                        ACE_TEXT("Got nil object reference from container while installing home %C on node %C,")
                        ACE_TEXT("throwing PlanError\n"),
                        idd.name.in (), idd.node.in ()));
          throw ::Deployment::PlanError (idd.name.in (),
                                         "Nil object reference returned from conainer");
        }

      DANCE_DEBUG (8, (LM_INFO, DLINFO  ACE_TEXT("NodeApplication_Impl::install_home - ")
                    ACE_TEXT("Home '%C' on node '%C' successfully installed\n"),
                    idd.name.in (), idd.node.in ()));

      inst.ref = CORBA::Object::_narrow (home);

      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication_Impl::install_home - ")
                    ACE_TEXT("Populating attributes for home %C\n"),
                    idd.name.in ()));


      ComponentAttributesSetter::SetComponentAttributes (idd.name.in (),
                                                         inst.ref.in (),
                                                         idd.configProperty,
                                                         this->orb_.in ());

      inst.state = eInstalled;
    }
  catch (const Components::InvalidConfiguration &)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::install_home - ")
                    ACE_TEXT("Error creating home %C on node %C, caught InvalidConfiguration.  Throwing exception\n"),
                    idd.name.in (), idd.node.in ()));
      throw ::Deployment::InvalidProperty (idd.name.in (),
                                     "Invalid configuration exception from container");
    }
  catch (const CORBA::Exception &ex)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::install_home - ")
                    ACE_TEXT("Caught CORBA exception while installing home %C: %C\n"),
                    idd.name.in (),
                    ex._info ().c_str ()));
      throw ::Deployment::StartError (idd.name.in (),
                                      ex._info ().c_str ());
    }
  catch (...)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::install_home - ")
                    ACE_TEXT("Caught unknown C++ exception while installing home %C\n"),
                    idd.name.in ()));
      throw ::Deployment::StartError (idd.name.in (),
                                      "Unknown C++ exception");
    }
}

void
NodeApplication_Impl::install_component (Container &cont, Instance &inst)
{
  DANCE_TRACE( "NodeApplication_Impl::install_component");

  ::Deployment::MonolithicDeploymentDescription const &mdd =
    this->plan_.implementation[inst.mdd_idx];
  ::Deployment::InstanceDeploymentDescription const &idd =
    this->plan_.instance[inst.idd_idx];

  DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::install_home - ")
                ACE_TEXT("Starting installation of home %C on node %C\n"),
                idd.name.in (), idd.node.in ()));

  this->instances_[inst.idd_idx] = &inst;

  const char *entrypt = 0;
  get_property_value (DAnCE::COMPONENT_FACTORY, mdd.execParameter, entrypt);

  if (entrypt == 0)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::install_component - ")
                    ACE_TEXT("Unable to find component factory property on component %C\n"),
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
      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication_Impl::install_component - ")
                    ACE_TEXT("Inserting value for execParameter %C\n"), mdd.execParameter[i].name.in ()));
      config[pos++] = new CIAO::ConfigValue_impl (mdd.execParameter[i].name.in (),
                                                  mdd.execParameter[i].value);
    }

  for (CORBA::ULong i = 0; i < idd.configProperty.length (); ++i)
    {
      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication_Impl::install_component - ")
                    ACE_TEXT("Inserting value for configProperty %C\n"), idd.configProperty[i].name.in ()));
      config[pos++] =  new CIAO::ConfigValue_impl (idd.configProperty[i].name.in (),
                                                   idd.configProperty[i].value);
    }

  ::CIAO::Deployment::Container_var ciao_cont = ::CIAO::Deployment::Container::_narrow (cont.ref.in ());

  if (CORBA::is_nil (ciao_cont.in ()))
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::install_component - ")
                    ACE_TEXT("Unable to narrow container assigned for component instance %C to one that supports ")
                    ACE_TEXT("un-homed components.\n")));
      throw ::Deployment::PlanError (idd.name.in (),
                                     "Hosting container does not support unhomed components.\n");
    }

  try
    {
      DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::install_component - ")
                    ACE_TEXT("Calling install_component on container.  Component id '%C', entrypt '%C', ")
                    ACE_TEXT("length of config values is %u\n"),
                    idd.name.in (), entrypt, config.length ()));

      ::Components::CCMObject_var comp = ciao_cont->install_component (idd.name.in (),
                                                                       entrypt,
                                                                       config);

      if (CORBA::is_nil (comp))
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::install_component - ")
                        ACE_TEXT("Got nil object reference from container while installing component %C on node %C,")
                        ACE_TEXT("throwing PlanError\n"),
                        idd.name.in (), idd.node.in ()));
          throw ::Deployment::PlanError (idd.name.in (),
                                         "Nil object reference returned from install_component on conainer");
        }

      DANCE_DEBUG (8, (LM_INFO, DLINFO ACE_TEXT("NodeApplication_Impl::install_component - ")
                    ACE_TEXT("Component '%C' on node '%C' successfully installed\n"),
                    idd.name.in (), idd.node.in ()));

      inst.ref = CORBA::Object::_narrow (comp);

      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication_Impl::install_component - ")
                    ACE_TEXT("Populating attributes for home %C\n"),
                    idd.name.in ()));


      ComponentAttributesSetter::SetComponentAttributes (idd.name.in (),
                                                         inst.ref.in (),
                                                         idd.configProperty,
                                                         this->orb_.in ());

      inst.state = eInstalled;
    }
    catch (const Components::InvalidConfiguration &)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::install_component - ")
                    ACE_TEXT("Error creating component %C on node %C, caught InvalidConfiguration. Throwing exception\n"),
                    idd.name.in (), idd.node.in ()));
      throw ::Deployment::InvalidProperty (idd.name.in (),
                                           "Invalid configuration exception from container");
    }
  catch (const CORBA::Exception &ex)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::install_component - ")
                    ACE_TEXT("Caught CORBA exception while installing component %C: %C\n"),
                    idd.name.in (),
                    ex._info ().c_str ()));
      throw ::Deployment::StartError (idd.name.in (),
                                      ex._info ().c_str ());
    }
  catch (...)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::install_component - ")
                    ACE_TEXT("Caught unknown C++ exception while installing component %C\n"),
                    idd.name.in ()));
      throw ::Deployment::StartError (idd.name.in (),
                                      "Unknown C++ exception");
    }
}

void
NodeApplication_Impl::install_homed_component (Container &cont, Instance &inst)
{
  DANCE_TRACE("NodeApplication_Impl::install_homed_component (unsigned int index)");

  ::Deployment::InstanceDeploymentDescription const &idd =
    this->plan_.instance[inst.idd_idx];
  this->instances_[inst.idd_idx] = &inst;

  DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication_Impl::install_homed_component - ")
                ACE_TEXT("Starting installation of homed component %C on node %C\n"),
                idd.name.in (),
                idd.node.in ()));

  const char *home_id = 0;
  get_property_value (DAnCE::EXPLICIT_HOME, idd.configProperty, home_id);

  DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::install_homed_component - ")
                ACE_TEXT("Property %C has value %C\n"),
                DAnCE::EXPLICIT_HOME, home_id));


  if (home_id == 0)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("Nodeapplication_Impl::install_homed_component - ")
                    ACE_TEXT("Apparent homed component %C lacks a %C configProperty, aborting installation\n"),
                    idd.name.in (), DAnCE::EXPLICIT_HOME));
      throw ::Deployment::PlanError (idd.name.in (),
                                     "No explicit home  property on component requiring explicit home.");
    }

  DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::install_homed_component - ")
                ACE_TEXT("Found explicit home property '%C' for component '%C'\n"),
                home_id,
                idd.name.in ()));

  Instance *home_inst (0);

  for (size_t i = 0; i < cont.homes.size (); ++i)
    {
      if (ACE_OS::strcmp (home_id,
                          this->plan_.instance[cont.homes[i].idd_idx].name.in ()) == 0)
        {
          DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication_Impl::install_homed_component - ")
                        ACE_TEXT("Found home designated for component '%C'\n"),
                        idd.name.in ()));
          home_inst = &cont.homes[i];
          break;
        }
    }

  if (home_inst == 0)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::install_homed_component - ")
                    ACE_TEXT("Unable to find home instance matching %C allocated to same container as component %C\n"),
                    home_id,
                    idd.name.in ()));
      throw ::Deployment::InvalidProperty (idd.name.in (),
                                           "Unable to find valid home allocated to same container.");
    }

  Components::KeylessCCMHome_var home =
    Components::KeylessCCMHome::_narrow (home_inst->ref.in ());

  if (CORBA::is_nil (home.in ()))
    {
      DANCE_ERROR (1, (LM_ERROR,  DLINFO ACE_TEXT("NodeApplication_Impl::install_homed_component - ")
                    ACE_TEXT("Object reference for home '%C' for homed component '%C' is nil\n"),
                    home_id,
                    idd.name.in ()));
      throw ::Deployment::PlanError (idd.name.in (),
                                     "Installed home for explicitly homed component has nil object reference\n");
    }

  try
    {
      inst.home = home_inst;
      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication_Impl::install_homed_component - ")
                    ACE_TEXT("Invoking create_component on home %C for component %C\n"),
                    home_id,
                    idd.name.in ()));

      Components::CCMObject_var ccm_obj = home->create_component ();

      if (CORBA::is_nil (ccm_obj))
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::install_homed_component - ")
      ACE_TEXT("Received nil component reference from create_component on home %C ")
      ACE_TEXT(" while creating component %C\n"),
                        home_id, idd.name.in ()));
          throw ::Deployment::StartError (idd.name.in (),
                                          "Home for explicitly homed component returned nil");
        }

      inst.ref = CORBA::Object::_narrow (ccm_obj.in ());
      DANCE_DEBUG (8, (LM_INFO, DLINFO  ACE_TEXT("NodeApplication_Impl::install_homed_component - ")
                    ACE_TEXT("Component %C successfully installed in home %C\n"),
                    idd.name.in (),
                    home_id));

      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication_Impl::install_home - ")
                    ACE_TEXT("Populating attributes for component %C\n"),
                    idd.name.in ()));
      ComponentAttributesSetter::SetComponentAttributes (idd.name.in (),
                                                        inst.ref.in (),
                                                        idd.configProperty,
                                                        this->orb_.in ());
      inst.state = eInstalled;
    }
  catch (const Components::CreateFailure &)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::install_homed_component - ")
                    ACE_TEXT("Caught CreateFailure exception from home '%C' while installing component '%C'\n"),
                    home_inst,
                    idd.name.in ()));
      throw ::Deployment::StartError (idd.name.in (),
                                      "Caught CreateFailure exception");
    }
  catch (const CORBA::Exception &ex)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::install_homed_component - ")
                    ACE_TEXT("Caught CORBA exception while installing component %C in home %C: %C\n"),
                    idd.name.in (),
                    home_id,
                    ex._info ().c_str ()));
      throw ::Deployment::StartError (idd.name.in (),
                                      ex._info ().c_str ());
    }
  catch (...)
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::install_homed_component - ")
                    ACE_TEXT("Caught unknown C++ exception while installing component %C in home  %C\n"),
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
      DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::create_component_Server - ")
                   ACE_TEXT("creating component server %u\n"), index));
      ::Components::ConfigValues config_values;

      if (this->servers_[index].properties != 0)
        {
          DANCE_DEBUG (9, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::create_component_Server - ")
                           ACE_TEXT ("Passing %u properties to component server\n"),
                           this->servers_[index].properties->length ()));

          config_values.length (this->servers_[index].properties->length ());
          for (CORBA::ULong i = 0; i < this->servers_[index].properties->length ();
               ++i)
            {
              DANCE_DEBUG (9, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::create_component_Server - ")
                               ACE_TEXT ("Copying value <%C>\n"),
                               (*this->servers_[index].properties)[i].name.in ()));

              config_values[i] = new CIAO::ConfigValue_impl ((*this->servers_[index].properties)[i].name.in (),
                                                             (*this->servers_[index].properties)[i].value);

            }
        }


      server.ref = this->activator_->create_component_server (config_values);
      DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::create_component_server - ")
                   ACE_TEXT("component server created\n")));
    }
  catch (const ::Components::CreateFailure& )
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::create_container - ")
                   ACE_TEXT("Components::Deployment::ServerActivator_var::create_component_server() ")
                   ACE_TEXT("returned ::Components::CreateFailure exception\n")));
      throw ::Deployment::StartError("",
                                     "Received a ::Components::CreateFailure exception while creating component server.");
    }
  catch (const ::Components::Deployment::InvalidConfiguration& )
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::create_container - ")
                   ACE_TEXT("Components::Deployment::ServerActivator_var::create_component_server() ")
                   ACE_TEXT("returned ::Components::Deployment::InvalidConfiguration exception\n")));
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
        DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::create_container - ")
                      ACE_TEXT("Caught exception whilst creating container; re-throwing.\n")));
        throw;
      }
}

void
NodeApplication_Impl::store_instance_ior (Instance &inst)
{
  DANCE_TRACE ("NodeApplication_Impl::store_instance_ior");

  const char *name = 0;

  if (get_property_value (DAnCE::REGISTER_NAMING,
                          this->plan_.instance[inst.idd_idx].configProperty,
                          name))
    {
      DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::store_instance_ior - ")
                    ACE_TEXT("Storing instance '%C' object reference in Naming Service as %C\n"),
                    this->plan_.instance[inst.idd_idx].name.in (),
                    name));

      Name_Utilities::bind_object (name, inst.ref.in (), this->instance_nc_.in ());
    }

  if (get_property_value (DAnCE::INSTANCE_IOR_FILE,
                          this->plan_.instance[inst.idd_idx].configProperty,
                          name))
    {
      CORBA::String_var ior = this->orb_->object_to_string (inst.ref.in ());
      Name_Utilities::write_ior (ACE_TEXT_CHAR_TO_TCHAR (name),
                                 (ior.in ()));
    }
}

void
NodeApplication_Impl::create_container (size_t server, size_t cont_idx)
{
  DANCE_TRACE ("NodeApplication_Impl::create_container");

  Container &container = this->servers_[server].containers[cont_idx];

  DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication_Impl::create_container - ")
                ACE_TEXT("Creating container\n")));
  // TODO: Need to create configvalues
  Components::ConfigValues cvs;

  container.ref = this->servers_[server].ref->create_container (cvs);

  DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication_Impl::create_container - ")
                ACE_TEXT("Configuring %u homes on container %u on server %u\n"),
                container.homes.size (),
                server, cont_idx));

  // Configure homes first
  for (size_t i = 0; i < container.homes.size (); ++i)
    {
      this->install_home (container, container.homes[i]);
      this->store_instance_ior (container.homes[i]);
    }

  DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication_Impl::create_container - ")
                   ACE_TEXT("Configuring %u components on container %u on server %u\n"),
                   container.components.size (),
                   server,
                   cont_idx));

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

NodeApplication_Impl::ColocationMap
NodeApplication_Impl::create_colocation_groups (void)
{
  DANCE_TRACE ("NodeApplication_Impl::create_colocation_groups");

  ColocationMap  retval;
  this->servers_.max_size (this->plan_.localityConstraint.length () + 1);

  size_t num_servers = 0;

  for (CORBA::ULong i = 0; i < this->plan_.instance.length (); ++i)
    {
      retval [this->plan_.instance[i].name.in ()] = -1;
    }

  for (CORBA::ULong i = 0; i < this->plan_.localityConstraint.length (); ++i)
    {
      if (this->plan_.localityConstraint[i].constraint == ::Deployment::PlanNoConstraint)
        {
          DANCE_DEBUG (10, (LM_INFO, DLINFO
                            ACE_TEXT ("NodeApplication_Impl::create_colocation_groups - ")
                            ACE_TEXT ("Skipping NoConstraint Colocation group\n")));
          continue;
        }
      else if (this->plan_.localityConstraint[i].constraint != ::Deployment::PlanSameProcess)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                        ACE_TEXT ("NodeApplication_Impl::create_colocation_groups - ")
                        ACE_TEXT ("Error: On locality constraint %u, unsupported locality constraint.\n"),
                        i));
          continue;
        }

      ::CORBA::ULongSeq const &instances =
        this->plan_.localityConstraint[i].constrainedInstanceRef;

      this->servers_.size (num_servers + 1);

      for (CORBA::ULong j = 0; j < instances.length (); ++j)
        {
          std::string id = this->plan_.instance[instances[j]].name.in ();

          DANCE_DEBUG (8, (LM_INFO, DLINFO
                        ACE_TEXT ("NodeApplication_Impl::create_colocation_groups - ")
                        ACE_TEXT ("Instance <%C> allocated to component server %u\n"),
                        id.c_str (), num_servers));

          retval[id] = num_servers;

          CORBA::ULong impl = this->plan_.instance[instances[j]].implementationRef;

          if (this->get_instance_type (this->plan_.implementation[impl].execParameter) ==
              eComponentServer)
            {
              DANCE_DEBUG (8, (LM_INFO, DLINFO
                               ACE_TEXT ("NodeApplication_Impl::create_colocation_groups - ")
                               ACE_TEXT ("Found component server instance\n")));

              this->servers_[num_servers].properties = &this->plan_.instance[instances[j]].configProperty;
            }
        }

      ++num_servers;
    }

  bool create_default_server (false);

  for (ColocationMap::iterator i = retval.begin ();
       i != retval.end (); ++i)
    {
      if (i->second == -1)
        {
          if (!create_default_server)
            {
              DANCE_DEBUG (8, (LM_INFO, DLINFO
                            ACE_TEXT ("NodeApplication_Impl::create_colocation_groups - ")
                            ACE_TEXT ("Creating default colocation group.\n")));
              create_default_server = true;
            }

          DANCE_DEBUG (8, (LM_INFO, DLINFO
                        ACE_TEXT ("NodeApplication_Impl::create_colocation_groups - ")
                        ACE_TEXT ("Assigning instance <%C> to default colocation group.\n"),
                        i->first.c_str ()));
          i->second = num_servers;
        }
      else
        {

        }
    }

  if (create_default_server)
    {
      ++num_servers;
    }

  this->servers_.size (num_servers);

  return retval;
}

void
NodeApplication_Impl::init_components()
{
  DANCE_TRACE ("NodeApplication_Impl::init_components");

  Components::ConfigValues config_values;
  DANCE_DEBUG (6, (LM_DEBUG, DLINFO
               ACE_TEXT("NodeApplication_Impl::init_components - ")
               ACE_TEXT("Configuring %u component/home instances\n"),
               this->plan_.instance.length()));

  if (this->plan_.instance.length () == 0)
    return;

  ColocationMap colocation_map = this->create_colocation_groups ();

  // @@TODO:  For the moment, we are only going to support a single container per server.
  // in the future, we will need to determine how many containers we need per server.
  for (size_t i = 0; i < this->servers_.size (); ++i)
    {
      this->servers_[i].containers.size (1);
    }

  for (CORBA::ULong i = 0; i < this->plan_.instance.length(); i++)
    {
      try
        {
          CORBA::ULong impl = this->plan_.instance[i].implementationRef;
          // Determine type from implementation properties, then from instance properties.
          EInstanceType type =
            this->get_instance_type (this->plan_.implementation[impl].execParameter);
          if (type == eInvalid)
            type = this->get_instance_type (this->plan_.instance[i].configProperty);
          if (type == eInvalid)
            continue;

          switch (type)
            {
            case eHome:
              {
                DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                              ACE_TEXT("NodeApplication_Impl::init_components - ")
                              ACE_TEXT("Allocating instance %C as a home\n"),
                              this->plan_.instance[i].name.in ()));
                size_t const svr = colocation_map[this->plan_.instance[i].name.in ()];
                size_t const pos = this->servers_[svr].containers[0].homes.size ();

                //append_properties (this->servers_[svr].properties, this->plan_.instance[i].configProperty);

                this->servers_[svr].containers[0].homes.size (pos + 1);
                this->servers_[svr].containers[0].homes[pos] = Instance (eHome,
                                                                         &this->servers_[svr].containers[0],
                                                                         i,
                                                                         this->plan_.instance[i].implementationRef);
                break;
              }
            case eComponent:
              {
                DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::init_components - ")
                              ACE_TEXT("Allocating instance %C as a standalone component\n"),
                              this->plan_.instance[i].name.in ()));
                size_t const svr = colocation_map[this->plan_.instance[i].name.in ()];
                size_t const pos = this->servers_[svr].containers[0].components.size ();
                //append_properties (this->servers_[svr].properties, this->plan_.instance[i].configProperty);
                this->servers_[svr].containers[0].components.size (pos + 1);
                this->servers_[svr].containers[0].components[pos] = Instance (eComponent,
                                                                              &this->servers_[svr].containers[0],
                                                                              i,
                                                                              this->plan_.instance[i].implementationRef);
                break;
              }
            case eHomedComponent:
              {
                DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::init_components - ")
                              ACE_TEXT("Allocating instance %C as a home managed component\n"),
                              this->plan_.instance[i].name.in ()));
                size_t const svr = colocation_map[this->plan_.instance[i].name.in ()];
                size_t const pos = this->servers_[svr].containers[0].components.size ();
                //append_properties (this->servers_[svr].properties, this->plan_.instance[i].configProperty);
                this->servers_[svr].containers[0].components.size (pos + 1);
                this->servers_[svr].containers[0].components[pos] = Instance (eHomedComponent,
                                                                              &this->servers_[svr].containers[0],
                                                                              i,
                                                                              this->plan_.instance[i].implementationRef);
                break;
              }
            case eComponentServer:
              {
                size_t const svr = colocation_map[this->plan_.instance[i].name.in ()];
                this->servers_[svr].instance = Instance (eComponentServer,
                                                         0,
                                                         i,
                                                         this->plan_.instance[i].implementationRef);
                this->instances_[i] = &this->servers_[svr].instance;
                break;
              }

            default:
              {
                DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT(" NodeApplication_Impl::init_components - ")
                             ACE_TEXT("get_instance_type function returned invalid instance type\n")));
                throw ::Deployment::InvalidProperty (this->plan_.instance[i].name.in (),
                                                     "Unable to affirmatively determine instance type");
              }
            } // switch
        }
        catch (...)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                      ACE_TEXT("Exception was thrown while sorting instance \"%C\".\n"),
                      this->plan_.instance[i].name.in()));
          throw;
        }
    }

  DANCE_DEBUG (9, (LM_TRACE, DLINFO
                ACE_TEXT("NodeApplication_Impl::init_components - ")
                ACE_TEXT("Creating component servers and installing components.\n")));

  for (size_t i = 0; i < this->servers_.size (); ++i)
    {
      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication_Impl::init_components - ")
                    ACE_TEXT("Creating component server with index %u\n"), i));
      this->create_component_server (i);
    }


  DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::init_components - finished\n")));
}

void
NodeApplication_Impl::passivate_components()
{
  DANCE_TRACE ("NodeApplication_Impl::passivate_components()");

  bool error (false);
  ::Deployment::StopError exception ("unfilled", "unfilled passivate components");

  for (INSTANCES::size_type k = 0; k < this->instances_.size (); ++k)
    {
      if (this->instances_[k]->type == eHome ||
          this->instances_[k]->type == eComponentServer ||
          this->instances_[k]->type == eInvalid)
        continue;

      try
        {
          CIAO::Deployment::Container_var cont =
            CIAO::Deployment::Container::_narrow (this->instances_[k]->container->ref.in());

          if (CORBA::is_nil (this->instances_[k]->container->ref.in ()))
            {
              DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::start - ")
                            ACE_TEXT("Failed to narrow object reference for container managing ")
                            ACE_TEXT("component instance %C to a CIAO container reference\n"),
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
              DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::start - ")
                            ACE_TEXT("Attempting to passivate non-active component %C\n"),
                            this->plan_.instance[this->instances_[k]->idd_idx].name.in ()));
              test_and_set_exception (error, exception,
                                      this->plan_.instance[this->instances_[k]->idd_idx].name.in (),
                                      "Attempting to passivate non-active component.");
              continue;
            }

          DANCE_DEBUG (8, (LM_INFO, DLINFO ACE_TEXT("NodeApplication_Impl::passivate_components - ")
                        ACE_TEXT("Component %C successfully passivated.\n"),
                        this->plan_.instance[this->instances_[k]->idd_idx].name.in ()));
        }
      catch (const CORBA::Exception &ex)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::passivate_components - ")
                        ACE_TEXT("Caught CORBA exception from ccm_passivate on component %C: %C\n"),
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
          DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::passivate_components - ")
                        ACE_TEXT("Caught unknown C++ exception from ccm_passivate on component %C\n"),
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
  ::Deployment::StopError exception ("unfilled", "unfilled remove_components");

  // Removing components first.
  for (INSTANCES::size_type k = 0; k < this->instances_.size (); ++k)
    {
      try
        {
          if (this->instances_[k]->type == eInvalid ||
              this->instances_[k]->type == eComponentServer ||
              this->instances_[k]->type == eHome)
            continue;

          if (this->instances_[k]->state != ePassive)
            DANCE_DEBUG (6, (LM_WARNING, DLINFO ACE_TEXT("NodeApplication_Impl::remove_components - ")
                          ACE_TEXT("Attempting to remove component that is not passive.\n")));

          if (this->instances_[k]->type == eComponent)
            {
              if (this->instances_[k]->container == 0)
                {
                  DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::remove_components - ")
                                ACE_TEXT("Container element in component data structure for '%C' is nill\n"),
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
                  DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::remove_components - ")
                                ACE_TEXT("Attempted to remove un-homed component from unsupported container\n")));

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
                  DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::remove_components - ")
                                ACE_TEXT("Object reference for home %C, managing component %C is nil.\n"),
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

          DANCE_DEBUG (8, (LM_INFO, DLINFO ACE_TEXT("NodeApplication_Impl::remove_components - ")
                        ACE_TEXT("Component %C successfully removed.\n"),
                        this->plan_.instance[this->instances_[k]->idd_idx].name.in ()));

        }
      catch (const CORBA::Exception &ex)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::remove_components - ")
                        ACE_TEXT("Caught CORBA exception removing on component %C: %C\n"),
                        this->plan_.instance[this->instances_[k]->idd_idx].name.in (),
                        ex._info ().c_str ()));
          test_and_set_exception (error,
                                  exception,
                                  this->plan_.instance[this->instances_[k]->idd_idx].name.in (),
                                  exception.reason = ex._info ().c_str ());
        }
      catch (...)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::remove_components - ")
                        ACE_TEXT("Caught unknown C++ exception from ccm_remove on component %C\n"),
                        this->plan_.instance[this->instances_[k]->idd_idx].name.in ()));
          test_and_set_exception (error,
                                  exception,
                                  this->plan_.instance[this->instances_[k]->idd_idx].name.in (),
                                  "Unknown C++ exception");
        }
    }

  for (INSTANCES::size_type k = 0; k < this->instances_.size (); ++k)
    {
      try
        {
          if (this->instances_[k]->type != eHome)
            continue;

          DANCE_ERROR (1, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication_Impl::remove_components - ")
                        ACE_TEXT("Removing home %C\n"),
                        this->plan_.instance[this->instances_[k]->idd_idx].name.in ()));

          if (this->instances_[k]->container == 0 ||
              CORBA::is_nil (this->instances_[k]->container->ref.in ()))
            {
              DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::remove_components - ")
                            ACE_TEXT("Home %C lacks an associated container reference\n"),
                            this->plan_.instance[this->instances_[k]->idd_idx].name.in ()));
              test_and_set_exception (error,
                                      exception,
                                      this->plan_.instance[this->instances_[k]->idd_idx].name.in (),
                                      "Didn't have a valid container reference");
              continue;
            }

          Components::CCMHome_var home = Components::CCMHome::_narrow (this->instances_[k]->ref.in ());
          this->instances_[k]->container->ref->remove_home (home.in ());

          DANCE_DEBUG (8, (LM_INFO, DLINFO ACE_TEXT("NodeApplication_Impl::remove_components - ")
                       ACE_TEXT("Successfully uninstalled home %C\n"),
                       this->plan_.instance[this->instances_[k]->idd_idx].name.in ()));
        }
      catch (const CORBA::Exception &ex)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::remove_components - ")
                        ACE_TEXT("Caught CORBA exception removing home %C: %C\n"),
                        this->plan_.instance[this->instances_[k]->idd_idx].name.in (),
                        ex._info ().c_str ()));
          test_and_set_exception (error,
                                  exception,
                                  this->plan_.instance[this->instances_[k]->idd_idx].name.in (),
                                  exception.reason = ex._info ().c_str ());
        }
      catch (...)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::remove_components - ")
                        ACE_TEXT("Caught unknown C++ exception from while removing home %C\n"),
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
NodeApplication_Impl::get_instance_type (const ::Deployment::Properties& prop) const
{
  DANCE_TRACE ("NodeApplication_Impl::get_instance_type");

  for (CORBA::ULong i = 0; i < prop.length (); ++i)
    {
      DANCE_DEBUG (9, (LM_TRACE, DLINFO  ACE_TEXT("NodeApplication_Impl::get_instance_type - ")
                    ACE_TEXT("Checking property %C\n"),
                    prop[i].name.in ()));

      if (ACE_OS::strcmp (prop[i].name.in (),
                          DAnCE::HOME_FACTORY) == 0)
        {
          DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication_Impl::get_instance_type - ")
                        ACE_TEXT("Found Home type\n")));
          return eHome;
        }
      if (ACE_OS::strcmp (prop[i].name.in (),
                          DAnCE::COMPONENT_FACTORY) == 0)
        {
          DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication_Impl::get_instance_type - ")
                         ACE_TEXT("Found unmanaged component type.\n")));
          return eComponent;
        }
      if (ACE_OS::strcmp (prop[i].name.in (),
                          DAnCE::EXPLICIT_HOME) == 0)
        {
          DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication_Impl::get_instance_type - ")
                        ACE_TEXT("Found explicit home component type.\n")));
          return eHomedComponent;
        }

      if (ACE_OS::strcmp (prop[i].name.in (),
                          DAnCE::IMPL_TYPE) == 0)
        {
          const char *val;
          if (get_property_value (DAnCE::IMPL_TYPE,
                                  prop,
                                  val) &&
              ACE_OS::strcmp (val,
                              DAnCE::SERVER_EXECUTABLE) == 0)
            {
              DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication_Impl::get_instance_type - ")
                               ACE_TEXT("Found component server type.\n")));
              return eComponentServer;
            }
        }
    }

  DANCE_ERROR (1, (LM_INFO, DLINFO ACE_TEXT("NodeApplication_Impl::get_instance_type - ")
                ACE_TEXT("Unable to determine instance type, instance will be ignored.\n")));
  return eInvalid;
}

::Deployment::Connections*
NodeApplication_Impl::getAllConnections()
{
  DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::getAllConnections - started\n")));

  ::Deployment::Connections_var conn;
  ACE_NEW_THROW_EX (conn,
                    ::Deployment::Connections (),
                    CORBA::NO_MEMORY ());
  unsigned int index = 0;

  for (CORBA::ULong i = 0; i < this->plan_.connection.length(); i++)
    {
      for (CORBA::ULong j = 0; j < this->plan_.connection[i].internalEndpoint.length(); j++)
        {
          if (this->plan_.connection[i].internalEndpoint[j].provider)
            {
              index = conn->length ();
              conn->length (index + 1);
              (*conn) [index].name = CORBA::string_dup (this->plan_.connection[i].name.in());

              ACE_CString const inst_name =
                this->plan_.instance[this->plan_.connection[i].internalEndpoint[j].instanceRef].name.in();

              DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::getAllConnections - ")
                            ACE_TEXT("Found provider '%C' for connection '%C'\n"),
                            this->plan_.connection[i].name.in (),
                            inst_name.c_str ()));

              Components::CCMObject_var obj =
                Components::CCMObject::
                _narrow (this->instances_[this->plan_.connection[i].internalEndpoint[j].instanceRef]->ref.in ());

              (*conn) [index].endpoint.length (1L);
              switch (this->plan_.connection[i].internalEndpoint[j].kind)
                {
                case ::Deployment::Facet:
                  {
                    try
                      {
                        ACE_CString const name = this->plan_.connection[i].internalEndpoint[j].portName.in();
                        if (name.compare ("supports") != 0)
                          {
                            if (this->is_local_facet (this->plan_.connection[i]))
                              {
                                DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::getAllConnections - ")
                                             ACE_TEXT("Found local facet for connection %C endpoint %C started\n"),
                                             this->plan_.connection[i].name.in(),
                                             name.c_str ()));
                                (*conn) [index].endpoint[0] = CORBA::Object::_duplicate (obj.in());
                              }
                            else
                              {
                                DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::getAllConnections - ")
                                             ACE_TEXT("provide_facet for connection %C endpoint %C started\n"),
                                             this->plan_.connection[i].name.in(),
                                             name.c_str ()));
                                (*conn) [index].endpoint[0] = obj->provide_facet (name.c_str());
                                DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::getAllConnections - ")
                                             ACE_TEXT("provide_facet for connection %C endpoint %C finished\n"),
                                             this->plan_.connection[i].name.in(),
                                             name.c_str ()));
                              }
                          }
                        else
                          {
                            DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::getAllConnections - ")
                                         ACE_TEXT("provide_facet for connection %C endpoint %C started\n"),
                                         this->plan_.connection[i].name.in(),
                                         name.c_str ()));
                            (*conn) [index].endpoint[0] = CORBA::Object::_duplicate (obj.in());
                          }
                        DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::getAllConnections - provide_facet finished\n")));
                      }
                    catch (const ::Components::InvalidName& )
                      {
                        DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::getAllConnections - ")
                                     ACE_TEXT("Components::CCMObject_var::provide_facet() returned ")
                                     ACE_TEXT("::Components::InvalidName exception for connection %C and port %C\n"),
                                     this->plan_.connection[i].name.in (),
                                     this->plan_.connection[i].internalEndpoint[j].portName.in ()));
                        throw ::Deployment::InvalidProperty(this->plan_.connection[i].name.in (),
                                                            "Container returned InvalidName");
                      }
                    break;
                  }

                case ::Deployment::EventConsumer:
                  {
                    try
                      {
                        DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::getAllConnections - ")
                                     ACE_TEXT("get_consumer for connection %C endpoint %C started\n"),
                                     this->plan_.connection[i].name.in(),
                                     this->plan_.connection[i].internalEndpoint[j].portName.in()));
                        (*conn) [index].endpoint[0] = obj->get_consumer (this->plan_.connection[i].internalEndpoint[j].portName.in());
                        DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::getAllConnections - ")
                                     ACE_TEXT("get_consumer finished\n")));
                      }
                    catch (const ::Components::InvalidName& )
                      {
                        DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT(" NodeApplication_Impl::getAllConnections - ")
                                     ACE_TEXT("Components::CCMObject_var::get_consumer() returned ")
                                     ACE_TEXT("::Components::InvalidName exception for connection %C and port %C\n"),
                                     this->plan_.connection[i].name.in (),
                                     this->plan_.connection[i].internalEndpoint[j].portName.in ()));
                        throw ::Deployment::InvalidProperty(this->plan_.connection[i].name.in (),
                                                            "Container returned InvalidName exception");
                      }
                    break;
                  }
                default:
                  {
                    DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT(" NodeApplication_Impl::getAllConnections - ")
                                 ACE_TEXT("Connection.InternalEndPoint.Kind is not a ")
                                 ACE_TEXT("Deployment::Facet or Deployment::EventConsumer\n")));
                    throw ::Deployment::InvalidProperty(this->plan_.connection[i].name.in (),
                                                        "Invalid connection type, should be Facet or EventConsumer");
                  }
                }
            }
        }
    }
  DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::getAllConnections - finished\n")));
  return conn._retn();
}

void
NodeApplication_Impl::finishLaunch (const ::Deployment::Connections & providedReference,
                                    ::CORBA::Boolean start)
{
  DANCE_TRACE ("NodeApplication_Impl::finishLaunch");

  DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::finishLaunch - ")
               ACE_TEXT("started for connections sequence with length %d and plan size %d\n"),
               providedReference.length(),
               this->plan_.connection.length()));

#ifdef GEN_OSTREAM_OPS
  {
    std::ostringstream pr_stream;
    pr_stream << providedReference << std::endl;
    DANCE_DEBUG (9, (LM_TRACE, DLINFO "NodeApplication_Impl::finishLaunch - Provided references: %C\n",
                  pr_stream.str ().c_str ()));
  }
#endif /* GEN_OSTREAM_OPS */

  for (CORBA::ULong j = 0; j < this->plan_.connection.length(); ++j)
    {
      CORBA::ULong inst (this->plan_.connection[j].internalEndpoint[0].instanceRef);

      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("NodeApplication_Impl::finishLaunch - ")
                    ACE_TEXT("Connection %C, instance %u\n"),
                    this->plan_.connection[j].name.in (),
                    inst));

#ifdef GEN_OSTREAM_OPS
      {
        std::ostringstream conn_stream;
        conn_stream << this->plan_.connection[j] << std::endl;
        DANCE_DEBUG (9, (LM_TRACE, "NodeApplication_Impl::finishLaunch - Local connections: %C\n", conn_stream.str ().c_str ()));
      }
#endif /* GEN_OSTREAM_OPS  */

      Components::CCMObject_var obj =
        Components::CCMObject::_narrow (this->instances_[inst]->ref.in ());

      if (CORBA::is_nil (obj.in ()))
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::finishLaunch - ")
                        ACE_TEXT("Unable to narrow apparent component instance reference to CCMObject for instance '%C'\n"),
                        this->plan_.instance[inst].name.in ()));
          throw ::Deployment::InvalidConnection (this->plan_.instance[inst].name.in (),
                                                 "Unable to narrow apparent component instance reference to CCMObject\n");
        }

      ::Deployment::PlanConnectionDescription const &conn = this->plan_.connection[j];

      ACE_CString const name = conn.name.in();

      for (CORBA::ULong i = 0; i < providedReference.length(); ++i)
        {
          if (name.compare (providedReference[i].name.in()) == 0)
            {
              try
                {
                  switch (conn.internalEndpoint[0].kind)
                    {
                    case ::Deployment::Facet:
                      {
                        DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::finishLaunch - ")
                                     ACE_TEXT("Set for facet [%C]\n"), name.c_str ()));
                        Components::CCMObject_var ext_inst;
                        try
                          {
                            if (0 == conn.externalReference.length())
                              {
                                if (conn.internalEndpoint.length () == 2 &&
                                    (conn.internalEndpoint[1].kind == ::Deployment::MultiplexReceptacle ||
                                     conn.internalEndpoint[1].kind == ::Deployment::SimplexReceptacle))
                                  {
                                    obj = Components::CCMObject::
                                      _narrow (this->instances_[conn.internalEndpoint[1].instanceRef]->ref.in ());

                                    if (this->is_local_facet (conn))
                                      {
                                        ::Components::CCMObject_var facet =
                                          ::Components::CCMObject::_narrow (providedReference[i].endpoint[0].in ());

                                        ::Components::CCMObject_var recep =
                                            ::Components::CCMObject::_narrow (this->instances_[conn.internalEndpoint[1].instanceRef]->ref.in ());

                                        ::CIAO::Deployment::Container_var cont =
                                            ::CIAO::Deployment::Container::_narrow (this->instances_[conn.internalEndpoint[1].instanceRef]->container->ref.in ());

                                        this->connect_receptacle (conn,
                                                                  facet.in (),
                                                                  conn.internalEndpoint[0].portName.in (),
                                                                  obj.in (),
                                                                  conn.internalEndpoint[1].portName.in(),
                                                                  cont.in ());
                                      }
                                    else
                                      {
                                        this->connect_receptacle (conn,
                                                                  obj.in (),
                                                                  "",
                                                                  providedReference[i].endpoint[0].in(),
                                                                  conn.internalEndpoint[1].portName.in(),
                                                                  ::CIAO::Deployment::Container::_nil());
                                      }
                                  }
                                 break;
                              }
                            CORBA::Object_var tmp =
                              this->orb_->string_to_object (conn.externalReference[0].location.in());
                            ext_inst = Components::CCMObject::_narrow (tmp);
                            if (CORBA::is_nil (ext_inst.in()))
                              {
                                DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::finishLaunch - ")
                                             ACE_TEXT("facet for [%C] can't be narrowed\n"), name.c_str ()));
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

                    case ::Deployment::EventConsumer:
                      {
                        DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::finishLaunch - set for consumer\n")));
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
                                DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT(" NodeApplication_Impl::finishLaunch - ")
                                             ACE_TEXT("reference for %C can't be narrowed\n"), name.c_str ()));
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
                                DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                                                ACE_TEXT("NodeApplication_Impl::finishLaunch - ")
                                                ACE_TEXT("connect_emitter_ext resulted in InvalidName, trying connect_publisher\n")));
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
                    case ::Deployment::MultiplexReceptacle:
                    case ::Deployment::SimplexReceptacle:
                      {
                        // What we should do with Cookie, returned from connect call???
                        DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::finishLaunch - Set for receptacle\n")));
                        if (CORBA::is_nil (providedReference[i].endpoint[0].in ()))
                          {
                            DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT ("NodeApplication_Impl::finishLaunch - ")
                                             ACE_TEXT ("Reference provided from DomainApplication was nil.\n")));
                            throw Deployment::InvalidConnection ("",
                                                                 "Nil provided reference from DomainApplication");
                          }

                        ::Components::CCMObject_var facet =
                           ::Components::CCMObject::_narrow (providedReference[i].endpoint[0].in ());

                        if (this->is_local_facet (conn))
                          {
                            ::CIAO::Deployment::Container_var cont =
                               ::CIAO::Deployment::Container::_narrow (this->instances_[conn.internalEndpoint[1].instanceRef]->container->ref.in ());
                            this->connect_receptacle (conn,
                                                      facet.in (),
                                                      conn.internalEndpoint[1].portName.in (),
                                                      obj.in(),
                                                      conn.internalEndpoint[0].portName.in(),
                                                      cont.in ());
                          }
                        else
                          {
                            CORBA::Object_var portref = CORBA::Object::_duplicate (providedReference[i].endpoint[0].in());

                            if (0 != conn.externalReference.length ())
                              {
                                ::Components::CCMObject_var facet =
                                  ::Components::CCMObject::_narrow (providedReference[i].endpoint[0].in ());

                                if (!CORBA::is_nil (facet.in ()))
                                  portref = facet->provide_facet (conn.externalReference[0].portName.in ());
                              }

                            this->connect_receptacle (conn,
                                                      obj.in (),
                                                      "",
                                                      portref.in (),
                                                      conn.internalEndpoint[0].portName.in(),
                                                      ::CIAO::Deployment::Container::_nil());

                          }

                        break;
                      }
                    case ::Deployment::EventEmitter:
                      {
                        DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::finishLaunch - set for emitter\n")));
                        this->connect_emitter (obj.in(),
                                               conn.internalEndpoint[0].portName.in(),
                                               providedReference[i].endpoint[0].in());
                        break;
                      }
                    case ::Deployment::EventPublisher:
                      {
                        DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::finishLaunch - set for publisher\n")));
                        this->connect_publisher (obj.in(),
                                                 conn.internalEndpoint[0].portName.in(),
                                                 providedReference[i].endpoint[0].in());
                        break;
                      }
                    default:
                      {
                        DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::finishLaunch - currect Connection.InternalEndPoint.Kind ")
                                     ACE_TEXT("is not a Deployment::SimplexReceptacle, Deployment::EventEmitter, Deployment::EventPublisher ")
                                     ACE_TEXT("(Connection:%C Kind:%i PortName:%C)\n"),
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
                  DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::finishLaunch - ")
                                ACE_TEXT("Intercepted StartError exception while configuring [%C] connection, rethrowing\n"),
                                name.c_str ()));
                  ex.name = name.c_str ();
                  throw;
                }
              catch (::Deployment::InvalidConnection &ex)
                {
                  DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::finishLaunch - ")
                                ACE_TEXT("Intercepted InvalidConnection exception while configuring [%C] connection, rethrowing\n"),
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

  DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::finishLaunch - finished\n")));
}

bool
NodeApplication_Impl::is_local_facet (const ::Deployment::PlanConnectionDescription& conn)
{
  Deployment::Requirements const& deploy_req = conn.deployRequirement;
  for (CORBA::ULong i = 0; i < deploy_req.length (); i++)
  {
    if (ACE_OS::strcmp (deploy_req[i].name, "edu.dre.vanderbilt.DAnCE.ConnectionType") == 0 &&
        ACE_OS::strcmp (deploy_req[i].resourceType, "Local_Interface") == 0)
    {
      return true;
    }
  }
  return false;
}

Components::Cookie*
NodeApplication_Impl::connect_receptacle (const ::Deployment::PlanConnectionDescription& conn,
                                          Components::CCMObject_ptr facet,
                                          const ACE_CString &facet_name,
                                          CORBA::Object_ptr receptacle,
                                          const ACE_CString& recep_name,
                                          CIAO::Deployment::Container_ptr cont)
{
  Components::Cookie* res = 0;
  try
    {
      if (this->is_local_facet (conn))
        {
          ::Components::CCMObject_var ccm_receptacle =
             ::Components::CCMObject::_narrow (receptacle);

          if (CORBA::is_nil (facet))
            {
              DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::connect_receptacle - ")
                            ACE_TEXT ("Unable to narrow facet for local facet connection [%C] to receptable [%C]\n"),
                            facet_name.c_str (), recep_name.c_str ()));
              throw ::Deployment::InvalidConnection ("", "");
            }

          if (CORBA::is_nil (ccm_receptacle))
            {
              DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::connect_receptacle - ")
                            ACE_TEXT ("Unable to narrow receptacle for local facet connection [%C] to receptable [%C]\n"),
                            facet_name.c_str (), recep_name.c_str ()));
              throw ::Deployment::InvalidConnection ("", "");
            }

          if (CORBA::is_nil (cont))
            {
              DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::connect_receptacle - ")
                            ACE_TEXT ("Unable to narrow container for local facet connection [%C] to receptable [%C]\n"),
                            facet_name.c_str (), recep_name.c_str ()));
              throw ::Deployment::InvalidConnection ("", "");
            }

          DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT ("NodeApplication_Impl::connect_receptacle - ")
                        ACE_TEXT ("Connecting local facet [%C] to receptacle [%C]\n"),
                        facet_name.c_str (), recep_name.c_str ()));

          cont->connect_local_facet (facet, facet_name.c_str (), ccm_receptacle, recep_name.c_str ());
          DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::connect_receptacle - connect finished\n")));
        }
      else
        {
          DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::connect_receptacle - ")
                       ACE_TEXT("connect SimplexReceptacle for [%C] started\n"), recep_name.c_str()));
          if (CORBA::is_nil (facet))
            {
              DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT ("NodeApplication_Impl::connect_receptacle - ")
                               ACE_TEXT ("Object reference for facet to connect to [%C] was nil\n"),
                               recep_name.c_str ()));
              throw ::Deployment::InvalidConnection ("",
                                                     "Provided facet reference was nil\n");
            }

          res = facet->connect (recep_name.c_str(), receptacle);
          DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::connect_receptacle - connect finished\n")));
        }
    }
  catch (const ::Components::InvalidName& )
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::connect_receptacle - ")
                   ACE_TEXT("Components::CCMObject_var::connect() returned ::Components::InvalidName exception\n")));
      throw ::Deployment::StartError(conn.name.in (),
                                     "Received InvalidName exception while connecting receptacle.");
    }
  catch (const ::Components::InvalidConnection& )
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT(" NodeApplication_Impl::connect_receptacle - ")
                   ACE_TEXT("Components::CCMObject_var::connect() returned ::Components::InvalidConnection exception\n")));
      throw ::Deployment::InvalidConnection(conn.name.in (),
                                            "InvalidConnection caught while connecting receptacle.");
    }
  catch (const ::Components::AlreadyConnected& )
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT(" NodeApplication_Impl::connect_receptacle - ")
                       ACE_TEXT("Components::CCMObject_var::connect() returned ::Components::AlreadyConnected exception ")
                       ACE_TEXT("for connection <%C>\n"),
                       conn.name.in ()));
      throw ::Deployment::InvalidConnection(conn.name.in (),
                                            "Caught AlreadyConnected exception while connecting receptacle");
    }
  catch (const ::Components::ExceededConnectionLimit& )
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT(" NodeApplication_Impl::connect_receptacle - ")
                   ACE_TEXT("Components::CCMObject_var::connect() returned ::Components::ExceededConnectionLimit exception\n")));
      throw ::Deployment::InvalidConnection(conn.name.in (),
                                            "Caught ExceededConnectionLimit exception while connecting receptacle.");
    }
  catch (const ::CORBA::Exception &ex )
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::connect_receptacle - ")
                       ACE_TEXT("Caught a CORBA exception while connecting <%C>: <%C>\n"),
                       conn.name.in (),
                       ex._info ().c_str ()));
      throw ::Deployment::StartError(conn.name.in (),
                                     ex._info ().c_str ());
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
      DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::connect_receptacle_ext - ")
                   ACE_TEXT("connect SimplexReceptacle for %C started\n"), port_name.c_str()));
      res = inst->connect (port_name.c_str(), facet);
      DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::connect_receptacle_ext - connect finished\n")));
    }
  catch (const ::Components::InvalidName& )
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT(" NodeApplication_Impl::connect_receptacle_ext - ")
                   ACE_TEXT("Components::CCMObject_var::connect() returned ::Components::InvalidName exception\n")));
      throw ::Deployment::StartError("",
                                     "Caught InvalidName exception while connecting external receptacle.");
    }
  catch (const ::Components::InvalidConnection& )
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT(" NodeApplication_Impl::connect_receptacle_ext - ")
                   ACE_TEXT("Components::CCMObject_var::connect() returned ::Components::InvalidConnection exception\n")));
      throw ::Deployment::InvalidConnection("",
                                            "Caught InvalidConnection exception while connecting external receptacle.");
    }
  catch (const ::Components::AlreadyConnected& )
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT(" NodeApplication_Impl::connect_receptacle_ext - ")
                   ACE_TEXT("Components::CCMObject_var::connect() returned ::Components::AlreadyConnected exception\n")));
      throw ::Deployment::InvalidConnection("",
                                            "Caught AlreadyConnected exception while connecting external receptacle.");
    }
  catch (const ::Components::ExceededConnectionLimit& )
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT(" NodeApplication_Impl::connect_receptacle_ext - ")
                   ACE_TEXT("Components::CCMObject_var::connect() returned ::Components::ExceededConnectionLimit exception\n")));
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
      DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::connect_emitter - ")
                   ACE_TEXT("connect_consumer for %C started\n"), port_name.c_str()));
      inst->connect_consumer (port_name.c_str(), event);
      DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::connect_emitter - ")
                   ACE_TEXT("connect_consumer finished\n")));
    }
  catch (const ::Components::InvalidName& )
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT(" ACE_TEXT(NodeApplication_Impl::connect_emitter - ")
                   ACE_TEXT("Components::CCMObject_var::connect_consumer() returned ::Components::InvalidName exception\n")));
      throw ::Deployment::StartError("",
                                     "Caught InvalidName while connecting emitter.");
    }
  catch (const ::Components::AlreadyConnected& )
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT(" NodeApplication_Impl::connect_emitter - ")
                   ACE_TEXT("Components::CCMObject_var::connect_consumer() returned ::Components::AlreadyConnected exception\n")));
      throw ::Deployment::InvalidConnection("",
                                            "Caught AlreadyConnected exception while connecting emitter");
    }
  catch (const ::Components::InvalidConnection& )
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT(" NodeApplication_Impl::connect_emitter - ")
                   ACE_TEXT("Components::CCMObject_var::connect_consumer() returned ::Components::InvalidConnection exception\n")));
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
      DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::connect_emitter_ext - ")
                   ACE_TEXT("connect_emitter_ext for %C started\n"), port_name.c_str()));
      inst->connect_consumer (port_name.c_str(), event);
      DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::connect_emitter_ext - connect_emitter_ext finished\n")));
    }
  catch (const ::Components::AlreadyConnected& )
    {
      DANCE_DEBUG (6, (LM_WARNING, ACE_TEXT("NodeApplication_Impl::connect_emitter_ext - ")
                     ACE_TEXT("Components::CCMObject_var::connect_consumer() returned ::Components::AlreadyConnected exception\n")));
      throw ::Deployment::InvalidConnection("",
                                            "Caught AlreadyConnected exception while connecting external emitter.");
    }
  catch (const ::Components::InvalidConnection& )
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT(" NodeApplication_Impl::connect_emitter_ext - ")
                   ACE_TEXT("Components::CCMObject_var::connect_consumer() returned ::Components::InvalidConnection exception\n")));
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
      DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::connect_publisher - ")
                    ACE_TEXT("Provided a nil CCMObject reference while connecting port %C\n"),
                    port_name.c_str ()));
      throw ::Deployment::InvalidConnection ();
    }
  Components::Cookie* res = 0;
  Components::EventConsumerBase_var event = Components::EventConsumerBase::_unchecked_narrow (consumer);
  try
    {
      res = inst->subscribe (port_name.c_str(), event);
      DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::connect_publisher - successfully subscribed %C\n"),
                   port_name.c_str ()));
    }
  catch (const ::Components::InvalidName& )
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT(" NodeApplication_Impl::connect_publisher - ")
                   ACE_TEXT("Components::CCMObject_var::subscribe() returned ::Components::InvalidName exception\n")));
      throw ::Deployment::StartError("", "Caught InvalidName exception while connecting publisher");
    }
  catch (const ::Components::InvalidConnection& )
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT(" NodeApplication_Impl::connect_publisher - ")
                   ACE_TEXT("Components::CCMObject_var::subscribe() returned ::Components::InvalidConnection exception\n")));
      throw ::Deployment::InvalidConnection("", "Caught InvalidConnection exception while connecting publisher.");
    }
  catch (const ::Components::ExceededConnectionLimit& )
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT(" NodeApplication_Impl::connect_publisher - ")
                   ACE_TEXT("Components::CCMObject_var::subscribe() returned ::Components::ExceededCOnnectionLimit exception\n")));
      throw ::Deployment::InvalidConnection("", "Caught ExceededConnectionLimit exception while connecting publisher.");
    }
  return res;
}

