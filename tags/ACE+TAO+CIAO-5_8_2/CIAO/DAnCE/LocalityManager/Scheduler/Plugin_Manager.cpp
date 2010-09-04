// $Id$

#include "Plugin_Manager.h"
#include "Logger/Log_Macros.h"
#include "Deployment/Deployment_PlanErrorC.h"
#include "DAnCE/DAnCE_LocalityManagerC.h"
#include "DAnCE/DAnCE_DeploymentInterceptorsC.h"

namespace DAnCE
{
  namespace
  {
    template <typename PLUGIN>
    typename PLUGIN::_ptr_type
    load_plugin (const ACE_TCHAR *artifact,
                 const ACE_TCHAR *entrypoint)
    {
      if (!artifact || !entrypoint)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                           ACE_TEXT ("Plugin_Manager::load_plugin - ")
                           ACE_TEXT ("Must provide non-nill artifact and entrypoint names\n")));
          throw ::Deployment::PlanError ("",
                                         "Invalid parameters for plug-in installation");
        }
    
      DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                       ACE_TEXT ("Plugin_Manager::load_plugin - ")
                       ACE_TEXT ("Loading plugin from <%s>:<%s>\n"),
                       artifact,
                       entrypoint));
                     
      ACE_DLL plugin_dll;
    
      if (plugin_dll.open (artifact,
                           ACE_DEFAULT_SHLIB_MODE,
                           false) != 0)
        {
          const ACE_TCHAR *error = plugin_dll.error ();
        
          DANCE_ERROR (1, (LM_ERROR, DLINFO 
                           ACE_TEXT ("Plugin_Manager::load_plugin - ")
                           ACE_TEXT ("Error while loading artifact <%s>: %s\n"),
                           artifact,
                           error));
        
          throw ::Deployment::PlanError (ACE_TEXT_ALWAYS_CHAR (artifact),
                                         ACE_TEXT_ALWAYS_CHAR (error));
        }
    
      DANCE_DEBUG (9, (LM_TRACE, DLINFO
                       ACE_TEXT ("Plugin_Manager::load_plugin - ")
                       ACE_TEXT ("Loading artifact <%s> successfully loaded.\n"),
                       artifact));

      void *void_ptr = plugin_dll.symbol (entrypoint);
      ptrdiff_t tmp_ptr = reinterpret_cast <ptrdiff_t> (void_ptr);

      typedef typename PLUGIN::_ptr_type (*PluginFactory) (void);
      PluginFactory pcreator = reinterpret_cast<PluginFactory> (tmp_ptr);

      if (!pcreator)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                           ACE_TEXT ("Plugin_Manager::load_plugin - ")
                           ACE_TEXT ("Unable to load plugin <%s>:<%s>\n"),
                           artifact,
                           entrypoint));
          throw ::Deployment::PlanError (ACE_TEXT_ALWAYS_CHAR (artifact),
                                         "Invalid entrypoint");
        }
    
      typename PLUGIN::_var_type plugin = pcreator ();
    
      if (CORBA::is_nil (plugin))
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                           ACE_TEXT ("Plugin_Manager::load_plugin - ")
                           ACE_TEXT ("Unable to load plugin <%s>:<%s>, nil result from factory\n"),
                           artifact,
                           entrypoint));
          throw ::Deployment::PlanError (ACE_TEXT_ALWAYS_CHAR (artifact),
                                         "Nil result from factory");
        }
    
      DANCE_DEBUG (9, (LM_TRACE, DLINFO
                       ACE_TEXT ("Plugin_Manager::load_plugin - ")
                       ACE_TEXT ("Successfully created plugin from <%s>:<%s>.\n"),
                       artifact,
                       entrypoint));

    
      return plugin._retn ();
    }
  }
  
  Plugin_Manager::Plugin_Manager (void)
  {
  }

  void
  Plugin_Manager::set_orb (CORBA::ORB_ptr orb)
  {
    this->orb_ = CORBA::ORB::_duplicate (orb);
  }
  
  CORBA::ORB_ptr 
  Plugin_Manager::get_orb (void)
  {
    return CORBA::ORB::_duplicate (this->orb_);
  }
  
  template<typename T>
  struct Closer
  {
    void operator() (T& item)
    {
      item.second->close ();
    }
  };

  Plugin_Manager::~Plugin_Manager (void)
  {
    try
      {
        std::for_each (handler_map_.begin (),
                       handler_map_.end (),
                       Closer<HANDLER_MAP::value_type> ());
        
        for (INTERCEPTORS::iterator i = this->interceptors_.begin ();
             i != this->interceptors_.end ();
             ++i)
          *i = ::DAnCE::DeploymentInterceptor::_nil ();
      }
    catch (...)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("Plugin_Manager::~Plugin_Manager - ")
                         ACE_TEXT ("Caught unknown C++ exception while closing plugin manager\n")));
      }
  }
  
  void
  Plugin_Manager::set_configuration (const Deployment::Properties &config)
  {
    this->config_ = config;
  }

  char *
  Plugin_Manager::register_installation_handler (const ACE_TCHAR *artifact,
                                                 const ACE_TCHAR *entrypoint,
                                                 const Plugin_Manager::IH_DEPS &depends)
  {
    ::DAnCE::InstanceDeploymentHandler_var plugin = 
      load_plugin< ::DAnCE::InstanceDeploymentHandler > (artifact,
                                                         entrypoint);
    
    try
      {
        plugin->configure (this->config_);

        CORBA::String_var instance_type = plugin->instance_type ();
        
        DANCE_DEBUG (6, (LM_INFO, DLINFO
                         ACE_TEXT ("Plugin_Manager::register_installation_handler - ")
                         ACE_TEXT ("Successfully created installation handler for instance type <%C>\n"),
                         instance_type.in ()));
        
        this->ih_dep_.add_dependency (instance_type.in (), depends);
        
        ::CORBA::StringSeq_var deps = plugin->dependencies ();
        
        if (!CORBA::is_nil (deps))
          {
            for (CORBA::ULong i = 0; i < deps->length (); ++i)
              {
                this->ih_dep_.add_dependency (instance_type.in (), 
                                              deps[i].in ());
              }
          }

        this->handler_map_[instance_type.in ()] = plugin._retn ();
        
        return instance_type._retn ();
      }
    catch (const CORBA::Exception &ex)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("Plugin_Manager::register_installation_handler - ")
                         ACE_TEXT ("CORBA Exception caught while loading artifact <%s>:<%s> - %C\n"),
                         artifact,
                         entrypoint,
                         ex._info ().c_str ()));
        throw ::Deployment::PlanError (ACE_TEXT_ALWAYS_CHAR (artifact),
                                       ex._info ().c_str ());
      }
    catch (...)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("Plugin_Manager::register_installation_handler - ")
                         ACE_TEXT ("Unknown C++ exception while configuring plugin from <%s>:<%s>\n"),
                         artifact,
                         entrypoint));
        throw ::Deployment::PlanError (ACE_TEXT_ALWAYS_CHAR (artifact),
                                       "Unknown C++ exception during handler configuration\n");
      }
  }
  
  void
  Plugin_Manager::get_installation_order (Plugin_Manager::INSTALL_ORDER &io)
  {
    this->ih_dep_.calculate_order (io);
  }

  void 
  Plugin_Manager::register_interceptor (const ACE_TCHAR *artifact,
                                        const ACE_TCHAR *entrypoint)
  {
    
    ::DAnCE::DeploymentInterceptor_var plugin = 
      load_plugin< ::DAnCE::DeploymentInterceptor > (artifact,
                                                     entrypoint);
    try
      {
        plugin->configure (this->config_);
        
        DANCE_DEBUG (6, (LM_INFO, DLINFO
                         ACE_TEXT ("Plugin_Manager::register_interceptor - ")
                         ACE_TEXT ("Successfully created deployment interceptor\n")));
        
        this->interceptors_.push_back (plugin._retn ());
      }
    catch (const CORBA::Exception &ex)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("Plugin_Manager::register_interceptor - ")
                         ACE_TEXT ("CORBA Exception caught while loading artifact <%s>:<%s> - %C\n"),
                         artifact,
                         entrypoint,
                         ex._info ().c_str ()));
        throw ::Deployment::PlanError (ACE_TEXT_ALWAYS_CHAR (artifact),
                                        ex._info ().c_str ());
      }
    catch (...)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("Plugin_Manager::register_interceptor - ")
                         ACE_TEXT ("Unknown C++ exception while configuring plugin from <%s>:<%s>\n"),
                         artifact,
                         entrypoint));
        throw ::Deployment::PlanError (ACE_TEXT_ALWAYS_CHAR (artifact),
                                        "Unknown C++ exception during handler configuration\n");
      }
  }

  ::DAnCE::InstanceDeploymentHandler_ptr 
  Plugin_Manager::fetch_installation_handler (const char *instance_type)
  {
    if (instance_type == 0)
      {
        return 0;
      }
    HANDLER_MAP::iterator i;

    if ((i = this->handler_map_.find (instance_type)) ==
        this->handler_map_.end ())
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("Plugin_Manager::fetch_installation_handler - ")
                         ACE_TEXT ("No installation handler for type %C found\n"),
                         instance_type));
        
        return 0;
      }
    
    return ::DAnCE::InstanceDeploymentHandler::_duplicate (i->second);
  }
  
  const 
  Plugin_Manager::INTERCEPTORS & 
  Plugin_Manager::fetch_interceptors (void)
  {
    return this->interceptors_;
  }

  void 
  Plugin_Manager::register_configuration_plugin (const ACE_TCHAR *artifact,
                                                 const ACE_TCHAR *entrypoint)
  {
    DAnCE::LocalityConfiguration_var plugin = 
      load_plugin< DAnCE::LocalityConfiguration > (artifact,
                                                   entrypoint);
    
    try
      {
        CORBA::String_var id = plugin->type ();
        
        this->config_plugins_[id.in ()] = plugin._retn ();
      }
    catch (const CORBA::Exception &ex)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("Plugin_Manager::register_configuration_plugin - ")
                         ACE_TEXT ("CORBA Exception caught while loading artifact <%s>:<%s> - %C\n"),
                         artifact,
                         entrypoint,
                         ex._info ().c_str ()));
        throw ::Deployment::PlanError (ACE_TEXT_ALWAYS_CHAR (artifact),
                                        ex._info ().c_str ());
      }
    catch (...)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("Plugin_Manager::register_configuration_plugin - ")
                         ACE_TEXT ("Unknown C++ exception while configuring plugin from <%s>:<%s>\n"),
                         artifact,
                         entrypoint));
        throw ::Deployment::PlanError (ACE_TEXT_ALWAYS_CHAR (artifact),
                                        "Unknown C++ exception during plugin configuration\n");
      }
  }
  
  ::DAnCE::LocalityConfiguration_ptr
  Plugin_Manager::get_configuration_handler (const char *id)
  {
    if (id == 0)
      {
        return 0;
      }
    
    CONFIG_MAP::iterator i;

    if ((i = this->config_plugins_.find (id)) ==
        this->config_plugins_.end ())
      {
        DANCE_ERROR (9, (LM_TRACE, DLINFO
                         ACE_TEXT ("Plugin_Manager::get_configuration_handler - ")
                         ACE_TEXT ("No configuration plugin for type %C found\n"),
                         id));
        
        return 0;
      }
    
    return ::DAnCE::LocalityConfiguration::_duplicate (i->second);
  }

}
