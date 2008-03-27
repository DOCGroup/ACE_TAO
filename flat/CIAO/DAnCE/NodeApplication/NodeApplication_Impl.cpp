// $Id$


#include "ace/OS_Memory.h"
#include "ace/Log_Msg.h"
#include "ace/streams.h"

#include "ciao/Valuetype_Factories/ConfigValue.h"
#include "ccm/CCM_ObjectC.h"
#include "Cdmw/CDMW_IDL_ExtC.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "Deployment/Deployment_BaseC.h"
#include "Deployment/Deployment_ApplicationC.h"

#include "ComponentAttributesSetter.h"
#include "ServerActivator_Impl.h"
#include "ComponentInstallation_Impl.h"
#include "NodeApplication_Impl.h"


bool read_config_value (const ACE_CString & name,
                        const Deployment::Properties & prop,
                        CORBA::Any_out value)
throw()
{
  ACE_CString cdmw_name = "cdmw.config." + name;
  bool found = false;
  CORBA::ULong len = prop.length();
  for (CORBA::ULong count = 0; count < len; ++count)
    if (cdmw_name.compare (prop[count].name.in()) == 0)
      {
        value = new CORBA::Any (prop[count].value);
        found = true;
        break;
      }
  return found;
}

bool find_config_value (const ACE_CString & name,
                        const Deployment::Properties & prop,
                        CORBA::Any_out value)
throw()
{
  bool found = false;
  CORBA::ULong len = prop.length();
  for (CORBA::ULong count = 0; count < len; ++count)
    if (name.compare (prop[count].name.in()) == 0)
      {
        value = new CORBA::Any (prop[count].value);
        found = true;
        break;
      }
  return found;
}

bool find_config_value_cfg (const ACE_CString & name,
                            const Components::ConfigValues & prop,
                            CORBA::Any_out value)
throw()
{
  bool found = false;
  CORBA::ULong len = prop.length();
  for (CORBA::ULong count = 0; count < len; ++count)
    if (name.compare (prop[count]->name ()) == 0)
      {
        value = new CORBA::Any (prop[count]->value());
        found = true;
        break;
      }
  return found;
}


void dump_config_values (const Components::ConfigValues& cv)
{
  for (unsigned int i = 0; i < cv.length(); i++)
    {
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::dump_config_values - "
                   "ConfigValue name = %s\n", cv[i]->name()));
    }
}
using namespace DAnCE;

NodeApplication_Impl::NodeApplication_Impl (CORBA::ORB_ptr orb
                                            , PortableServer::POA_ptr poa
                                            , const Deployment::DeploymentPlan& plan
                                            , RedirectionService & redirection
                                            , const ACE_CString& node_name)
    : orb_ (CORBA::ORB::_duplicate (orb))
    , poa_ (PortableServer::POA::_duplicate (poa))
    , plan_ (plan)
    , redirection_ (redirection)
    , node_name_ (node_name)
{
  DANCE_TRACE (DLINFO "NodeApplication_Impl::NodeApplication_Impl");
}

NodeApplication_Impl::~NodeApplication_Impl()
{
  DANCE_TRACE(DLINFO "NodeApplication_Impl::~NodeApplication_Impl()")

  using namespace Components;
  ConfigValues config_values;
  config_values.length (1L);
  CORBA::Any feature_any;
  for (TContainers::iterator iter = this->containers_.begin();
       iter != this->containers_.end();
       ++iter)
    {
      /* TODO: This is highly suspect.  I believe we should be using get_component_server,
       not calling create_container. */
      try
        {
          feature_any <<= CORBA::string_dup ( (*iter).ext_id_.c_str());
          config_values[0] = new CIAO::ConfigValue_impl (PROCESS_DESTINATION,
                                                         feature_any);
          DANCE_DEBUG((LM_TRACE, DLINFO " ~NodeApplication_impl - before create_component_server\n"));
          Components::Deployment::ComponentServer_var compServer =
            this->activator_->create_component_server (config_values);
          DANCE_DEBUG((LM_TRACE, DLINFO " ~NodeApplication_impl - before remove_container\n"));
          compServer->remove_container ( (*iter).int_id_.in());
          DANCE_DEBUG((LM_TRACE, DLINFO " ~NodeApplication_impl - before remove_component_server\n"));
          this->activator_->remove_component_server (compServer.in());
          DANCE_DEBUG((LM_TRACE, DLINFO " ~NodeApplication_impl - after remove_component_server\n"));
        }
      catch (...)
        {
          DANCE_DEBUG((LM_ERROR, DLINFO
                       "~NodeApplication_impl - An error has ocured during removing of container \"%s\"\n"
                       (*iter).ext_id_.c_str()));
        }
    }
}

void
NodeApplication_Impl::init()
{
  DANCE_TRACE(DLINFO "NodeApplication_Impl::init()");
  
  /* TODO:  Lets move this stuff to the constructor, shall we?!? */
  /* TODO:  Might be nice to use a component configurator here to load the proper versions
     of the serveractivator.  */
  ACE_NEW_THROW_EX (this->installation_,
                    ComponentInstallation_Impl(),
                    CORBA::NO_MEMORY ());

  PortableServer::ObjectId_var ci_id =
    this->poa_->activate_object (this->installation_);

  DANCE_DEBUG((LM_TRACE, DLINFO "NodeApplication_impl::init - ComponentInstallation object created\n"));
  CORBA::Object_var obj = this->poa_->id_to_reference (ci_id.in());
  Components::Deployment::ComponentInstallation_var ci =
    Components::Deployment::ComponentInstallation::_narrow (this->poa_->servant_to_reference (this->installation_));
  ACE_NEW_THROW_EX (this->activator_,
                    ServerActivator_Impl (this->orb_.in(),
                                          this->poa_.in(),
                                          ci.in()),
                    CORBA::NO_MEMORY ());

  PortableServer::ObjectId_var sa_id =
    this->poa_->activate_object (this->activator_);

  DANCE_DEBUG((LM_TRACE, DLINFO "NodeApplication_impl::init - ServerActivator object created\n"));
}

