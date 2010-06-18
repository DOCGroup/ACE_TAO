// $Id$
#include "CIAO_Container_Impl.h"

#include "ace/Log_Msg.h"
#include "ccm/CCM_HomeC.h"
#include "CIAO_PropertiesC.h"
#include "CIAO_CS_ClientC.h"
#include "Client_init.h"
#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  namespace Deployment
  {
    // Constructor
    CIAO_Container_i::CIAO_Container_i (const Components::ConfigValues &config,
                                        const Static_Config_EntryPoints_Maps *static_entrypts,
                                        const char *name,
                                        const CORBA::PolicyList *policies,
                                        Components::Deployment::ComponentInstallation_ptr ci,
                                        CORBA::ORB_ptr orb,
                                        PortableServer::POA_ptr poa)
      : orb_ (CORBA::ORB::_duplicate (orb)),
        poa_ (PortableServer::POA::_duplicate (poa)),
        config_ (config.length ()),
        static_entrypts_maps_ (static_entrypts),
        ci_ (Components::Deployment::ComponentInstallation::_duplicate (ci))
    {
      CIAO_TRACE("CIAO_Container_i::CIAO_Container_i");

      for (CORBA::ULong i = 0; i < config.length (); ++i)
        {
          this->config_[i] = config[i];
        }

      try
        {
          if (this->static_entrypts_maps_ == 0)
            {
              CIAO_DEBUG (6, (LM_DEBUG, CLINFO "CIAO_Container_i: creating Session container with dynamic linkage\n"));
              this->container_ = new CIAO::Session_Container (this->orb_.in (), this->poa_.in (), this, false,
                                                              0, name, policies);
            }
          else
            {
              CIAO_DEBUG (6, (LM_DEBUG, CLINFO "CIAO_Container_i: creating Session container with static linkage\n"));
              this->container_ = new CIAO::Session_Container (this->orb_.in (), this->poa_.in (), this, true,
                                                              this->static_entrypts_maps_,
                                                              name, policies);
            }
        }
      catch (...)
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO "CIAO_Container_i::CIAO_Container_i - "
                      "Caught exception while allocating container implementation\n"));
          throw;
        }
    }


    // Destructor
    CIAO_Container_i::~CIAO_Container_i (void)
    {
      CIAO_TRACE("CIAO_Container_i::~CIAO_Container_i");
    }

    ::Components::CCMObject_ptr
    CIAO_Container_i::install_component (const char * id,
                                         const char * entrypt,
                                         const ::Components::ConfigValues & config)
    {
      CIAO_TRACE("CIAO_Container_i::install_component");

      if (!id)
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO "CIAO_Container_i::install_component - "
                       "No home ID provided\n"));
          throw ::Components::Deployment::InvalidConfiguration ();
        }

      Components::CCMObject_var comp;

      if (this->component_map_.find (id, comp) == 0)
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO "CIAO_Container_i::install_component - "
                       "Component with id %C already installed, aborting\n",
                       id));
          throw Components::CreateFailure ();
        }

      if (!entrypt)
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO "CIAO_Container_i::install_component - "
                      "No executor entrypoint found.\n"));
          throw ::Components::Deployment::InvalidConfiguration ();
        }

      CIAO_DEBUG (6, (LM_INFO, CLINFO "CIAO_Container_i::install_component - "
                   "Attempting to install home with id [%C]\n",
                   id));

      CIAO_DEBUG (9, (LM_TRACE, CLINFO
                  "CIAO_Container_i::install_component - "
                  "Extracting ConfigValues from sequence of length [%u]\n",
                  config.length ()));

      CIAO::Utility::CONFIGVALUE_MAP cm;
      CIAO::Utility::build_config_values_map (cm, config);
      CORBA::Any val;

      const char *tmp = 0;
      CORBA::String_var exec_art, svnt_art, svnt_entry;

      if (cm.find (SVNT_ENTRYPT, val) == 0)
        {
          val >>= tmp;
          svnt_entry = tmp;
          CIAO_DEBUG (9, (LM_TRACE, CLINFO
                       "CIAO_Container_i::install_component - "
                       "Found Servant entrypoint %C\n", svnt_entry.in ()));
        }
      else
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO
                       "CIAO_Container_i::install_component - "
                       "Error: No Servant entrypoint provided, aborting installation\n"));
          throw Components::InvalidConfiguration ();
        }

      if (cm.find (SVNT_ARTIFACT, val) == 0)
        {
          val >>= tmp;
          svnt_art = this->ci_->get_implementation (tmp);
          CIAO_DEBUG (9, (LM_TRACE, CLINFO
                       "CIAO_Container_i::install_component - "
                       "Found Servant artifact %C\n", svnt_art.in ()));
        }
      else
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO
                       "CIAO_Container_i::install_component - "
                       "Error: No Servant artifact provided, aborting installation\n"));
          throw Components::InvalidConfiguration ();
        }


      if (cm.find (EXEC_ARTIFACT, val) == 0)
        {
          val >>= tmp;
          exec_art = this->ci_->get_implementation (tmp);
          CIAO_DEBUG (9, (LM_TRACE, CLINFO
                       "CIAO_Container_i::install_component - "
                       "Found executor artifact: %C\n", exec_art.in ()));
        }
      else
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO
                       "CIAO_Container_i::install_component - "
                       "Error: No Executor artifact provided, aborting installation\n"));
        }

      CIAO_DEBUG (9, (LM_TRACE, CLINFO
                  "CIAO_Container_i::install_component - "
                  "Extraction resulted in map of [%u] values\n", cm.current_size ()));

      comp = this->container_->install_component (exec_art,
                                                  entrypt,
                                                  svnt_art,
                                                  svnt_entry,
                                                  id);

      if (this->component_map_.bind (id,
                                     Components::CCMObject::_duplicate (comp.in ())) == -1)
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO
                       "CIAO_Container_i::install_component - "
                       "Unable to bind componnet into component map\n"));
        }

      return comp._retn ();
    }

    void
    CIAO_Container_i::remove_component (::Components::CCMObject_ptr cref)
    {
      CIAO_TRACE("CIAO_Container_i::remove_component");

      Component_Iterator i = this->component_map_.begin ();
      while (!i.done ())
        {
          if (i->item ()->_is_equivalent (cref))
            {
              CIAO_DEBUG (9, (LM_TRACE, CLINFO "CIAO_Container_i::remove_component - "
                           "Successfully found matching component\n"));
              break;
            }
          i.advance ();
        }

      if (i.done ())
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO "CIAO_Container_i::remove_component - "
                       "Unable to find matching component managed by this container, throwing RemoveFailure\n"));
          throw Components::RemoveFailure ();
        }

      if (!CORBA::is_nil (this->container_.in ()))
        {
          CIAO_DEBUG (9, (LM_TRACE, CLINFO "CIAO_Container_i::remove_component - "
                       "Invoking remove on the container impl for component %C.\n",
                       i->key ().c_str ()));
          this->container_->uninstall_component (cref);
          CIAO_DEBUG (6, (LM_INFO, CLINFO "CIAO_Container_i::remove_component - "
                       "Successfully removed component %C\n",
                       i->key ().c_str ()));
        }

      if (this->component_map_.unbind (i->key ()) != 0)
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO "CIAO_Container_i::remove_component - "
                       "Unable to unbind removed component with id %C from component map\n",
                       i->key ().c_str ()));
        }
    }

    ::Components::CCMObjectSeq *
    CIAO_Container_i::get_components (void)
    {
      CIAO_TRACE("CIAO_Container_i::get_components");
      throw CORBA::NO_IMPLEMENT ();
    }

    ::Components::ConfigValues *
    CIAO_Container_i::configuration (void)
    {
      CIAO_TRACE("CIAO_Container_i::configuration");
      throw CORBA::NO_IMPLEMENT ();
    }

    ::Components::Deployment::ComponentServer_ptr
    CIAO_Container_i::get_component_server (void)
    {
      CIAO_TRACE("CIAO_Container_i::get_component_server");
      throw CORBA::NO_IMPLEMENT ();
    }

    ::Components::CCMHome_ptr
    CIAO_Container_i::install_home (const char * id,
                                    const char * entrypt,
                                    const ::Components::ConfigValues & config)
    {
      CIAO_TRACE("CIAO_Container_i::install_home");

      if (!id)
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO "CIAO_Container_i::install_home - "
                      "No home ID provided\n"));
          throw ::Components::Deployment::InvalidConfiguration ();
        }

      Components::CCMHome_var home;

      if (this->home_map_.find (id, home) == 0)
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO "CIAO_Container_i::install_home - "
                       "Home with id %C already installed, aborting\n",
                       id));
          throw Components::CreateFailure ();
        }

      if (!entrypt)
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO "CIAO_Container_i::install_home - "
                      "No executor entrypoint found.\n"));
          throw ::Components::Deployment::InvalidConfiguration ();
        }

      CIAO_DEBUG (6, (LM_INFO, CLINFO "CIAO_Container_i::install_home - "
      "Attempting to install home with id [%C]\n", id));

      CIAO_DEBUG (9, (LM_TRACE, CLINFO
                  "CIAO_Container_i::install_home - "
                  "Extracting ConfigValues from sequence of length [%u]\n",
                  config.length ()));

      CIAO::Utility::CONFIGVALUE_MAP cm;
      CIAO::Utility::build_config_values_map (cm, config);
      CORBA::Any val;

      const char *tmp = 0;
      CORBA::String_var exec_art, svnt_art, svnt_entry;
      if (cm.find (SVNT_ENTRYPT, val) == 0)
        {
          val >>= tmp;
          svnt_entry = tmp;
          CIAO_DEBUG (9, (LM_TRACE, CLINFO
                       "CIAO_Container_i::install_home - "
                       "Found Servant entrypoint %C\n", svnt_entry.in ()));
        }
      else
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO
                       "CIAO_Container_i::install_home - "
                       "Error: No Servant entrypoint provided, aborting installation\n"));
          throw Components::InvalidConfiguration ();
        }

      if (cm.find (SVNT_ARTIFACT, val) == 0)
        {
          val >>= tmp;
          svnt_art = this->ci_->get_implementation (tmp);
          CIAO_DEBUG (9, (LM_TRACE, CLINFO
                       "CIAO_Container_i::install_home - "
                       "Found Servant artifact %C\n", svnt_art.in ()));
        }
      else
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO
                       "CIAO_Container_i::install_home - "
                       "Error: No Servant artifact provided, aborting installation\n"));
          throw Components::InvalidConfiguration ();
        }


      if (cm.find (EXEC_ARTIFACT, val) == 0)
        {
          val >>= tmp;
          exec_art = this->ci_->get_implementation (tmp);
          CIAO_DEBUG (9, (LM_TRACE, CLINFO
                       "CIAO_Container_i::install_home - "
                       "Found executor artifact: %C\n", exec_art.in ()));
        }
      else
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO
                       "CIAO_Container_i::install_home - "
                       "Error: No Executor artifact provided, aborting installation\n"));
        }

      CIAO_DEBUG (9, (LM_TRACE, CLINFO
                  "CIAO_Container_i::install_home - "
                  "Extraction resulted in map of [%u] values\n", cm.current_size ()));


      // extract config values here...

      //CIAO_DEBUG (6, (LM_DEBUG, CLINFO "CIAO_Container_i::install_home - ",
      //"Executor entrypoint [%C], servant entrypoint [%C], servant library [%C]\n",
      //entrypt, svnt_entrypt.in (), svnt_library.in ()));

      home = this->container_->install_home (exec_art,
                                             entrypt,
                                             svnt_art,
                                             svnt_entry,
                                             id);

      if (this->home_map_.bind (id,
                                Components::CCMHome::_duplicate (home.in ())) == -1)
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO
                       "CIAO_Container_i::install_home - "
                       "Unable to bind home into home map\n"));
        }

      return  home._retn ();
    }

    void
    CIAO_Container_i::remove_home (::Components::CCMHome_ptr href)
    {
      CIAO_TRACE("CIAO_Container_i::remove_home");

      Home_Iterator i = this->home_map_.begin ();
      while (!i.done ())
        {
          if (i->item ()->_is_equivalent (href))
            {
              CIAO_DEBUG (9, (LM_TRACE, CLINFO "CIAO_Container_i::remove_home - "
                           "Successfully found matching home\n"));
              break;
            }
          i.advance ();
        }

      if (i.done ())
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO "CIAO_Container_i::remove_home - "
                       "Unable to find matching home managed by this container, throwing RemoveFailure\n"));
          throw Components::RemoveFailure ();
        }

      CIAO_DEBUG (9, (LM_TRACE, CLINFO "CIAO_Container_i::remove_home - "
                   "Invoking remove on the container impl for home %C.\n",
                   i->key ().c_str ()));
      this->container_->uninstall_home (href);
      CIAO_DEBUG (6, (LM_INFO, CLINFO "CIAO_Container_i::remove_home - "
                   "Successfully removed home %C\n",
                   i->key ().c_str ()));

      if (this->home_map_.unbind (i->key ()) != 0)
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO "CIAO_Container_i::remove_home - "
                       "Unable to unbind removed home with id %C from home map\n",
                       i->key ().c_str ()));
        }
    }

    ::Components::CCMHomes *
    CIAO_Container_i::get_homes (void)
    {
      CIAO_TRACE("CIAO_Container_i::get_homes");

      ::Components::CCMHomes *tmp_homes = 0;

      ACE_NEW_THROW_EX (tmp_homes,
                        ::Components::CCMHomes (this->home_map_.current_size ()),
                        CORBA::NO_MEMORY ());

      ::Components::CCMHomes_var retval (tmp_homes);
      retval->length (this->home_map_.current_size ());
      Home_Iterator i = this->home_map_.begin ();
      CORBA::ULong pos = 0;
      while (!i.done ())
        {
          retval[pos++] = ::Components::CCMHome::_duplicate (i->item ().in ());
          i.advance ();
        }

      return retval._retn ();
    }

    void
    CIAO_Container_i::remove (void)
    {
      CIAO_TRACE("CIAO_Container_i::remove");

      if (this->home_map_.current_size () != 0 ||
          this->component_map_.current_size () != 0)
        {
          CIAO_ERROR (1, (LM_WARNING, CLINFO "CIAO_Container_i::remove - "
                       "Attempting to remove container that still has %u homes and %u components installed\n",
                       this->home_map_.current_size (),
                       this->component_map_.current_size ()));
        }
    }

    void
    CIAO_Container_i::activate_component (::Components::CCMObject_ptr comp)
    {
      CIAO_TRACE ("CIAO_Container_i::activate_component");

      this->container_->activate_component (comp);
    }


    void
    CIAO_Container_i::passivate_component (::Components::CCMObject_ptr comp)
    {
      CIAO_TRACE ("CIAO_Container_i::passivate_component");

      this->container_->passivate_component (comp);
    }

    PortableServer::POA_ptr
    CIAO_Container_i::_default_POA (void)
    {
      CIAO_TRACE ("CIAO_Container_i::_default_POA");
      return PortableServer::POA::_duplicate (this->poa_.in ());
    }

    void CIAO_Container_i::connect_local_facet (::Components::CCMObject_ptr provider,
                                                const char * provider_port,
                                                ::Components::CCMObject_ptr user,
                                                const char * user_port)
    {
      CIAO_TRACE ("CIAO_Container_i::connect_local_facet");
      this->container_->connect_local_facet (provider, provider_port, user, user_port);

    }

    void CIAO_Container_i::disconnect_local_facet (::Components::CCMObject_ptr provider,
                                                   const char * provider_port,
                                                   ::Components::CCMObject_ptr user,
                                                   const char * user_port)
    {
      CIAO_TRACE ("CIAO_Container_i::connect_local_facet");
      this->container_->disconnect_local_facet (provider, provider_port, user, user_port);
    }
  }
}
