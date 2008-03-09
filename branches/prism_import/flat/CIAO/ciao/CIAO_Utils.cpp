#include "CIAO_Utils.h"
#include "CIAO_Common.h"

#include "ConfigValue.h"

namespace CIAO
{
  void build_config_values_map (CONFIGVALUE_MAP &map, const ::Components::ConfigValues &config)
  {
    CIAO_TRACE("CIAO::build_config_values_map");
    map.unbind_all ();

    for (CORBA::ULong i = 0; i < config.length (); ++i)
      {
	int retval = map.rebind (config.name (), config.value ());
	
	if (retval == 1)
	  {
	    ACE_ERROR ((LM_WARNING, CLINFO "build_config_values_map: Duplicate value for %s encountered, "
			"old value overwritten.\n",
			config.name ()));
	  }
	else if (retval == -1)
	  {
	    ACE_ERROR ((LM_WARNING, CLINFO "build_config_values_map: Error binding value for %s, ignoring.\n",
			config.name ()));
	  }
      }
  }
  
  void build_config_values_sequence (::Components::ConfigValues &config, const CONFIGVALUE_MAP &map)
  {
    CIAO_TRACE ("CIAO::build_config_values_sequence");
    
    config.length (map.current_size ());

    CORBA::ULong pos = 0;

    for (CONFIGVALUE_MAP::const_iterator i = map.begin ();
	 (i = i.advance ()) != 0; ++pos)
      {
	config[pos] = ConfigValue_impl (i->ext_id_.c_str (), i->int_id_);
      }
  }

}