void
NodeApplication_Impl::start ()
{
  DANCE_TRACE(DLINFO "NodeApplication_Impl::start ()");
  
  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::start - started\n"));
  for (TComponents::iterator iter = this->components_.begin();
       iter != this->components_.end();
       ++iter)
    {
      try
        {
          DANCE_DEBUG((LM_TRACE, DLINFO "NodeApplication_impl::start - "
                       "configuration_complete started for %s\n", (*iter).ext_id_.c_str()));
          (*iter).int_id_->configuration_complete();
          DANCE_DEBUG((LM_TRACE, DLINFO "NodeApplication_impl::start - "
                       "configuration_complete finished\n"));
        }
      catch (::Components::InvalidConfiguration& )
        {
          DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::start - "
                       "Components::CCMObject_var::configuration_complete() "
                       "returned ::Components::InvalidConfiguration exception.\n"));
          throw ::Deployment::StartError();
        }
    }
}

void
NodeApplication_Impl::create_home (unsigned int index)
{
  DANCE_TRACE(DLINFO "NodeApplication_Impl::create_home (unsigned int index)");

  ACE_CString uuid = this->plan_.UUID.in ();
  uuid += "/";
  uuid += this->plan_.instance[index].name.in();
  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_home - Home uuid is %s\n", uuid.c_str()));

  if (0 == this->homes_.find (uuid))
    {
      // Home already exists so we shouldn't do anything
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_home - requested home already exists\n"));
      return;
    }

  Components::ConfigValues config_values;
  ACE_CString processDest;
  CORBA::Any_var feature_any;
  if (read_config_value (Components::PROCESS_DESTINATION,
                         this->plan_.instance[index].configProperty,
                         feature_any.out()))
    {
      const char* str;
      feature_any >>= str;
      processDest = str;
    }
  else
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_home - read_config_value() function fails\n"));
      throw ::Deployment::InvalidProperty();
    }
  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_home - Process destination is %s\n", processDest.c_str()));

  // We should pass library location to ComponentInstallation object
  ACE_CString location =
    this->plan_.artifact[this->plan_.implementation[this->plan_.instance[index].implementationRef].artifactRef[0]].location[0].in();
  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_home - container library location is %s\n", location.c_str()));
  try
    {
      this->installation_->install (uuid.c_str(), location.c_str());
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_home - "
                   "ComponentInstallation::install succesfull\n"));
    }
  catch (::Components::Deployment::InvalidLocation& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_home - "
                   "ComponentInstallation_var::install() returned "
                   "::Components::Deployment::InvalidLocation exception\n"));
      throw ::Deployment::InvalidProperty();
    }
  catch (::Components::Deployment::InstallationFailure& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_home - "
                   "ComponentInstallation_var::install() returned "
                   "::Components::Deployment::InstallationFailure exception\n"));
      throw ::Deployment::StartError();
    }
  // This variable is needed for workaround while parsing complex types doesn't work
  CdmwDeployment::ValuetypeFactoryDescriptionSeq  valuetypes;

  // We should pass all repids from artifacts in our implementation to ComponenetInstallation
  for (CORBA::ULong i = 0;
       i < this->plan_.implementation[this->plan_.instance[index].implementationRef].artifactRef.length();
       ++i)
    {
      unsigned int indx = this->plan_.implementation[this->plan_.instance[index].implementationRef].artifactRef[i];
      ACE_CString factory_entrypoint;
      ACE_CString repid;
      unsigned int vt_index = valuetypes.length();
      if (find_config_value ("factory_entrypoint",
                             plan_.artifact[indx].execParameter,
                             feature_any.out()))
        {
          const char * tmp;
          feature_any >>= tmp;
          factory_entrypoint = tmp;
        }
      if (find_config_value ("repid",
                             plan_.artifact[indx].execParameter,
                             feature_any.out()))
        {
          const char * tmp;
          feature_any >>= tmp;
          repid = tmp;
        }
      if (0 != factory_entrypoint.length () && 0 != repid.length ())
        {
          valuetypes.length (vt_index + 1);
          valuetypes[vt_index].factory_entrypoint = CORBA::string_dup (factory_entrypoint.c_str ());
          valuetypes[vt_index].repid = CORBA::string_dup (repid.c_str ());
          try
            {
              this->installation_->install_valuetypefactory_location (
                uuid.c_str (),
                repid.c_str (),
                location.c_str ());
              DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_home - "
                           "ComponentInstallation::install_valuetypefactory_location succesfull\n"));
            }
          catch (::Components::Deployment::InvalidLocation& )
            {
              DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_home - "
                           "ComponentInstallation_var::install_valuetypefactory_location() "
                           "returned ::Components::Deployment::InvalidLocation exception\n"));
              throw ::Deployment::InvalidProperty();
            }
          catch (::Components::Deployment::InstallationFailure& )
            {
              DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_home - "
                           "ComponentInstallation_var::install_valuetypefactory_location() "
                           "returned ::Components::Deployment::InstallationFailure exception\n"));
              throw ::Deployment::StartError();
            }
        }
      else if (0 != factory_entrypoint.length () || 0 != repid.length ())
        {
          DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_home - "
                       "Deployment plan ERROR - not full description of valuetypefactory!\n"));
          throw ::Deployment::InvalidProperty();
        }
    }

  if (0 != this->containers_.find (processDest.c_str()))
    {
      // Correspondent container for this home is not created yet
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_home - create container\n"));

      this->create_container (index);
    }
  Components::Deployment::Container_var container;
  if (0 != this->containers_.find (processDest, container))
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_home - "
                   "Error creating Container for destination %s\n", processDest.c_str()));
      throw Deployment::StartError();
    }
  this->create_config_values (this->plan_.implementation[this->plan_.instance[index].implementationRef].execParameter,
                            eInstallHome,
                            config_values);

  // This code is workaround while parsing complex types doesn't work
  CORBA::Any any;
  if (0 != valuetypes.length())
    {
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_home - Valuetypes dependencies added\n"));
      any <<= valuetypes;
      unsigned int cv_indx = config_values.length();
      config_values.length (cv_indx + 1);
      config_values[cv_indx] = new CIAO::ConfigValue_impl (Components::VALUETYPE_FACTORY_DEPENDENCIES,
                                                           any);
    }

  CORBA::String_var home_entrypoint;
  // TODO Entrypoint varibale name should be specified as constant
  if (find_config_value ("home factory",
                         this->plan_.implementation[this->plan_.instance[index].implementationRef].execParameter,
                         feature_any.out()))
    {
      const char* entrypoint;
      feature_any >>= entrypoint;
      home_entrypoint = CORBA::string_dup (entrypoint);
    }
  else
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_home - "
                   "variable \'home factory\' has not found for InstallHome operation \n"));
      throw ::Deployment::InvalidProperty();
    }

  Components::CCMHome_var obj;
  try
    {
//      CdmwDeployment::ValuetypeFactoryDescriptionSeq* values_seq = 0;
//      DANCE_DEBUG(("NodeApplication_impl::create_home - checking VALUETYPE_FACTORY_DEPENDENCIES\n"));
//      CORBA::Any_var checkin_any;
//      if (find_config_value_cfg(Components::VALUETYPE_FACTORY_DEPENDENCIES, config_values, checkin_any.out ()))
//        {
//          DANCE_DEBUG(("NodeApplication_impl::create_home - VALUETYPE_FACTORY_DEPENDENCIES exist\n"));
//          checkin_any.in () >>= values_seq;
//          if (0 == values_seq)
//            {
//              DANCE_ERROR(("NodeApplication_impl::create_home - Failed to extruce valuetype factory\n"));
//              throw ::Deployment::InvalidProperty();
//            }
//          DANCE_DEBUG(("NodeApplication_impl::create_home - VALUETYPE_FACTORY_DEPENDENCIES has correct value\n"));
//        }

      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_home - installing home\n"));
      CORBA::String_var home_uuid = CORBA::string_dup (uuid.c_str());
      obj = container->install_home (home_uuid.in(),
                                     home_entrypoint.in(),
                                     config_values);
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_home - home installed\n"));
    }
  catch (::Components::Deployment::UnknownImplId& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_home - "
                   "Container::install_home() returned "
                   "::Components::Deployment::UnknownImplId exception\n"));
      throw ::Deployment::InvalidProperty();
    }
  catch (::Components::Deployment::ImplEntryPointNotFound& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_home - "
                   "Container::install_home() returned "
                   "::Components::Deployment::ImplEntryPointNotFound exception\n"));
      throw ::Deployment::InvalidProperty();
    }
  catch (::Components::Deployment::InstallationFailure &e)
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_home - Container::install_home() returned "
                   "::Components::Deployment::InstallationFailure exception with code %#x\n", 
                   e.reason));
      throw ::Deployment::StartError();
    }
  catch (::Components::Deployment::InvalidConfiguration &e)
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_home - "
                   "Container::install_home() returned ::Components::Deployment::InvalidConfiguration exception. "
                   "Name: %s. Reason: 0x%X\n", 
                   e.name.in(), 
                   e.reason));
      throw ::Deployment::InvalidProperty();
    }
  Components::KeylessCCMHome_var home = Components::KeylessCCMHome::_narrow (obj);
  this->homes_.rebind (uuid, Components::KeylessCCMHome::_duplicate (home));

  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_home - finished\n"));
}

