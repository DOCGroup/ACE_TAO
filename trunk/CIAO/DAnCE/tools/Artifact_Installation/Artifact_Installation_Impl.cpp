// $Id$

#include "Artifact_Installation_Impl.h"
#include "DAnCE/Logger/Log_Macros.h"

#if !defined (__ACE_INLINE__)
#include "Artifact_Installation_Impl.inl"
#endif

namespace DAnCE
  {
    /*
     *  ArtifactRegistry
     */

    ArtifactRegistry::ArtifactRegistry ()
    {
    }

    ArtifactRegistry::ArtifactRegistry (const ArtifactRegistry& ar)
    {
      *this = ar;
    }

    ArtifactRegistry::~ArtifactRegistry ()
    {
    }

    /*
     *  ArtifactInstallation_Impl
     */

    ArtifactInstallation_Impl::THandlerMap ArtifactInstallation_Impl::handlers_;
    ArtifactInstallation_Impl::TLOCK ArtifactInstallation_Impl::handler_lock_;

    ArtifactInstallation_Impl::ArtifactInstallation_Impl ()
        : POA_DAnCE::ArtifactInstallation ()
    {
    }

    ArtifactInstallation_Impl::~ArtifactInstallation_Impl ()
    {
    }

    void ArtifactInstallation_Impl::initialize ()
    {
      ACE_GUARD_REACTION (TLOCK,
                          handler_guard_,
                          handler_lock_,
                          throw Deployment::PlanError (
                            "ArtifactInstallation_Impl::initialize",
                            "ArtifactInstallation handler lock failed"));

      THandlerMap::iterator it = handlers_.end ();
      for (it = handlers_.begin ();
           it != handlers_.end ();
           ++it)
        {
          (*it).second->initialize ();
        }
    }

    void ArtifactInstallation_Impl::clear ()
    {
      ACE_GUARD_REACTION (TLOCK,
                          handler_guard_,
                          handler_lock_,
                          throw Deployment::PlanError (
                            "ArtifactInstallation_Impl::clear",
                            "ArtifactInstallation handler lock failed"));

      THandlerMap::iterator it = handlers_.end ();
      for (it = handlers_.begin ();
           it != handlers_.end ();
           ++it)
        {
          (*it).second->clear ();
        }
    }

    void ArtifactInstallation_Impl::install (
      const char * plan_uuid,
      const ::Deployment::ArtifactDeploymentDescription & artifact)
    {
      ACE_GUARD_REACTION (TLOCK,
                          handler_guard_,
                          handler_lock_,
                          throw Deployment::PlanError (
                            plan_uuid,
                            "ArtifactInstallation handler lock failed"));

      std::string name = artifact.name.in ();

      // extract artifact properties
      ArtifactInstallationHandler::TPropertyMap properties;
      for (CORBA::ULong i=0; i < artifact.execParameter.length () ;++i)
        {
          Deployment::Property property = artifact.execParameter[i];
          char* prop_val;
          if (property.value >>= CORBA::Any::to_string (prop_val, 0))
            {
              properties[property.name.in ()] = prop_val;
            }
          else
            {
              DANCE_DEBUG (1, (LM_ERROR,  DLINFO ACE_TEXT("ArtifactInstallation_Impl::install - ")
                               ACE_TEXT ("failed to extract property %C for plan %C, artifact %C; property will be ignored\n"),
                               property.name.in (), plan_uuid, name.c_str ()));
            }
        }

      // attempt to install the artifact going through the alternative locations
      CORBA::ULong loc_n = 0;
      while (loc_n < artifact.location.length ())
        {
          try
            {
              std::string location = artifact.location[loc_n].in ();

              DANCE_DEBUG (6, (LM_TRACE,  DLINFO ACE_TEXT("ArtifactInstallation_Impl::install - ")
                               ACE_TEXT ("installation request for plan %C, artifact %C @ location[%u] %C\n"),
                               plan_uuid, name.c_str (), loc_n, location.c_str ()));

              // parse protocol stack & path; URI = <protocol>:[<protocol>:[...]]//<path>
              TProtocolStack prot_stack;
              this->parse_uri (plan_uuid, location, prot_stack);

              // if no protocols specified no further action necessary
              if (!prot_stack.empty ())
                {
                  // process protocol handlers
                  this->install_i (plan_uuid,
                                   name,
                                   prot_stack,
                                   location,
                                   properties);
                }

              DANCE_DEBUG (6, (LM_TRACE,  DLINFO ACE_TEXT("ArtifactInstallation_Impl::install - ")
                               ACE_TEXT ("installed plan %C, artifact %C @ location %C\n"),
                               plan_uuid, name.c_str (), location.c_str ()));

              return; // artifact successfully installed
            }
          catch (Deployment::PlanError& ex)
            {
              DANCE_DEBUG (3, (LM_ERROR,  DLINFO ACE_TEXT("ArtifactInstallation_Impl::install - ")
                               ACE_TEXT ("installation failed for plan %C, artifact %C @ location[%u] %C : %C\n"),
                               plan_uuid, name.c_str (), loc_n, artifact.location[loc_n].in (),
                               ex.reason.in ()));

              // artifact installation failed for this location; clean up anything left behind
              this->remove (plan_uuid, name.c_str ());

              // if we have alternates we continue otherwise we fail finally
              if (++loc_n >= artifact.location.length ())
                {
                  throw; // rethrow exception
                }

              // try alternate
            }
        }
    }

    void ArtifactInstallation_Impl::parse_uri (const char* plan_uuid,
        std::string& location,
        TProtocolStack& protstack)
    {
      DANCE_DEBUG (9, (LM_TRACE,  DLINFO ACE_TEXT("ArtifactInstallation_Impl::parse_uri - ")
                       ACE_TEXT ("parsing location %C for plan %C\n"),
                       location.c_str (), plan_uuid));

      std::string loctmp = location;
      std::string::size_type p;
      while ((p = loctmp.find (':')) != std::string::npos)
        {
          if (p == 0)
            {
              std::string err ("malformed location ");
              err += location;
              throw Deployment::PlanError (plan_uuid,
                                           err.c_str ());
            }

          std::string prot = loctmp.substr (0, p);
          protstack.push (prot);

          DANCE_DEBUG (9, (LM_TRACE,  DLINFO ACE_TEXT("ArtifactInstallation_Impl::parse_uri - ")
                           ACE_TEXT ("parsed protocol %C from location %C for plan %C\n"),
                           prot.c_str (), location.c_str (), plan_uuid));

          loctmp = loctmp.substr (p+1);
        }

      // did we find any protocol specifier?
      if (!protstack.empty ())
        {
          // check for path separator
          p = loctmp.find ("//");
          if (p != 0)
            {
              std::string err ("malformed location ");
              err += location;
              throw Deployment::PlanError (plan_uuid,
                                           err.c_str ());
            }

          // remove path separator
          location = loctmp.substr (2);
        }
    }

    void ArtifactInstallation_Impl::install_i (const std::string& plan_uuid,
        const std::string& name,
        TProtocolStack& protstack,
        std::string& location,
        const TPropertyMap& properties)
    {
      // process protocol handlers from last to first
      while (!protstack.empty ())
        {
          std::string protocol = protstack.top ();

          // find protocol handler
          THandlerMap::iterator ith = handlers_.find (protocol);
          if (ith == handlers_.end ())
            {
              DANCE_DEBUG (1, (LM_ERROR,  DLINFO ACE_TEXT("ArtifactInstallation_Impl::install_i - ")
                               ACE_TEXT ("unknown protocol %C for artifact %C from plan %C\n"),
                               protocol.c_str (), name.c_str (), plan_uuid.c_str ()));

              std::string err ("unknown installation protocol ");
              err += protocol;
              throw Deployment::PlanError (plan_uuid.c_str (),
                                           err.c_str ());
            }

          DANCE_DEBUG (9, (LM_TRACE,  DLINFO ACE_TEXT("ArtifactInstallation_Impl::install_i - ")
                           ACE_TEXT ("handling protocol %C for artifact %C @ location %C from plan %C\n"),
                           protocol.c_str (), name.c_str (), location.c_str (), plan_uuid.c_str ()));

          // activate protocol installation handler
          // after successfull installation the protocol handler will update
          // the location to the installed version
          ith->second->install (plan_uuid, location, properties);

          DANCE_DEBUG (9, (LM_TRACE,  DLINFO ACE_TEXT("ArtifactInstallation_Impl::install_i - ")
                           ACE_TEXT ("protocol %C installed artifact %C @ location %C from plan %C to location %C\n"),
                           protocol.c_str (), name.c_str (), location.c_str (), plan_uuid.c_str (), location.c_str ()));

          // register installed version for later removal
          this->register_version (plan_uuid, name, protocol, location);

          // remove handled protocol
          protstack.pop ();
        }

      ArtifactInstallationProperties property_reader (properties);
      const std::string& propval = property_reader.get_property (
        ArtifactInstallationProperties::PROP_KEEP_INTERMEDIATES);
      if (propval != "yes" && propval != "true")
        {
          this->remove_intermediates (plan_uuid, name);
        }
    }

    void ArtifactInstallation_Impl::register_version (const std::string& plan_uuid,
        const std::string& name,
        const std::string& protocol,
        const std::string& location)
    {
      ACE_GUARD_REACTION (TLOCK,
                          artifact_guard_,
                          artifact_lock_,
                          throw Deployment::PlanError (
                            plan_uuid.c_str (),
                            "ArtifactInstallation artifact lock failed"));

      this->artifacts_[plan_uuid][name].versions ().push_back (
          ArtifactRegistry::Version(protocol,location));
    }

    void ArtifactInstallation_Impl::remove (
      const char * plan_uuid,
      const char * artifact_name)
    {
      ACE_GUARD_REACTION (TLOCK,
                          handler_guard_,
                          handler_lock_,
                          throw Deployment::PlanError (
                            plan_uuid,
                            "ArtifactInstallation handler lock failed"));

      DANCE_DEBUG (6, (LM_TRACE,  DLINFO ACE_TEXT("ArtifactInstallation_Impl::remove - ")
                       ACE_TEXT ("removing versions for artifact %C from plan %C\n"),
                       artifact_name, plan_uuid));

      ArtifactRegistry artifact_reg;
      this->remove_artifact_registry (plan_uuid, artifact_name, artifact_reg);

      this->remove_i (plan_uuid, artifact_name, artifact_reg);
    }

    void ArtifactInstallation_Impl::remove_intermediates (
        const std::string& plan_uuid,
        const std::string& artifact_name)
      {
        ACE_GUARD_REACTION (TLOCK,
                            handler_guard_,
                            handler_lock_,
                            throw Deployment::PlanError (
                              plan_uuid.c_str (),
                              "ArtifactInstallation handler lock failed"));

        DANCE_DEBUG (6, (LM_TRACE,  DLINFO ACE_TEXT("ArtifactInstallation_Impl::remove - ")
                        ACE_TEXT ("removing intermediate versions for artifact %C from plan %C\n"),
                        artifact_name.c_str (), plan_uuid.c_str ()));

        ArtifactRegistry& artifact_reg = this->get_artifact_registry (plan_uuid, artifact_name);
        ArtifactRegistry::TVersions& versions = artifact_reg.versions ();
        while (versions.size () > 1)
          {
            // find protocol handler
            THandlerMap::iterator ith = handlers_.find (versions.front ().protocol_);
            if (ith == handlers_.end ())
              {
                DANCE_DEBUG (1, (LM_ERROR,  DLINFO ACE_TEXT("ArtifactInstallation_Impl::remove_i - ")
                                ACE_TEXT ("cannot find protocol %C to remove version ")
                                ACE_TEXT ("@ location %C for artifact %C from plan %C\n"),
                                versions.front ().protocol_.c_str (), versions.front ().location_.c_str (),
                                artifact_name.c_str (), plan_uuid.c_str ()));
              }
            else
              {
                DANCE_DEBUG (9, (LM_TRACE,  DLINFO ACE_TEXT("ArtifactInstallation_Impl::remove_i - ")
                                ACE_TEXT ("removing version of protocol %C @ %C for artifact %C from plan %C\n"),
                                versions.front ().protocol_.c_str (), versions.front ().location_.c_str (),
                                artifact_name.c_str (), plan_uuid.c_str ()));

                ith->second->remove (plan_uuid, versions.front ().location_);
              }
            versions.erase (versions.begin ());
          }
      }

    void ArtifactInstallation_Impl::remove_i (const char * plan_uuid,
        const char * artifact_name,
        ArtifactRegistry& artifact_reg)
    {
      for (ArtifactRegistry::TVersions::iterator iti =
             artifact_reg.versions ().begin ();
           iti != artifact_reg.versions ().end ();
           ++iti)
        {
          // find protocol handler
          THandlerMap::iterator ith = handlers_.find (iti->protocol_);
          if (ith == handlers_.end ())
            {
              DANCE_DEBUG (1, (LM_ERROR,  DLINFO ACE_TEXT("ArtifactInstallation_Impl::remove_i - ")
                               ACE_TEXT ("cannot find protocol %C to remove version ")
                               ACE_TEXT ("@ location %C for artifact %C from plan %C\n"),
                               iti->protocol_.c_str (), iti->location_.c_str (),
                               artifact_name, plan_uuid));
            }
          else
            {
              DANCE_DEBUG (9, (LM_TRACE,  DLINFO ACE_TEXT("ArtifactInstallation_Impl::remove_i - ")
                               ACE_TEXT ("removing version of protocol %C @ %C for artifact %C from plan %C\n"),
                               iti->protocol_.c_str (), iti->location_.c_str (),
                               artifact_name, plan_uuid));

              ith->second->remove (plan_uuid, iti->location_);
            }
        }
    }

    void ArtifactInstallation_Impl::remove_all (const char * plan_uuid)
    {
      ACE_GUARD_REACTION (TLOCK,
                          handler_guard_,
                          handler_lock_,
                          throw Deployment::PlanError (
                            plan_uuid,
                            "ArtifactInstallation handler lock failed"));

      DANCE_DEBUG (6, (LM_TRACE,  DLINFO ACE_TEXT("ArtifactInstallation_Impl::remove_all - ")
                       ACE_TEXT ("removing versions for plan %C\n"),
                       plan_uuid));

      TArtifactsMap artifacts_map;
      this->remove_artifacts_map (plan_uuid, artifacts_map);

      for (TArtifactsMap::iterator ita = artifacts_map.begin ();
           ita != artifacts_map.end ();
           ++ita)
        {
          this->remove_i (plan_uuid, ita->first.c_str (), ita->second);
        }
    }

    void ArtifactInstallation_Impl::remove_artifact_registry (
      const std::string& plan_uuid,
      const std::string& name,
      ArtifactRegistry& artifact_reg)
    {
      ACE_GUARD_REACTION (TLOCK,
                          artifact_guard_,
                          artifact_lock_,
                          throw Deployment::PlanError (
                            plan_uuid.c_str (),
                            "ArtifactInstallation artifact lock failed"));

      TArtifactsRegistry::iterator it_reg = this->artifacts_.find (plan_uuid);
      if (it_reg != this->artifacts_.end ())
        {
          TArtifactsMap::iterator it_art = it_reg->second.find (name);
          if (it_art != it_reg->second.end ())
            {
              artifact_reg = it_art->second;
              it_reg->second.erase (it_art);
            }
        }
    }

    ArtifactRegistry& ArtifactInstallation_Impl::get_artifact_registry (
      const std::string& plan_uuid,
      const std::string& name)
    {
      TArtifactsRegistry::iterator it_reg = this->artifacts_.find (plan_uuid);
      if (it_reg != this->artifacts_.end ())
        {
          TArtifactsMap::iterator it_art = it_reg->second.find (name);
          if (it_art != it_reg->second.end ())
            {
              return it_art->second;
            }
        }
      throw Deployment::PlanError (plan_uuid.c_str (),
                                   "corrupt artifact registry");
    }

    void ArtifactInstallation_Impl::remove_artifacts_map (
      const std::string& plan_uuid,
      TArtifactsMap& artifacts_map)
    {
      ACE_GUARD_REACTION (TLOCK,
                          artifact_guard_,
                          artifact_lock_,
                          throw Deployment::PlanError (
                            plan_uuid.c_str (),
                            "ArtifactInstallation artifact lock failed"));

      TArtifactsRegistry::iterator it_reg = this->artifacts_.find (plan_uuid);
      if (it_reg != this->artifacts_.end ())
        {
          artifacts_map = it_reg->second;
          this->artifacts_.erase (it_reg);
        }
    }

    char * ArtifactInstallation_Impl::get_artifact_location (
      const char * uuid,
      const char * artifact_name)
    {
      ACE_GUARD_RETURN (TLOCK, artifact_guard_, artifact_lock_, 0);

      TArtifactsRegistry::iterator it_reg = this->artifacts_.find (uuid);
      if (it_reg != this->artifacts_.end ())
        {
          TArtifactsMap::iterator it_art = it_reg->second.find (artifact_name);
          if (it_art != it_reg->second.end ())
            {
              return CORBA::string_dup (it_art->second.location ().c_str ());
            }
        }
      return 0;
    }

    int ArtifactInstallation_Impl::register_handler (ArtifactInstallationHandler* aih)
    {
      if (aih != 0)
        {
          ACE_GUARD_RETURN (TLOCK, guard_, handler_lock_, -1);

          DANCE_DEBUG (6, (LM_TRACE,  DLINFO ACE_TEXT("ArtifactInstallation_Impl::register_handler - ")
                           ACE_TEXT("%C:// handler\n"),
                           aih->protocol_prefix ().c_str ()));
          ArtifactInstallation_Impl::handlers_[aih->protocol_prefix ()] = aih;
        }

      return 0;
    }

    int ArtifactInstallation_Impl::deregister_handler (ArtifactInstallationHandler* aih)
    {
      if (aih != 0)
        {
          ACE_GUARD_RETURN (TLOCK, guard_, handler_lock_, -1);

          THandlerMap::iterator it_aih =
            ArtifactInstallation_Impl::handlers_.find (aih->protocol_prefix ());
          if (it_aih->second == aih)
            {
              DANCE_DEBUG (6, (LM_TRACE,  DLINFO ACE_TEXT("ArtifactInstallation_Impl::deregister_handler - ")
                               ACE_TEXT("%C:// handler\n"),
                               aih->protocol_prefix ().c_str ()));
              ArtifactInstallation_Impl::handlers_.erase (it_aih);
            }
        }

      return 0;
    }
  };
