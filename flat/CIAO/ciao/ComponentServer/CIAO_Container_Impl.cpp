#include "CIAO_Container_Impl.h"

#include <ace/Log_Msg.h>
#include <ciao/CIAO_common.h>


namespace CIAO
{
  namespace Deployment
  {
    // Constructor 
    CIAO_Container_i::CIAO_Container_i (const Components::ConfigValues &config,
					const Static_Config_EntryPoints_Maps *static_entrypts,
					const char *name,
					const CORBA::PolicyList *policies,
					CORBA::ORB_ptr orb,
					PortableServer::POA_ptr poa)
      : orb_ (CORBA::ORB::_duplicate (orb)),
	poa_ (PortableServer::POA::_duplicate (poa)),
	config_ (config.length ()),
	static_entrypts_maps_ (static_entrypts)
    {
      CIAO_TRACE("CIAO_Container_i::CIAO_Container_i");
      
      for (CORBA::ULong i = 0; i < config.length (); ++i)
        {
          this->config_[i] = config[i];
        }
      
      if (this->static_entrypts_maps_ == 0)
	{
	  ACE_DEBUG((LM_DEBUG, CLINFO "CIAO_Container_i: creating Session container with dynamic linkage\n"));
	  this->container_.reset (new CIAO::Session_Container (this->orb_.in (), this, false,
							       0, name, policies));
	}
      else
	{
	  ACE_DEBUG((LM_DEBUG, CLINFO "CIAO_Container_i: creating Session container with static linkage\n"));
	  this->container_.reset (new CIAO::Session_Container (this->orb_.in (), this, true, 
							       this->static_entrypts_maps_,
							       name, policies));
	}
    }
    
  
    // Destructor 
    CIAO_Container_i::~CIAO_Container_i (void)
    {
      CIAO_TRACE("CIAO_Container_i::~CIAO_Container_i");
    }
    
    ::Components::CCMObject_ptr
    CIAO_Container_i::install_component (const char * /*id*/,
                                         const char * /*entrypt*/,
                                         const ::Components::ConfigValues & /*config*/)
    {
      CIAO_TRACE("CIAO_Container_i::install_component");
      return 0;
    }
    
    void 
    CIAO_Container_i::remove_component (::Components::CCMObject_ptr /*cref*/)
    {
      CIAO_TRACE("CIAO_Container_i::remove_component");
    }
    
    ::Components::Deployment::CIAO::CCMObjects * 
    CIAO_Container_i::get_components (void)
    {
      CIAO_TRACE("CIAO_Container_i::get_components");
      return 0;
    }
    
    ::Components::ConfigValues * 
    CIAO_Container_i::configuration (void)
    {
      CIAO_TRACE("CIAO_Container_i::configuration");
      return 0;
    }
    
    ::Components::Deployment::ComponentServer_ptr 
    CIAO_Container_i::get_component_server (void)
    {
      CIAO_TRACE("CIAO_Container_i::get_component_server");
      return 0;
    }
    
    ::Components::CCMHome_ptr 
    CIAO_Container_i::install_home (const char * /*id*/,
                                    const char * /*entrypt*/,
                                    const ::Components::ConfigValues & /*config*/)
    {
      CIAO_TRACE("CIAO_Container_i::install_home");
      return  0;
    }
    
    void 
    CIAO_Container_i::remove_home (::Components::CCMHome_ptr /*href*/)
    {
      CIAO_TRACE("CIAO_Container_i::remove_home");
    }
    
    ::Components::CCMHomes * 
    CIAO_Container_i::get_homes (void)
    {
      CIAO_TRACE("CIAO_Container_i::get_homes");
      return 0;
    }
    
    void 
    CIAO_Container_i::remove (void)
    {
      CIAO_TRACE("CIAO_Container_i::remove");
    }
    
    PortableServer::POA_ptr 
    CIAO_Container_i::_default_POA (void)
    {
      CIAO_TRACE ("CIAO_Container_i::_default_POA");
      return PortableServer::POA::_duplicate (this->poa_.in ());      
    }
  }
}
