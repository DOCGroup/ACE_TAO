// $Id$

#include "RTServer_Impl.h"

#if !defined (__ACE_INLINE__)
# include "RTServer_Impl.inl"
#endif /* __ACE_INLINE__ */

CIAO::RTServer::RTComponentServer_Impl::~RTComponentServer_Impl ()
{
}

int
CIAO::RTServer::RTComponentServer_Impl::init (Components::ConfigValues &options
                                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ Initialize ComponentServer and create the internal container
  // implementation that actually interacts with installed
  // homes/components.

  ACE_NEW_THROW_EX (this->config_,
                    ::Components::ConfigValues (),
                    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (-1);

  *this->config_ = options;

  this->parse_server_config_values (options
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // We will probably need two ORBs in this process.  One for the
  // deployment framework, and one for the actual components.
  return 0;
}

Components::Deployment::Container_ptr
CIAO::RTServer::RTComponentServer_Impl::create_container
(const Components::ConfigValues & config
 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CreateFailure,
                   Components::InvalidConfiguration))
{
  ACE_DEBUG ((LM_DEBUG, "RTComponentServer_Impl::create_container\n"));

  CIAO::RTServer::RTContainer_Impl *container_servant = 0;

  ACE_NEW_THROW_EX (container_servant,
                    CIAO::RTServer::RTContainer_Impl (this->orb_.in (),
                                                      this->poa_.in (),
                                                      this->get_objref (),
                                                      this->policy_set_manager_),
                    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  PortableServer::ServantBase_var safe_servant (container_servant);
  container_servant->init (config,
                           this->get_component_installation ()
                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  PortableServer::ObjectId_var oid
    = this->poa_->activate_object (container_servant
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::Object_var obj
    = this->poa_->id_to_reference (oid.in ()
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  Components::Deployment::Container_var ci
    = Components::Deployment::Container::_narrow (obj.in ()
                                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Cached the objref in its servant.
  container_servant->set_objref (ci.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);

    this->container_set_.add (ci.in ());
  }

  return ci._retn ();
}

void
CIAO::RTServer::RTComponentServer_Impl::
parse_server_config_values (const Components::ConfigValues &options
                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidConfiguration))
{
  ACE_DEBUG ((LM_DEBUG, "RTComponentServer_Impl::parse_server_config_values\n"));

  for (CORBA::ULong i = 0; i < options.length (); ++i)
    {
      ACE_DEBUG ((LM_DEBUG, "parse_server_options: %s\n", options[i]->name ()));

      if (ACE_OS::strcmp (options[i]->name (), "CIAO-RTResources") == 0)
        {
          CIAO::RTConfiguration::RTORB_Resource_Info *resource_info;
          if (options[i]->value () >>= resource_info)
            {
              // initialize the resource manager with resource defintions.
              this->resource_manager_.init (*resource_info
                                            ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;
            }
        }
      else if (ACE_OS::strcmp (options[i]->name (), "CIAO-RTPolicySets") == 0)
        {
          CIAO::RTConfiguration::Policy_Sets * policy_sets;
          if (options[i]->value () >>= policy_sets)
            {
              // initialize the policyset manager with policy set defintions.
              this->policy_set_manager_.init (*policy_sets
                                              ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;
            }
        }
      else
        {
          Components::InvalidConfiguration exc;
          exc.name = CORBA::string_dup (options[i]->name ());
          exc.reason = Components::UnknownConfigValueName;
          ACE_THROW (exc);
        }
    }
}

// ============================================================

CIAO::RTServer::RTContainer_Impl::~RTContainer_Impl ()
{
}

int
CIAO::RTServer::RTContainer_Impl::init (const Components::ConfigValues &options,
                                        Components::Deployment::ComponentInstallation_ptr inst
                                        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "RTContainer_Impl::init\n"));

  this->config_ = options;
  this->parse_container_config_values (options
                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->installation_ =
    Components::Deployment::ComponentInstallation::_duplicate (inst);

  // @@ Initialize container and create the internal container
  // implementation that actually interacts with installed
  // homes/components.

  // @@ We will need a container factory here later on when we support
  // more kinds of container implementations.

  // @@ Fish out the ComponentServer object reference from <options>.

  ACE_NEW_THROW_EX (this->container_,
                    CIAO::Session_Container (this->orb_.in ()),
                    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (-1);

  return this->container_->init (0,
                                 this->Policies_.ptr ()
                                 ACE_ENV_ARG_PARAMETER);
}

void
CIAO::RTServer::RTContainer_Impl::
parse_container_config_values (const Components::ConfigValues &options
                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidConfiguration))
{
  ACE_DEBUG ((LM_DEBUG, "parse_container_config_values"));

  for (CORBA::ULong i = 0; i < options.length (); ++i)
    {
      ACE_DEBUG ((LM_DEBUG, "options.name= %s\n", options[i]->name ()));

      if (ACE_OS::strcmp (options[i]->name (), "CIAO-RTPolicySet") == 0)
        {
          char *ps_name;
          if (options[i]->value () >>= ps_name)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "-Using RTPolicySet named: %s\n",
                          ps_name));
              CORBA::PolicyList_var policies =
              // initialize the policyset manager with policy set defintions.
                this->policyset_manager_.find_policies_by_name (ps_name
                                                                ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;

              this->Policies_ = policies;
              ACE_DEBUG ((LM_DEBUG,
                          "Found RTPolicySet named: %s\n",
                          ps_name));
            }
        }
      else
        {
          Components::InvalidConfiguration exc;
          exc.name = CORBA::string_dup (options[i]->name ());
          exc.reason = Components::UnknownConfigValueName;
          ACE_THROW (exc);
        }
    }
}