void
NodeApplication_Impl::create_container (unsigned int index)
{
  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_container - started\n"));

  Components::ConfigValues config_values;
  ACE_CString processDest;
  CORBA::Any_var feature_any;
  if (read_config_value (Components::PROCESS_DESTINATION,
                         this->plan_.instance[index].configProperty,
                         feature_any))
    {
      const char * tmp;
      feature_any >>= tmp;
      processDest = tmp;
    }
  else
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_container - "
                   "read_config_value() function fails\n"));
      throw ::Deployment::InvalidProperty();
    }
  // PROCESS_DESTINATION

  this->create_config_values (this->plan_.instance[index].configProperty,
                            eCreateComponentServer,
                            config_values);

  ::Components::Deployment::ComponentServer_var compServer;
  try
    {
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_container - "
                   "creating component server for destination: %s\n", processDest.c_str()));
      compServer = this->activator_->create_component_server (config_values);
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_container - "
                   "component server created\n"));
    }
  catch (::Components::CreateFailure& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_container - "
                   "Components::Deployment::ServerActivator_var::create_component_server() "
                   "returned ::Components::CreateFailure exception\n"));
      throw ::Deployment::StartError();
    }
  catch (::Components::Deployment::InvalidConfiguration& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_container - "
                   "Components::Deployment::ServerActivator_var::create_component_server() "
                   "returned ::Components::Deployment::InvalidConfiguration exception\n"));
      throw ::Deployment::InvalidProperty();
    }

  // COMPONENT_KIND
  this->create_config_values (this->plan_.implementation[this->plan_.instance[index].implementationRef].execParameter,
                            eCreateContainer,
                            config_values);
  try
    {
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_container - "
                   "creating container for destination: %s\n", processDest.c_str()));
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

