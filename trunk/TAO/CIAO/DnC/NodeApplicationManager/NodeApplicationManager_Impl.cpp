// $Id$

#include "NodeApplicationManager_Impl.h"
#include "ace/Process.h"
#include "ace/Read_Buffer.h"
#include "ace/OS_NS_stdio.h"

#if !defined (__ACE_INLINE__)
# include "NodeApplicationManager_Impl.inl"
#endif /* __ACE_INLINE__ */

CIAO::NodeApplicationManager_Callback_Impl::
NodeApplicationManager_Callback_Impl (CORBA::ORB_ptr o,
				      PortableServer::POA_ptr p,
				      Deployment::NodeApplicationManager_ptr nam,
				      const Components::ConfigValues &config )
  : orb_ (CORBA::ORB::_duplicate (o)),
    poa_ (PortableServer::POA::_duplicate (p)),
    nam_ (nam)
{
  this->config_ = new Components::ConfigValues (config);
}

CIAO::NodeApplicationManager_Callback_Impl::~NodeApplicationManager_Callback_Impl ()
{
}

PortableServer::POA_ptr
CIAO::NodeApplicationManager_Callback_Impl::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

Deployment::NodeApplicationManager_ptr
CIAO::NodeApplicationManager_Callback_Impl::
register_node_application (Deployment::NodeApplication_ptr na,
			   Components::ConfigValues_out config
			   ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  config = this->config_._retn ();

  this->na_ = Deployment::NodeApplication::_duplicate (na);
  return Deployment::NodeApplicationManager::_duplicate (this->nam_);
}

Deployment::NodeApplication_ptr
CIAO::NodeApplicationManager_Callback_Impl::get_nodeapp_ref (void)
{
  // Relinquish the ownership of the server reference.  This method
  // should only be called from the CIAO_NodeApplicationManager.
  return this->na_._retn ();
}


//===============================================================
//========NodeApplicationManager_Impl============================
CIAO::NodeApplicationManager_Impl::~NodeApplicationManager_Impl ()
{
  // @@ remove all NodeApplication's?
}

PortableServer::POA_ptr
CIAO::NodeApplicationManager_Impl::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

