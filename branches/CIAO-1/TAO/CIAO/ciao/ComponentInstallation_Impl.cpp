// $Id$

#include "ComponentInstallation_Impl.h"
#include "ace/Configuration_Import_Export.h"

#if !defined (__ACE_INLINE__)
# include "ComponentInstallation_Impl.inl"
#endif /* __ACE_INLINE__ */

CIAO::ComponentInstallation_Impl::~ComponentInstallation_Impl ()
{
  this->fini ();
}

PortableServer::POA_ptr
CIAO::ComponentInstallation_Impl::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

int
CIAO::ComponentInstallation_Impl::init (const char *fname,
                                        const char *section
                                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (fname == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (), -1);
  else
    this->filename_ = CORBA::string_dup (fname);

  if (section == 0)
    section = "ComponentInstallation";
  this->section_name_ = CORBA::string_dup (section);

  ACE_Configuration_Heap *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    ACE_Configuration_Heap (),
                    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (-1);

  auto_ptr<ACE_Configuration_Heap> config (tmp);

  if (config->open () != 0)
    {
      ACE_DEBUG ((LM_ERROR, "Unable to initilize installation datafile\n"));
      ACE_THROW_RETURN (CORBA::INTERNAL (), -1);
    }

  ACE_Ini_ImpExp import (*config);

  if (import.import_config (fname) != 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (), -1);

  this->installation_ = config.release ();

  return 0;
}

int
CIAO::ComponentInstallation_Impl::fini ()
{
  if (this->installation_ != 0)
    {
      // @ back up the installation and destroy the
      ACE_Ini_ImpExp exp (*this->installation_);

      if (exp.export_config (this->filename_.in ()) != 0)
        ACE_DEBUG ((LM_DEBUG,
                    "CIAO::ComponentInstallation Failed to store the installation information to file: %s\n",
                    this->filename_.in ()));
      delete this->installation_;
      this->installation_ = 0;
    }
  return 0;
}

void
CIAO::ComponentInstallation_Impl::install (const char * implUUID,
                                           const char * component_loc
                                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::Deployment::InvalidLocation,
                   Components::Deployment::InstallationFailure))
{
  // Only use the root section for now.
  const ACE_Configuration_Section_Key &root_section
    = this->installation_->root_section ();
  ACE_Configuration::VALUETYPE type;

  ACE_Configuration_Section_Key section;
  this->installation_->open_section (root_section,
                                     this->section_name_.in (),
                                     1,
                                     section);

  // Check if implUUID has already been installed.
  if (this->installation_->find_value (section,
                                       implUUID,
                                       type) == 0)
    ACE_THROW (Components::Deployment::InstallationFailure ());

  // @@ We may need to do some extra work to provide a more
  // comprehensive component installation facility.

  ACE_TString value (component_loc, 0, 0);
  if (this->installation_->set_string_value (section,
                                             implUUID,
                                             value) != 0)
    ACE_THROW (Components::Deployment::InstallationFailure ());
}

void
CIAO::ComponentInstallation_Impl::replace (const char * implUUID,
                                           const char * component_loc
                                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::Deployment::InvalidLocation,
                   Components::Deployment::InstallationFailure))
{
  // Only use the root section for now.
  const ACE_Configuration_Section_Key &root_section
    = this->installation_->root_section ();

  ACE_Configuration_Section_Key section;
  this->installation_->open_section (root_section,
                                     this->section_name_.in (),
                                     1,
                                     section);

  // @@ We may need to do some extra work to provide a more
  // comprehensive component installation facility.

  ACE_TString value (component_loc, 0, 0);
  if (this->installation_->set_string_value (section,
                                             implUUID,
                                             value) != 0)
    ACE_THROW (Components::Deployment::InstallationFailure ());
}

void
CIAO::ComponentInstallation_Impl::remove (const char * implUUID
                                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::Deployment::UnknownImplId,
                   Components::RemoveFailure))
{
  if (implUUID == 0)
    ACE_THROW (Components::Deployment::UnknownImplId ());

  // Only use the root section for now.
  const ACE_Configuration_Section_Key &root_section
    = this->installation_->root_section ();

  ACE_Configuration_Section_Key section;
  this->installation_->open_section (root_section,
                                     this->section_name_.in (),
                                     1,
                                     section);

  if (this->installation_->remove_value (section,
                                         implUUID) != 0)
    // This should very well be UnknownImplId instead.
    ACE_THROW (Components::RemoveFailure ());
}

char *
CIAO::ComponentInstallation_Impl::get_implementation (const char * implUUID
                                                      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::Deployment::UnknownImplId,
                   Components::Deployment::InstallationFailure))
{
  const ACE_Configuration_Section_Key &root_section
    = this->installation_->root_section ();
  ACE_TString retstr;

  ACE_Configuration_Section_Key section;
  this->installation_->open_section (root_section,
                                     this->section_name_.in (),
                                     1,
                                     section);

  // Check if implUUID has already been installed.
  if (this->installation_->get_string_value (section,
                                             implUUID,
                                             retstr) != 0)
    ACE_THROW (Components::Deployment::UnknownImplId ());

  return CORBA::string_dup (retstr.fast_rep ());
}