void
NodeApplication_Impl::create_component (unsigned int index)
{
  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_component - started\n"));

  // Looking for home name for current component
  ACE_CString homeName;
  CORBA::Any_var feature_any;
  if (find_config_value ("cdmw.component_home",
                         this->plan_.instance[index].configProperty,
                         feature_any))
    {
      const char* tmp;
      feature_any >>= tmp;
      homeName = tmp;
    }
  else
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_component - read_config_value() function fails\n"));
      throw ::Deployment::InvalidProperty();
    }
  // Generating home uuid
  ACE_CString uuid = this->plan_.UUID.in();
  uuid += "/";
  uuid += homeName;

  if (0 != this->homes_.find (uuid))
    {
      // Correspondent home doesn't exists
      // we should find it in instance sequence of deployment plan
      for (unsigned int i = 0; i <  this->plan_.instance.length(); i++)
        {
          if (homeName.compare (this->plan_.instance[i].name.in()) == 0)
            {
              // and create it
              this->create_home (i);
              break;
            }
        }
    }
  Components::KeylessCCMHome_var home;
  if (0 != this->homes_.find (uuid, home))
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_component - failed to find homes\n"));
      throw ::Deployment::StartError();
    }

  Components::ConfigValues config_values;
  this->create_config_values (this->plan_.instance[index].configProperty,
                            config_values);

  Components::CCMObject_var component;
  try
    {
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_component - creating component with config values\n"));
      CdmwCcmCif::KeylessCCMHome_var cdmw_home =
        CdmwCcmCif::KeylessCCMHome::_narrow (home.in ());
      component = cdmw_home->create_component_with_config_values (config_values);
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_component - component created\n"));
      if (CORBA::is_nil (component))
        {
          DANCE_DEBUG((LM_DEBUG, DLINFO "Components::CCMObject_ptr is nil\n"));
        }
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_component - setting component attributes\n"));
      ComponentAttributesSetter::SetComponentAttributes (this->plan_.instance[index].name.in(),
                                                         component,
                                                         this->plan_.instance[index].configProperty,
                                                         this->orb_.in());
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_component - component attributes have been setted\n"));
    }
  catch (::Components::CreateFailure& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_component - "
                   "CdmwCcmCif::KeylessCCMHome_var::create_component_with_config_values() "
                   "returned ::Components::CreateFailure exception\n"));
      throw ::Deployment::StartError();
    }

  this->components_.rebind (this->plan_.instance[index].name.in(), Components::CCMObject::_duplicate (component));

  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_component - finished\n"));
}

void
NodeApplication_Impl::init_components()
{
  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::init_components - started\n"));

  Components::ConfigValues config_values;
  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::init_components - "
               "this->plan_.instance.length() = %u\n", 
               this->plan_.instance.length()));

  for (unsigned int i = 0; i < this->plan_.instance.length(); i++)
    {
      try
      {
      switch (this->get_instance_type (this->plan_.instance[i].configProperty))
        {
        case eHome:
        {
          this->create_home (i);
          break;
        }
        case eComponent:
        {
          this->create_component (i);
          break;
        }
        default:
        {
          DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::init_components - "
                       "get_instance_type function returned incorrect instance type\n"));
          throw ::Deployment::InvalidProperty();
        }
        } // switch
      } catch (...)
      {
        DANCE_ERROR((LM_ERROR, DLINFO "Exception was thrown while creating instance \"%s\".\n", this->plan_.instance[i].name.in()));
        throw;
      }
    }

  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::init_components - finished\n"));
}

NodeApplication_Impl::EInstanceType
NodeApplication_Impl::get_instance_type (const Deployment::Properties& prop) const
{
  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::get_instance_type - started\n"));

  CORBA::Any_var feature_any;
  if (read_config_value (Components::PROCESS_DESTINATION, prop, feature_any))
    {
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::get_instance_type - finished\n"));
      return eHome;
    }
  if (read_config_value (Components::COMPONENT_NAME, prop, feature_any))
    {
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::get_instance_type - finished\n"));
      return eComponent;
    }

  DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::get_instance_type - "
               "current instance is not a Home and not a Component type\n"));
  throw ::Deployment::InvalidProperty();
}