int
CIAO::CIAO_NodeApplicationManager_Impl::
init (const char *nodeapp_location,
      CORBA::ULong spawn_delay,
      const char *installation_ior,
      const char *default_svcconf,
      const char *svc_conf_map
      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG (spawn_delay); // @@ We need to use this argumetn
                                // later to determine is a
                                // NodeApplication has been spawned
                                // successfully.

  if (nodeapp_location == 0)
    ACE_THROW_RETURN (CORBA::BAD_PARAM (), -1);

  if (spawn_delay_ == 0)
    ACE_THROW_RETURN (CORBA::BAD_PARAM (), -1);

  // Initialize svc.conf map
  if (this->init_svcconf_map (svc_conf_map) != 0)
    ACE_THROW_RETURN (CORBA::BAD_PARAM (), -1);

  if (default_svcconf != 0)
    this->default_svcconf_file_ = default_svcconf;

  this->nodeapp_path_ = CORBA::string_dup (nodeapp_location);

  this->installation_ior_ = CORBA::string_dup (installation_ior);

  PortableServer::POAManager_var mgr
    = this->poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CORBA::PolicyList policies (0);

  // Create a separate POA for callback objects.
  this->callback_poa_ =
    this->poa_->create_POA ("callback_poa",
                            mgr.in (),
                            policies
                            ACE_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN (-1);

  // Activate the CIAO_NodeApplicationManager Object.
  PortableServer::ObjectId_var oid
    = this->poa_->activate_object (this
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CORBA::Object_var obj = this->poa_->id_to_reference (oid.in ()
                                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // And cache the object reference.
  this->objref_ = NodeApplicationManager::_narrow (obj.in ()
						   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->ior_ = this->orb_->object_to_string (this->objref_.in ()
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
CIAO::NodeApplicationManager_Impl::init_svcconf_map (const char *filename)
{
  // It's fine if no svcconf map filename is provided.  User assumes
  // it's okay to use the default.
  if (filename == 0)
    return 0;

  FILE *inf = ACE_OS::fopen (filename, "r");

  if (inf != NULL)
    {
      ACE_Read_Buffer ior_buffer (inf);
      char *data = 0;

      while ((data = ior_buffer.read ('\n')) != 0)
        {
          ACE_CString aline (data);

          // Each entry line is a comma delimited (hint, pathname) tuple.
          ssize_t delpos = aline.find (',');

          // Skip over comments (lines that start with '#' or invalid entries
          // (lines without delimitor.)
          if (aline[0] == '#' || delpos == ACE_CString::npos)
            continue;

          ACE_CString hint = aline.substring (0, delpos);
          ACE_CString svcconf_name = aline.substring (delpos+1);

          switch (this->svc_conf_map_.bind (hint, svcconf_name))
            {
            case 0:
              // All is fine.
              // Debug info:
              ACE_DEBUG ((LM_DEBUG,
                          "Bound svc.conf hint \"%s\" successfully\n",
                          hint.c_str ()));
              break;

            case 1:
              ACE_DEBUG ((LM_DEBUG,
                          "Duplication svc.conf hint \"%s\" found - ignore\n",
                          hint.c_str ()));
              break;

            case -1:
            default:
              // Unrecoverable error!!
              ACE_ERROR_RETURN ((LM_DEBUG,
                                 "Fail to bind svc.conf hint \"%s\" - abort\n",
                                 hint.c_str ()),
                                -1);
            }
        }
    }
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Fail to open svcconf map data file: %s.\n",
                       filename),
                      -1);

  return 0;
}

const char *
CIAO::NodeApplicationManager_Impl::lookup_svcconf_pathname (const char *hint) const
{
  // Change this function to change the strategy of applying svc.conf files.

  if (hint == 0)
    {
      if (this->default_svcconf_file_.length () > 0)
        return this->default_svcconf_file_.c_str ();
      else
        return 0;
    }

  SVCCONF_MAP::ENTRY *entry;
  if (this->svc_conf_map_.find (hint, entry) == 0)
    return entry->int_id_.c_str ();
  ACE_ERROR_RETURN ((LM_ERROR,
                     "Unable to find matching svc.conf file from hint: %s\n",
                     hint),
                    0);
}

//@@  I assume that this functions will never be called in the current scheme.
//    These functions are maintained as place holders only for future
//    RT CIAO specific configurations.
//@@ we should use the InvalidProperty exception!! Change needed! --Tao
void
CIAO::NodeApplicationManager_Impl::
parse_config_value (const Components::ConfigValues &options,
		    NodeApp_Config_Info &config_info
		    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   Deployment::InvalidProperty))
{
  for (CORBA::ULong l = 0; l < options.length (); ++l)
    {
      if (ACE_OS::strcmp (options[l]->name (), "CIAO-svcconf-id") == 0)
        {
          const char *str_in = 0;
          if (options[l]->value () >>= str_in)
              config_info.svcconf_hint_ = CORBA::string_dup (str_in);
          ACE_CHECK;
        }
      else if (ACE_OS::strcmp (options[l]->name (), "CIAO-rtcad-filename") == 0)
        {
          const char *str_in = 0;
          if (options[l]->value () >>= str_in)
              config_info.rtcad_filename_ = CORBA::string_dup (str_in);
          ACE_CHECK;
        }
      else
        {
          Components::InvalidConfiguration *exc = 0;

          ACE_NEW_THROW_EX (exc,
                            Components::InvalidConfiguration,
                            CORBA::NO_MEMORY ());

          exc->name = CORBA::string_dup (options[l]->name ());
          exc->reason = Components::InvalidConfigValueType;
#if defined (ACE_HAS_EXCEPTIONS)

          auto_ptr<Deployment::InvalidProperty> safety (exc);

          // Direct throw because we don't have the ACE_TRY_ENV.
          exc->_raise ();
#else
          // We can not use ACE_THROW here.
          ACE_TRY_ENV.exception (exc);
#endif
        }
    }
}


Deployment::NodeApplication_ptr
CIAO::NodeApplicationManager_Impl::
create_node_application (const Components::ConfigValues & config
			 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Deployment::StartError,
                   Deployment::InvalidProperty))
{
  NodeApp_Config_Info config_info;
  this->parse_config_value (config,
                            config_info
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  ACE_DEBUG ((LM_DEBUG, "CIAO::NodeApplicationManager_Impl::create_node_application\n"));

  //@@ all configuration is subject to future fix! --Tao
  Deployment::NodeApplication_var retval;

  // @@ Need to figure out a way to pass the config data over to the
  // NodeApplication.  One option will be to convert it (again) into
  // an XML document and psss it along in the command line.

  // @@ Create a new callback servant.
  CIAO::NodeApplicationManager_Callback_Impl *callback_servant = 0;
  ACE_NEW_THROW_EX (callback_servant,
                    CIAO::NodeApplicationManager_Callback_Impl (this->orb_.in (),
								this->callback_poa_.in (),
								this->objref_.in (),
								config),
                    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  PortableServer::ServantBase_var servant_var (callback_servant);
  PortableServer::ObjectId_var cb_id
    = this->callback_poa_->activate_object (callback_servant
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  ACE_Process node_application;
  ACE_Process_Options options;

  ACE_TRY
    {
      CORBA::Object_var cb_obj =
        this->callback_poa_->id_to_reference (cb_id.in ()
                                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CIAO::NodeApplicationManager_Callback_var cb =
        CIAO::NodeApplicationManager_Callback::_narrow (cb_obj.in ()
							ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var cb_ior =
        this->orb_->object_to_string (cb.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      const char *svcconf_path =
        this->lookup_svcconf_pathname (config_info.svcconf_hint_.in ());

      // spawn the new NodeApplication.
      ACE_CString additional_options;

      if (svcconf_path != 0)
        {
          ACE_DEBUG((LM_DEBUG, "Using svcconf file: %s\n",
                     svcconf_path));
          additional_options += ACE_CString (" -ORBSvcConf ");
          additional_options += ACE_CString (svcconf_path);
        }

      if (config_info.rtcad_filename_.in () != 0)
        {
          ACE_DEBUG((LM_DEBUG, "Using RTCAD file: %s\n",
                     config_info.rtcad_filename_.in ()));
          additional_options += ACE_CString (" -r ");
          additional_options += ACE_CString (config_info.rtcad_filename_.in ());
        }

      options.command_line ("%s -k %s -ORBInitRef ComponentInstallation=%s "
                            "%s",
                            this->nodeapp_path_.in (),
                            cb_ior.in (),
                            this->installation_ior_.in (),
                            additional_options.c_str ());

      options.avoid_zombies (1);

      if (node_application.spawn (options) == -1)
        {
          ACE_DEBUG ((LM_ERROR, "Fail to spawn a NodeApplication process\n"));
          ACE_TRY_THROW (Components::CreateFailure ());
        }

      // wait for server to pass back its object reference. with a timeout value.
      // using perform_work and stuff.

      int looping = 1;

      ACE_Time_Value timeout (this->spawn_delay_, 0);

      while (looping)
        {
          this->orb_->perform_work (timeout
                                    ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          retval = callback_servant->get_nodeapp_ref ();

          if (timeout == ACE_Time_Value::zero || !CORBA::is_nil (retval.in ()))
            looping = 0;
        }

      if (CORBA::is_nil (retval.in ()))
        {
          ACE_DEBUG ((LM_ERROR, "Fail to acquire the NodeApplication object\n"));
          ACE_TRY_THROW (Components::CreateFailure ());
        }

      {
        ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);

        // Add the server reference to this->cs_set_;
        this->cs_set_.add (retval.in ());
      }
    }
  ACE_CATCHANY
    {
      this->callback_poa_->deactivate_object (cb_id.in ());
      ACE_TRY_CHECK;

      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);

  this->callback_poa_->deactivate_object (cb_id.in ());
  ACE_CHECK_RETURN (0);

  return retval._retn ();
}

virtual Deployment::Application_ptr
CIAO::NodeApplicationManager_Impl::
startLaunch (const Deployment::Properties & configProperty,
	     Deployment::Connections_out providedReference,
	     CORBA::Boolean start
	     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException
		   , Deployment::ResourceNotAvailable
		   , Deployment::StartError
		   , Deployment::InvalidProperty))
{
  /**
   *  1. First Map properties to TAO/CIAO specific property/configurations
   *  2. Necessary property checking (needed?)
   *  3. Call create_nade_application to spawn new process.
   *  4. Initialize the NodeApplication.
   *  5. get the provided connection endpoints back and return them.
   */
  ACE_TRY
    {

    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;

  ACE_CHECK_RETURN (0);

  return;

}


virtual void
CIAO::NodeApplicationManager_Impl::
destroyApplication (Deployment::Application_ptr app
		    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException
		   , Deployment::StopError))
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

  if (this->cs_set_.object_in_set (app) == 0)
    ACE_THROW (Deployment::StopError ());

  app->remove (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Should we remove the server still even if the previous call failed.

  if (this->cs_set_.remove (app) == -1)
    ACE_THROW (Deployment::StopError ());
}


/*
Deployment::NodeApplications *
CIAO::NodeApplicationManager_Impl::get_node_servers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);

  Components::Deployment::ComponentServers_var retval;

  ACE_NEW_THROW_EX (retval.out (),
                    Components::Deployment::ComponentServers (),
                    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  CORBA::ULong len = this->cs_set_.size ();
  retval->length (len);          // resize

#if 0
  // TAO is broken here.  Both <replace>, <get_buffer> and friends are missing.
  this->cs_set_.copy (len, retval->get_buffer (0));
#else
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      retval[i] = this->cs_set_.at (i);
    }
#endif

  return retval._retn ();
}
*/
