// $Id$

#include "Client_init.h"
#include <ccm/CCM_ComponentC.h>
#include <ccm/CCM_StandardConfiguratorC.h>
#include "CIAO_common.h"
#include "Valuetype_Factories/ConfigValue.h"

int
CIAO::Client_init (CORBA::ORB_ptr o)
{
  CIAO_REGISTER_VALUE_FACTORY (o, Components::Cookie_init,
                               Components::Cookie);
  CIAO_REGISTER_VALUE_FACTORY (o, Components::PortDescription_init,
                               Components::PortDescription);
  CIAO_REGISTER_VALUE_FACTORY (o, Components::FacetDescription_init,
                               Components::FacetDescription);
  CIAO_REGISTER_VALUE_FACTORY (o, Components::ConnectionDescription_init,
                               Components::ConnectionDescription);
  CIAO_REGISTER_VALUE_FACTORY (o, Components::ReceptacleDescription_init,
                               Components::ReceptacleDescription);
  CIAO_REGISTER_VALUE_FACTORY (o, Components::ConsumerDescription_init,
                               Components::ConsumerDescription);
  CIAO_REGISTER_VALUE_FACTORY (o, Components::EmitterDescription_init,
                               Components::EmitterDescription);
  CIAO_REGISTER_VALUE_FACTORY (o, Components::SubscriberDescription_init,
                               Components::SubscriberDescription);
  CIAO_REGISTER_VALUE_FACTORY (o, Components::PublisherDescription_init,
                               Components::PublisherDescription);
  CIAO_REGISTER_VALUE_FACTORY (o, Components::ConfigValue_init,
                               Components::ConfigValue);
  CIAO_REGISTER_VALUE_FACTORY (o, Components::ComponentPortDescription_init,
                               Components::ComponentPortDescription);
  return 0;
}

namespace CIAO
{
  namespace Utility
  {
        void build_config_values_map (CONFIGVALUE_MAP &map, 
				  const ::Components::ConfigValues &config)
  {
    CIAO_TRACE("CIAO::build_config_values_map");
    map.unbind_all ();
    
    for (CORBA::ULong i = 0; i < config.length (); ++i)
      {
	int retval = map.rebind (config[i]->name (), config[i]->value ());
	
	if (retval == 1)
	  {
	    CIAO_ERROR ((LM_WARNING, CLINFO "build_config_values_map: Duplicate value for %s encountered, "
			"old value overwritten.\n",
			config[i]->name ()));
	  }
	else if (retval == -1)
	  {
	    CIAO_ERROR ((LM_WARNING, CLINFO "build_config_values_map: Error binding value for %s, ignoring.\n",
			config[i]->name ()));
	  }
	CIAO_DEBUG ((LM_TRACE, CLINFO
		    "build_config_values_map: Bound value for config value %s\n", 
		    config[i]->name ()));
      }
  }
    
    void build_config_values_sequence (::Components::ConfigValues &config, 
				       const CONFIGVALUE_MAP &map)
    {
      CIAO_TRACE ("CIAO::build_config_values_sequence");
      
      config.length (map.current_size ());
      
      CORBA::ULong pos = 0;
      
      for (CONFIGVALUE_MAP::const_iterator i = map.begin ();
	   (i.advance ()) != 0; ++pos)
	{
	  Components::ConfigValue_var newval;
	  ACE_NEW_THROW_EX (newval, 
			    ConfigValue_impl (i->ext_id_.c_str (), i->int_id_), 
			    CORBA::NO_MEMORY ());
	  config[pos] =  newval._retn ();
	}
    }
    
    /*
    void print_config_values (const ::Components::ConfigValues &config,
                              ACE_Log_Priority prio,
                              const char * prefix)
    {
      if (config.length () > 0)
        for (CORBA::ULong i = 0; i < config.length (); ++i)
          {
            ACE_CString
            CIAO_DEBUG ((prio, pfx + 
        
        
    }
    */

  }
}