void
NodeApplication_Impl::create_config_values (const Deployment::Properties& prop,
                                          const ERequestType request,
                                          Components::ConfigValues& cfg) const
{
  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_config_values - started\n"));

  unsigned int ind = 0;
  CORBA::Any_var feature_any;
  switch (request)
    {
    case eCreateComponentServer:
    {
      cfg.length (1L);
      if (read_config_value (Components::PROCESS_DESTINATION, prop, feature_any))
        {
          DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_config_values PROCESS_DESTINATION variable has been added for CreateComponentServer operation\n"));
          cfg[ind] = new CIAO::ConfigValue_impl (Components::PROCESS_DESTINATION,
                                                 feature_any.in());
          ind++;
        }
      else
        {
          DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_config_values - variable PROCESS_DESTINATION has not found for CreateComponentServer operation\n"));
          throw ::Deployment::InvalidProperty();
        }
      break;
    }
    case eCreateContainer:
    {
      cfg.length (1L);
      if (read_config_value (Components::COMPONENT_KIND, prop, feature_any))
        {
          DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_config_values COMPONENT_KIND variable has been added for CreateContainer operation\n"));
          cfg[ind] = new CIAO::ConfigValue_impl (Components::COMPONENT_KIND,
                                                 feature_any.in());
          ind++;
        }
      else
        {
          DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_config_values - variable COMPONENT_KIND has not found for CreateContainer operation\n"));
          throw ::Deployment::InvalidProperty();
        }
      break;
    }
    case eInstallHome:
    {
      cfg.length (0L);
      if (read_config_value (Components::COMPONENT_KIND, prop, feature_any))
        {
          DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_config_values COMPONENT_KIND variable has been added for InstallHome operation\n"));
          cfg.length (ind + 1);
          cfg[ind] = new CIAO::ConfigValue_impl (Components::COMPONENT_KIND,
                                                 feature_any.in());
          ind++;
        }
      else
        {
          DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_config_values - variable COMPONENT_KIND has not found for InstallHome operation\n"));
          throw ::Deployment::InvalidProperty();
        }
      if (read_config_value (Components::SERVANT_LIFETIME, prop, feature_any))
        {
          DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_config_values SERVANT_LIFETIME variable has been added for InstallHome operation\n"));
          cfg.length (ind + 1);
          cfg[ind] = new CIAO::ConfigValue_impl (Components::SERVANT_LIFETIME,
                                                 feature_any.in());
          ind++;
        }
      //TODO Clarify is this parameter is compulsory
//            else {
//                // TODO: Add traces
//                throw InvalidProperty();
//            }
      if (read_config_value (Components::THREADING_POLICY, prop, feature_any))
        {
          DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_config_values THREADING_POLICY variable has been added for InstallHome operation\n"));
          cfg.length (ind + 1);
          cfg[ind] = new CIAO::ConfigValue_impl (Components::THREADING_POLICY,
                                                 feature_any.in());
          ind++;
        }
      else
        {
          DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_config_values - variable THREADING_POLICY has not found for InstallHome operation\n"));
          throw ::Deployment::InvalidProperty();
        }
      if (read_config_value (Components::HOME_REPOSITORY_ID, prop, feature_any))
        {
          DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_config_values HOME_REPOSITORY_ID variable has been added for InstallHome operation\n"));
          cfg.length (ind + 1);
          cfg[ind] = new CIAO::ConfigValue_impl (Components::HOME_REPOSITORY_ID,
                                                 feature_any.in());
          ind++;
        }
      else
        {
          DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_config_values - variable HOME_REPOSITORY_ID has not found for InstallHome operation\n"));
          throw ::Deployment::InvalidProperty();
        }
      if (read_config_value (Components::HOME_SERVANT_CLASSNAME, prop, feature_any))
        {
          DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_config_values HOME_SERVANT_CLASSNAME variable has been added for InstallHome operation\n"));
          cfg.length (ind + 1);
          cfg[ind] = new CIAO::ConfigValue_impl (Components::HOME_SERVANT_CLASSNAME,
                                                 feature_any.in());
          ind++;
        }
      else
        {
          DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_config_values - variable HOME_SERVANT_CLASSNAME has not found for InstallHome operation\n"));
          throw ::Deployment::InvalidProperty();
        }
      if (read_config_value (Components::VALUETYPE_FACTORY_DEPENDENCIES, prop, feature_any))
        {
          DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_config_values VALUETYPE_FACTORY_DEPENDENCIES variable has been added for InstallHome operation\n"));
          cfg.length (ind + 1);
          cfg[ind] = new CIAO::ConfigValue_impl (Components::VALUETYPE_FACTORY_DEPENDENCIES,
                                                 feature_any.in());
          ind++;
        }
      //TODO Clarify is this parameter is compulsory
//            else {
//                // TODO: Add traces
//                throw InvalidProperty();
//            }
      if (read_config_value (Components::FAULT_TOLERANCE_REPLICATION_STYLE, prop, feature_any))
        {
          DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_config_values FAULT_TOLERANCE_REPLICATION_STYLE variable has been added for InstallHome operation\n"));
          cfg.length (ind + 1);
          cfg[ind] = new CIAO::ConfigValue_impl (Components::FAULT_TOLERANCE_REPLICATION_STYLE,
                                                 feature_any.in());
          ind++;
        }
      //TODO Clarify is this parameter is compulsory
//            else {
//                // TODO: Add traces
//                throw InvalidProperty();
//            }
      break;
    }
    case eCreateComponentWithConfigValues:
    {
      cfg.length (0);
      if (read_config_value (Components::COMPONENT_NAME, prop, feature_any))
        {
          DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_config_values COMPONENT_NAME variable has been added for CreateComponentWithConfigValues operation\n"));
          cfg.length (ind + 1);
          cfg[ind] = new CIAO::ConfigValue_impl (Components::COMPONENT_NAME,
                                                 feature_any.in());
          ind++;
        }
      else
        {
          DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_config_values - variable COMPONENT_NAME has not found for CreateComponentWithConfigValues operation\n"));
          throw ::Deployment::InvalidProperty();
        }
      if (read_config_value (Components::USES_PORT_TIMEOUTS, prop, feature_any))
        {
          DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_config_values USES_PORT_TIMEOUTS variable has been added for CreateComponentWithConfigValues operation\n"));
          cfg.length (ind + 1);
          cfg[ind] = new CIAO::ConfigValue_impl (Components::USES_PORT_TIMEOUTS,
                                                 feature_any.in());
          ind++;
        }
      if (read_config_value (Components::FAULT_TOLERANCE_GROUP_NAME, prop, feature_any))
        {
          DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_config_values FAULT_TOLERANCE_GROUP_NAME variable has been added for CreateComponentWithConfigValues operation\n"));
          cfg.length (ind + 1);
          cfg[ind] = new CIAO::ConfigValue_impl (Components::FAULT_TOLERANCE_GROUP_NAME,
                                                 feature_any.in());
          ind++;
        }

      //TODO Clarify is this parameter is compulsory
//            else {
//                // TODO: Add traces
//                throw InvalidProperty();
//            }

//            Commented, because RECORDING property doesn't implemented yet
//            if (read_config_value (CdmwDeployment::RECORDING, prop, feature_any)) {
//                cfg[ind] = new Container::ConfigValue_impl(    CdmwDeployment::RECORDING,
//                                        feature_any);
//                ind++;
//            }
//            else {
//                // TODO: Add traces
//                throw InvalidProperty();
//            }

      break;
    }
    default:
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::create_config_values - request is not a know type: eCreateComponentServer, eCreateContainer, eInstallHome, eCreateComponentWithConfigValues\n"));
      throw ::Deployment::InvalidProperty();
    }
    }

  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::create_config_values - finished\n"));
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

  this->redirection_.registration_start (this->node_name_, this->plan_.UUID.in());

  // CCMObjects registration
  for (TComponents::iterator it = this->components_.begin();
       !it.done();
       ++it)
    {
      this->redirection_.registration (this->node_name_,
                                       this->plan_.UUID.in(),
                                       (*it).ext_id_,
                                       (*it).int_id_.in());

    }
  for (unsigned int i = 0; i < this->plan_.connection.length(); i++)
    {
      for (unsigned int j = 0; j < this->plan_.connection[i].internalEndpoint.length(); j++)
        {
          if (this->plan_.connection[i].internalEndpoint[j].provider)
            {
              index = conn->length ();
              conn->length (index + 1);
              (*conn) [index].name = CORBA::string_dup (this->plan_.connection[i].name.in());
              Components::CCMObject_var obj;
              ACE_CString inst_name = 
                this->plan_.instance[this->plan_.connection[i].internalEndpoint[j].instanceRef].name.in();
              if (0 != this->components_.find (inst_name, obj))
                {
                  DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::getAllConnections - "
                               "component instance for current connection cannot be found\n"));
                  throw ::Deployment::InvalidProperty();
                }
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
                                       "provide_facet for connection %s endpoint %s started\n", 
                                       this->plan_.connection[i].name.in(), 
                                       this->plan_.connection[i].internalEndpoint[j].portName.in()));
                          CORBA::String_var facet_name = CORBA::string_dup (name.c_str());
                          (*conn) [index].endpoint[0] = obj->provide_facet (facet_name.in());
                          DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::getAllConnections - "
                                       "provide_facet for connection %s endpoint %s finished\n", 
                                       this->plan_.connection[i].name.in(), 
                                       this->plan_.connection[i].internalEndpoint[j].portName.in()));
                        }
                      else
                        {
                          DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::getAllConnections - "
                                       "provide_facet for connection %s endpoint %s started\n", 
                                       this->plan_.connection[i].name.in(), 
                                       this->plan_.connection[i].internalEndpoint[j].portName.in()));
                          (*conn) [index].endpoint[0] = CORBA::Object::_duplicate (obj.in());
                        }
                      this->redirection_.registration (this->node_name_,
                                                       this->plan_.UUID.in(),
                                                       inst_name,
                                                       name,
                                                       (*conn) [index].endpoint[0].in());
                      //app_node.addChild(path.c_str(), obj->provide_facet(facet_name));
                      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::getAllConnections - provide_facet finished\n"));
                    }
                  catch (::Components::InvalidName& )
                    {
                      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::getAllConnections - "
                                   "Components::CCMObject_var::provide_facet() returned "
                                   "::Components::InvalidName exception\n"));
                      throw ::Deployment::InvalidProperty();
                    }
                  break;
                }
                case Deployment::EventConsumer:
                {
                  try
                    {
                      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::getAllConnections - "
                                   "get_consumer for connection %s endpoint %s started\n", 
                                   this->plan_.connection[i].name.in(), 
                                   this->plan_.connection[i].internalEndpoint[j].portName.in()));
                      (*conn) [index].endpoint[0] = obj->get_consumer (this->plan_.connection[i].internalEndpoint[j].portName.in());
                      this->redirection_.registration (this->node_name_,
                                                       this->plan_.UUID.in(),
                                                       inst_name,
                                                       this->plan_.connection[i].internalEndpoint[j].portName.in(),
                                                       (*conn) [index].endpoint[0].in());

                      //app_node.addChild(path.c_str(), obj->get_consumer(this->plan_.connection[i].internalEndpoint[j].portName));
                      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::getAllConnections - "
                                   "get_consumer finished\n"));
                    }
                  catch (::Components::InvalidName& )
                    {
                      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::getAllConnections - "
                                   "Components::CCMObject_var::get_consumer() returned "
                                   "::Components::InvalidName exception\n"));
                      throw ::Deployment::InvalidProperty();
                    }
                  break;
                }
                default:
                {
                  DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::getAllConnections - "
                               "Connection.InternalEndPoint.Kind is not a "
                               "Deployment::Facet or Deployment::EventConsumer\n"));
                  throw ::Deployment::InvalidProperty();
                }
                }
              //              index++;
            }
        }
    }
  this->redirection_.registration_finish (this->node_name_, this->plan_.UUID.in());
  //this->node_.registerObjects(this->plan_.UUID.in(), app_node);
  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::getAllConnections - finished\n"));
  return conn._retn();
}

