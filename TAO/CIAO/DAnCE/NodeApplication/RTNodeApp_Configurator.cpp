// $Id$

#include "RTNodeApp_Configurator.h"
#include "tao/RTPortableServer/RTPortableServer.h"

int
CIAO::RTNodeApp_Configurator::pre_orb_initialize ()
{
  ACE_DEBUG ((LM_DEBUG, "RTNodeApp_Configurator::pre_orb_init\n"));

  return 0;
}

int
CIAO::RTNodeApp_Configurator::post_orb_initialize (CORBA::ORB_ptr o)
{
  ACE_DEBUG ((LM_DEBUG, "RTNodeApp_Configurator::post_orb_init\n"));

  this->orb_ = CORBA::ORB::_duplicate (o);

  CORBA::Object_var object =
    this->orb_->resolve_initial_references ("RTORB");

  this->rtorb_ =
    RTCORBA::RTORB::_narrow (object.in ());

  return 0;
}

int
CIAO::RTNodeApp_Configurator::init_resource_manager
(const ::Deployment::Properties &)
{
  return 0;
}

CORBA::PolicyList *
CIAO::RTNodeApp_Configurator::find_container_policies
(const ::Deployment::Properties &)
{

  return 0;
}


extern "C" CIAO_RTNA_Configurator_Export CIAO::NodeApp_Configurator *create_nodeapp_configurator (void);

CIAO::NodeApp_Configurator *
create_nodeapp_configurator (void)
{
  CIAO::RTNodeApp_Configurator *config;
  ACE_NEW_RETURN (config, CIAO::RTNodeApp_Configurator, 0);
  return config;
}

