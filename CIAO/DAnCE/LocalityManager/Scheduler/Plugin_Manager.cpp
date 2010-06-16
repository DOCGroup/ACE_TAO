// $Id$

#include "Plugin_Manager.h"
#include "Logger/Log_Macros.h"
#include "Deployment/Deployment_PlanErrorC.h"
#include "DAnCE/DAnCE_LocalityManagerC.h"
#include "DAnCE/DAnCE_DeploymentInterceptorsC.h"

namespace DAnCE
{
  Plugin_Manager::Plugin_Manager (void)
  {
  }

  Plugin_Manager::~Plugin_Manager (void)
  {
  }
  
  void
  Plugin_Manager::set_configuration (const Deployment::Properties &config)
  {
    this->config_ = config;
  }

  const char *
  Plugin_Manager::register_installation_handler (const ACE_TCHAR *artifact,
                                                 const ACE_TCHAR *entrypoint)
  {
    if (!artifact || !entrypoint)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("Plugin_Manager::register_installation_handler - ")
                         ACE_TEXT ("Must provide non-nill artifact and entrypoint names\n")));
        throw ::Deployment::PlanError ("",
                                       "Invalid parameters for plug-in installation");
      }
    
    DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                     ACE_TEXT ("Plugin_Manager::register_installation_handler - ")
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
                         ACE_TEXT ("Plugin_Manager::register_installation_handler - ")
                         ACE_TEXT ("Error while loading artifact <%s>: %s\n"),
                         artifact,
                         error));
        
        throw ::Deployment::PlanError (ACE_TEXT_ALWAYS_CHAR (artifact),
                                       ACE_TEXT_ALWAYS_CHAR (error));
      }
    
    DANCE_DEBUG (9, (LM_TRACE, DLINFO
                     ACE_TEXT ("Plugin_Manager::register_installation_handler - ")
                     ACE_TEXT ("Loading artifact <%s> successfully loaded.\n"),
                     artifact));

    void *void_ptr = plugin_dll.symbol (entrypoint);
    ptrdiff_t tmp_ptr = reinterpret_cast <ptrdiff_t> (void_ptr);

    typedef ::DAnCE::InstanceDeploymentHandler_ptr (*PluginFactory) (void);
    PluginFactory pcreator = reinterpret_cast<PluginFactory> (tmp_ptr);
    
    if (!pcreator)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("Plugin_Manager::register_installation_handler - ")
                         ACE_TEXT ("Unable to load plugin <%s>:<%s>\n"),
                         artifact,
                         entrypoint));
        throw ::Deployment::PlanError (ACE_TEXT_ALWAYS_CHAR (artifact),
                                       "Invalid entrypoint");
      }
    
    ::DAnCE::InstanceDeploymentHandler_var plugin = pcreator ();
    
    if (CORBA::is_nil (plugin))
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("Plugin_Manager::register_installation_handler - ")
                         ACE_TEXT ("Unable to load plugin <%s>:<%s>, nil result from factory\n"),
                         artifact,
                         entrypoint));
        throw ::Deployment::PlanError (ACE_TEXT_ALWAYS_CHAR (artifact),
                                       "Nil result from factory");
      }
    
    DANCE_DEBUG (9, (LM_TRACE, DLINFO
                     ACE_TEXT ("Plugin_Manager::register_installation_handler - ")
                     ACE_TEXT ("Successfully created installation handler from <%s>:<%s>.\n"),
                     artifact,
                     entrypoint));

    
    try
      {
        plugin->configure (this->config_);
        
        CORBA::String_var instance_type = plugin->instance_type ();
        
        this->handler_map_[instance_type.in ()] = plugin._retn ();
        
        DANCE_DEBUG (6, (LM_INFO, DLINFO
                         ACE_TEXT ("Plugin_Manager::register_installation_handler - ")
                         ACE_TEXT ("Successfully created installation handler for instance type <%C>\n"),
                         instance_type.in ()));
        
        return instance_type.in ();
      }
    catch (const CORBA::Exception &ex)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("Plugin_Manager::register_installation_handler - ")
                         ACE_TEXT ("CORBA Exception caught while loading artifact <%s>:<%s> - %C\n"),
                         artifact,
                         entrypoint,
                         ex._info ().c_str ()));
        throw ::Deployment::StartError (ACE_TEXT_ALWAYS_CHAR (artifact),
                                        ex._info ().c_str ());
      }
    catch (...)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("Plugin_Manager::register_installation_handler - ")
                         ACE_TEXT ("Unknown C++ exception while configuring plugin from <%s>:<%s>\n"),
                         artifact,
                         entrypoint));
        throw ::Deployment::StartError (ACE_TEXT_ALWAYS_CHAR (artifact),
                                        "Unknown C++ exception during handler configuration\n");
      }
  }
  
  void 
  Plugin_Manager::register_interceptor (const ACE_TCHAR *artifact,
                                        const ACE_TCHAR *entrypoint)
  {
    if (!artifact || !entrypoint)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("Plugin_Manager::register_interceptor - ")
                         ACE_TEXT ("Must provide non-nill artifact and entrypoint names\n")));
        throw ::Deployment::PlanError ("",
                                       "Invalid parameters for plug-in installation");
      }
    
    DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                     ACE_TEXT ("Plugin_Manager::register_interceptor - ")
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
                         ACE_TEXT ("Plugin_Manager::register_interceptor - ")
                         ACE_TEXT ("Error while loading artifact <%s>: %s\n"),
                         artifact,
                         error));
        
        throw ::Deployment::PlanError (ACE_TEXT_ALWAYS_CHAR (artifact),
                                       ACE_TEXT_ALWAYS_CHAR (error));
      }
    
    DANCE_DEBUG (9, (LM_TRACE, DLINFO
                     ACE_TEXT ("Plugin_Manager::register_interceptor - ")
                     ACE_TEXT ("Loading artifact <%s> successfully loaded.\n"),
                     artifact));

    void *void_ptr = plugin_dll.symbol (entrypoint);
    ptrdiff_t tmp_ptr = reinterpret_cast <ptrdiff_t> (void_ptr);

    typedef ::DAnCE::DeploymentInterceptor_ptr (*PluginFactory) (void);
    PluginFactory pcreator = reinterpret_cast<PluginFactory> (tmp_ptr);
    
    if (!pcreator)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("Plugin_Manager::register_interceptor - ")
                         ACE_TEXT ("Unable to load plugin <%s>:<%s>\n"),
                         artifact,
                         entrypoint));
        throw ::Deployment::PlanError (ACE_TEXT_ALWAYS_CHAR (artifact),
                                       "Invalid entrypoint");
      }
    
    ::DAnCE::DeploymentInterceptor_var plugin = pcreator ();
    
    if (CORBA::is_nil (plugin))
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("Plugin_Manager::register_interceptor - ")
                         ACE_TEXT ("Unable to load interceptor <%s>:<%s>, nil result from factory\n"),
                         artifact,
                         entrypoint));
        throw ::Deployment::PlanError (ACE_TEXT_ALWAYS_CHAR (artifact),
                                       "Nil result from factory");
      }
    
    DANCE_DEBUG (9, (LM_TRACE, DLINFO
                     ACE_TEXT ("Plugin_Manager::register_interceptor - ")
                     ACE_TEXT ("Successfully created deployment interceptor from <%s>:<%s>.\n"),
                     artifact,
                     entrypoint));

    
    try
      {
        plugin->configure (this->config_);
        
        ::DAnCE::InstanceInstallation_var install;
        if (!CORBA::is_nil (install = ::DAnCE::InstanceInstallation::_narrow (plugin)))
          {
            DANCE_DEBUG (6, (LM_INFO, DLINFO
                             ACE_TEXT ("Plugin_Manager::register_interceptor - ")
                             ACE_TEXT ("Successfully created installation interceptor\n")));

            this->inst_interceptors_.push_back (install._retn ());
            return;
          }
        
        
        ::DAnCE::InstanceConnection_var conn;
        if (!CORBA::is_nil (install = ::DAnCE::InstanceInstallation::_narrow (plugin)))
          {
            DANCE_DEBUG (6, (LM_INFO, DLINFO
                             ACE_TEXT ("Plugin_Manager::register_interceptor - ")
                             ACE_TEXT ("Successfully created connection interceptor\n")));
          }
        this->conn_interceptors_.push_back (conn._retn ());
        return;
    
        ::DAnCE::PlanPreprocessing_var pp;
        if (!CORBA::is_nil (install = ::DAnCE::InstanceInstallation::_narrow (plugin)))
          {
            DANCE_DEBUG (6, (LM_INFO, DLINFO
                             ACE_TEXT ("Plugin_Manager::register_interceptor - ")
                             ACE_TEXT ("Successfully created preprocessing interceptor\n")));
        
            this->pp_interceptors_.push_back (pp._retn ());
            return;
          }
      }
    catch (const CORBA::Exception &ex)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("Plugin_Manager::register_interceptor - ")
                         ACE_TEXT ("CORBA Exception caught while loading artifact <%s>:<%s> - %C\n"),
                         artifact,
                         entrypoint,
                         ex._info ().c_str ()));
        throw ::Deployment::StartError (ACE_TEXT_ALWAYS_CHAR (artifact),
                                        ex._info ().c_str ());
      }
    catch (...)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("Plugin_Manager::register_interceptor - ")
                         ACE_TEXT ("Unknown C++ exception while configuring plugin from <%s>:<%s>\n"),
                         artifact,
                         entrypoint));
        throw ::Deployment::StartError (ACE_TEXT_ALWAYS_CHAR (artifact),
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
  Plugin_Manager::INST_INTERCEPTORS & 
  Plugin_Manager::fetch_installation_interceptors (void)
  {
    return this->inst_interceptors_;
  }
      
  const 
  Plugin_Manager::CONN_INTERCEPTORS &
  Plugin_Manager::fetch_connection_interceptors (void)
  {
    return this->conn_interceptors_;
  }
  
  const 
  Plugin_Manager::PP_INTERCEPTORS &
  Plugin_Manager::fetch_pp_interceptors (void)
  {
    return this->pp_interceptors_;
  }
}