void
NodeApplication_Impl::finishLaunch (const Deployment::Connections & providedReference,
                                    ::CORBA::Boolean start)
{
  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - started for connection \n"));
  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - "
                 "started for connections sequence with length: %d\n", providedReference.length()));

  for (unsigned int j = 0; j < this->plan_.connection.length(); ++j)
    {
      Components::CCMObject_var obj;
      if (0 != this->components_.find (this->plan_.instance[this->plan_.connection[j].internalEndpoint[0].instanceRef].name.in(), obj))
        {
          DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                         "component instance for the connection \"%s\" cannot be found.\n", 
                         this->plan_.connection[j].name.in()));
          throw ::Deployment::StartError();
        }

      for (unsigned int i = 0; i < providedReference.length(); ++i)
        {
          //DANCE_DEBUG((LM_DEBUG, "[%M] NodeApplication_impl::finishLaunch - loop on all connections iteration %d for connection %s\n", i, providedReference[i].name.in()));
          ACE_CString name = this->plan_.connection[j].name.in();
          if (name.compare (providedReference[i].name.in()) == 0)
            {
              switch (this->plan_.connection[j].internalEndpoint[0].kind)
                {
                case Deployment::Facet:
                {
                  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - "
                               "set for facet %s \n", name.c_str ()));
                  Components::CCMObject_var ext_inst;
                  try
                    {
                      if (0 == this->plan_.connection[j].externalReference.length())
                        {
                          break;
                        }
                      CORBA::Object_var tmp = 
                        this->orb_->string_to_object (this->plan_.connection[j].externalReference[0].location.in());
                      ext_inst = Components::CCMObject::_narrow (tmp);
                      if (CORBA::is_nil (ext_inst.in()))
                        {
                          DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                                       "facet for %s can't be narrowed \n", name.c_str ()));
                          break;
                        }
                      this->connect_receptacle_ext (ext_inst,
                                                  this->plan_.connection[j].externalReference[0].portName.in(),
                                                  providedReference[i].endpoint[0].in());
                    }
                  catch (CORBA::OBJECT_NOT_EXIST&)
                    {
                      // @@TODO: Shouldn't this be an error?!?
                      break;
                    }
                  catch (CORBA::TRANSIENT&)
                    {
                      // @@TODO: Shouldn't this be an error?!?
                      break;
                    }

                  break;
                }
                case Deployment::EventConsumer:
                {
                  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - set for consumer \n"));
                  Components::CCMObject_var ext_inst;
                  try
                    {
                      if (0 == this->plan_.connection[j].externalReference.length())
                        {
                          break;
                        }
                      CORBA::Object_var tmp = 
                        this->orb_->string_to_object (this->plan_.connection[j].externalReference[0].location.in());
                      ext_inst = Components::CCMObject::_narrow (tmp);
                      if (CORBA::is_nil (ext_inst.in()))
                        {
                          DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                                       "facet for %s can't be narrowed \n", name.c_str ()));
                          break;
                        }
                      try
                        {
                          // Check is connection kind is consumer to emitter?
                          this->connect_emitter_ext (ext_inst,
                                                   this->plan_.connection[j].externalReference[0].portName.in(),
                                                   providedReference[i].endpoint[0].in());
                        }
                      catch (::Components::InvalidName&)
                        {
                          // No this is consumer to publisher
                          this->connect_publisher (ext_inst,
                                                  this->plan_.connection[j].externalReference[0].portName.in(),
                                                  providedReference[i].endpoint[0].in());
                        }
                    }
                  catch (CORBA::OBJECT_NOT_EXIST&)
                    {
                      break;
                    }
                  catch (CORBA::TRANSIENT&)
                    {
                      break;
                    }
                  break;
                }
                case Deployment::MultiplexReceptacle:
                case Deployment::SimplexReceptacle:
                {
                  // What we should do with Cookie, returned from connect call???
                  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - set for receptacle \n"));
                  this->connect_receptacle (obj.in(),
                                           this->plan_.connection[j].internalEndpoint[0].portName.in(),
                                           providedReference[i].endpoint[0].in());
                  break;
                }
                case Deployment::EventEmitter:
                {
                  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - set for emitter \n"));
                  this->connect_emitter (obj.in(),
                                        this->plan_.connection[j].internalEndpoint[0].portName.in(),
                                        providedReference[i].endpoint[0].in());
                  break;
                }
                case Deployment::EventPublisher:
                {
                  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - set for publisher \n"));
                  this->connect_publisher (obj.in(),
                                          this->plan_.connection[j].internalEndpoint[0].portName.in(),
                                          providedReference[i].endpoint[0].in());
                  break;
                }
                default:
                {
                  DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - currect Connection.InternalEndPoint.Kind "
                               "is not a Deployment::SimplexReceptacle, Deployment::EventEmitter, Deployment::EventPublisher "
                               "(Connection:%s Kind:%i PortName:%s)\n", 
                               this->plan_.connection[j].name.in(), 
                               this->plan_.connection[j].internalEndpoint[0].kind, 
                               this->plan_.connection[j].internalEndpoint[0].portName.in()
                              ));
                  throw ::Deployment::InvalidConnection();
                }//default
                }//switch
            }//if(name.compare(providedReference[i].name.in()) == 0)
        }//for ( unsigned int i = 0; i < providedReference.length(); ++i )
    }//for ( unsigned int j = 0; j < this->plan_.connection.length(); ++j )


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
                   "connect SimplexReceptacle for %s started\n", port_name.c_str()));
      res = inst->connect (port_name.c_str(), facet);
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - connect finished\n"));
    }
  catch (::Components::InvalidName& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect() returned ::Components::InvalidName exception\n"));
      throw ::Deployment::StartError();
    }
  catch (::Components::InvalidConnection& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect() returned ::Components::InvalidConnection exception\n"));
      throw ::Deployment::InvalidConnection();
    }
  catch (::Components::AlreadyConnected& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect() returned ::Components::AlreadyConnected exception\n"));
      throw ::Deployment::InvalidConnection();
    }
  catch (::Components::ExceededConnectionLimit& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect() returned ::Components::ExceededConnectionLimit exception\n"));
      throw ::Deployment::InvalidConnection();
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
                   "connect SimplexReceptacle for %s started\n", port_name.c_str()));
      res = inst->connect (port_name.c_str(), facet);
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - connect finished\n"));
    }
  catch (::Components::InvalidName& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect() returned ::Components::InvalidName exception\n"));
      throw ::Deployment::StartError();
    }
  catch (::Components::InvalidConnection& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect() returned ::Components::InvalidConnection exception\n"));
      throw ::Deployment::InvalidConnection();
    }
  catch (::Components::AlreadyConnected& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect() returned ::Components::AlreadyConnected exception\n"));
      //throw ::Deployment::InvalidConnection();
    }
  catch (::Components::ExceededConnectionLimit& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect() returned ::Components::ExceededConnectionLimit exception\n"));
      throw ::Deployment::InvalidConnection();
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
                   "connect_consumer for %s started\n", port_name.c_str()));
      inst->connect_consumer (port_name.c_str(), event);
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - "
                   "connect_consumer finished\n"));
    }
  catch (::Components::InvalidName& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect_consumer() returned ::Components::InvalidName exception\n"));
      throw ::Deployment::StartError();
    }
  catch (::Components::AlreadyConnected& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect_consumer() returned ::Components::AlreadyConnected exception\n"));
      throw ::Deployment::InvalidConnection();
    }
  catch (::Components::InvalidConnection& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect_consumer() returned ::Components::InvalidConnection exception\n"));
      throw ::Deployment::InvalidConnection();
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
                   "connect_consumer for %s started\n", port_name.c_str()));
      inst->connect_consumer (port_name.c_str(), event);
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - connect_consumer finished\n"));
    }
  catch (::Components::AlreadyConnected& )
    {
      DANCE_DEBUG ( (LM_WARNING, "NodeApplication_impl::finishLaunch - "
                     "Components::CCMObject_var::connect_consumer() returned ::Components::AlreadyConnected exception\n"));
      //throw ::Deployment::InvalidConnection();
    }
  catch (::Components::InvalidConnection& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::connect_consumer() returned ::Components::InvalidConnection exception\n"));
      throw ::Deployment::InvalidConnection();
    }
}

Components::Cookie*
NodeApplication_Impl::connect_publisher (Components::CCMObject_ptr inst,
                                        const ACE_CString& port_name,
                                        CORBA::Object_ptr consumer)
{
  Components::Cookie* res = 0;
  Components::EventConsumerBase_var event = Components::EventConsumerBase::_unchecked_narrow (consumer);
  try
    {
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - subscribe for %s started\n", port_name.c_str()));
      res = inst->subscribe (port_name.c_str(), event);
      DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplication_impl::finishLaunch - subscribe finished\n"));
    }
  catch (::Components::InvalidName& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::subscribe() returned ::Components::InvalidName exception\n"));
      throw ::Deployment::StartError();
    }
  catch (::Components::InvalidConnection& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::subscribe() returned ::Components::InvalidConnection exception\n"));
      throw ::Deployment::InvalidConnection();
    }
  catch (::Components::ExceededConnectionLimit& )
    {
      DANCE_ERROR((LM_ERROR, DLINFO " NodeApplication_impl::finishLaunch - "
                   "Components::CCMObject_var::subscribe() returned ::Components::ExceededCOnnectionLimit exception\n"));
      throw ::Deployment::InvalidConnection();
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

