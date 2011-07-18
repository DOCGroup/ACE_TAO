// $Id$

#include "RTNodeApp_Configurator.h"
#include "DAnCE/Deployment/CIAO_ServerResourcesC.h"
#include "tao/RTPortableServer/RTPortableServer.h"

CIAO::RTNodeApp_Configurator::~RTNodeApp_Configurator ()
{
}

int
CIAO::RTNodeApp_Configurator::pre_orb_initialize ()
{
  CIAO_DEBUG ((LM_DEBUG, "RTNodeApp_Configurator::pre_orb_init\n"));

  return 0;
}

int
CIAO::RTNodeApp_Configurator::post_orb_initialize (CORBA::ORB_ptr o)
{
  CIAO_DEBUG ((LM_DEBUG, "RTNodeApp_Configurator::post_orb_init\n"));

  this->orb_ = CORBA::ORB::_duplicate (o);

  CORBA::Object_var object =
    this->orb_->resolve_initial_references ("RTORB");

  this->rtorb_ =
    RTCORBA::RTORB::_narrow (object.in ());

  this->config_manager_.init (this->rtorb_.in ());

  return 0;
}

int
CIAO::RTNodeApp_Configurator::init_resource_manager
(const ::Deployment::Properties &properties)
{

  //  if (CIAO::debug_level () > 9)
    CIAO_DEBUG ((LM_DEBUG, "RTNodeApp_Configurator::init_resource_manager\n"));
  // @@ Should we cache the properties that we consumed here?

  for (CORBA::ULong i = 0; i < properties.length (); ++i)
    {
      //      if (CIAO::debug_level () > 9)
      CIAO_DEBUG ((LM_DEBUG, "RTNodeApp_Configurator::init_resource_manager processing property: %C\n",
                  properties[i].name.in ()));

      if (ACE_OS::strcmp ("CIAOServerResources", properties[i].name.in ()) == 0)
        {
          const CIAO::DAnCE::ServerResource *svr_resource;
          if (properties[i].value >>= svr_resource)
            {
              this->config_manager_.init_resources (*svr_resource);
              //  Now we have the information to initialize the manager.
            }
          else
            CIAO_ERROR_RETURN ((LM_ERROR,
                               "ERROR: RTNodeApp_Configurator::init_resource_manager unable to extract CIAOServerResources\n"), -1);
        }

      // Ignore other stuff
    }

  return 0;
}

CORBA::PolicyList *
CIAO::RTNodeApp_Configurator::find_container_policies
(const ::Deployment::Properties &properties)
{
  CIAO_DEBUG ((LM_DEBUG, "RTNodeApp_Configurator::find_container_policies\n"));
  // @@ Should we cache the properties that we consumed here?

  for (CORBA::ULong i = 0; i < properties.length (); ++i)
    {
      CIAO_DEBUG ((LM_DEBUG, "RTNodeApp_Configurator::find_container_policies processing property: %C\n",
                  properties[i].name.in ()));

      if (ACE_OS::strcmp ("ContainerPolicySet", properties[i].name) == 0)
        {
          const char *policy_name = 0;
          if (properties[i].value >>= policy_name)
            {
              return this->config_manager_.find_policies_by_name (policy_name);
            }
          else
            CIAO_ERROR_RETURN ((LM_ERROR,
                               "ERROR: RTNodeApp_Configurator::find_container_policies unable to extract ContainerPolicySet\n"), 0);
        }
    }

  return 0;
}


extern "C" CIAO_RTNA_Configurator_Export CIAO::NodeApp_Configurator *create_nodeapp_configurator (void);

CIAO::NodeApp_Configurator *
create_nodeapp_configurator (void)
{
  CIAO::RTNodeApp_Configurator *config = 0;
  ACE_NEW_RETURN (config, CIAO::RTNodeApp_Configurator, 0);
  return config;
}
