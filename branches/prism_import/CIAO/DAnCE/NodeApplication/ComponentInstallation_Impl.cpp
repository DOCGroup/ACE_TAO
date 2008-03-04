// $Id$

#include "ComponentInstallation_Impl.h"

using namespace DAnCE;

ComponentInstallation_Impl::ComponentInstallation_Impl()
{
  ACE_DEBUG ( (LM_DEBUG, "[%M] ComponentInstallation_Impl::ComponentInstallation_Impl - started\n"));
  ACE_DEBUG ( (LM_DEBUG, "[%M] ComponentInstallation_Impl::ComponentInstallation_Impl - finished\n"));
}

ComponentInstallation_Impl::~ComponentInstallation_Impl()
{
  ACE_DEBUG ( (LM_DEBUG, "[%M] ComponentInstallation_Impl::~ComponentInstallation_Impl - started\n"));
  ACE_DEBUG ( (LM_DEBUG, "[%M] ComponentInstallation_Impl::~ComponentInstallation_Impl - finished\n"));
}

void
ComponentInstallation_Impl::install (const char * implUUID,    const char * component_loc)
ACE_THROW_SPEC ( (
                   ::CORBA::SystemException,
                   ::Components::Deployment::InvalidLocation,
                   ::Components::Deployment::InstallationFailure
                 ))
{
  ACE_DEBUG ( (LM_DEBUG, "[%M] ComponentInstallation_Impl::install - started\n"));

  ACE_DEBUG ( (LM_DEBUG, "[%M] ComponentInstallation_Impl::install - implUUID %s, component_loc %s\n", implUUID, component_loc));
  if (0 != this->locations_.find (implUUID))
    {
      ACE_CString location = component_loc;
      this->locations_.bind (implUUID, location);
    }
  else
    {
      // I don't certaint that we should throw exception here
      //throw ::Components::Deployment::InstallationFailure();
    }

  ACE_DEBUG ( (LM_DEBUG, "[%M] ComponentInstallation_Impl::install - finished\n"));
}


void
ComponentInstallation_Impl::replace (const char * , const char *)
ACE_THROW_SPEC ( (
                   ::CORBA::SystemException,
                   ::Components::Deployment::InvalidLocation,
                   ::Components::Deployment::InstallationFailure
                 ))
{
  ACE_DEBUG ( (LM_DEBUG, "[%M] ComponentInstallation_Impl::replace - started\n"));
  ACE_DEBUG ( (LM_DEBUG, "[%M] ComponentInstallation_Impl::replace - finished\n"));
}

void
ComponentInstallation_Impl::remove (const char *)
ACE_THROW_SPEC ( (
                   ::CORBA::SystemException,
                   ::Components::Deployment::UnknownImplId,
                   ::Components::RemoveFailure
                 ))
{
  ACE_DEBUG ( (LM_DEBUG, "[%M] ComponentInstallation_Impl::remove - started\n"));
  ACE_DEBUG ( (LM_DEBUG, "[%M] ComponentInstallation_Impl::remove - finished\n"));
}

char *
ComponentInstallation_Impl::get_implementation (const char * implUUID)
ACE_THROW_SPEC ( (
                   ::CORBA::SystemException,
                   ::Components::Deployment::UnknownImplId,
                   ::Components::Deployment::InstallationFailure
                 ))
{
  ACE_DEBUG ( (LM_DEBUG, "[%M] ComponentInstallation_Impl::get_implementation - started\n"));

  ACE_CString s;
  if (0 == this->locations_.find (implUUID, s))
    {
      ACE_DEBUG ( (LM_DEBUG, "[%M] ComponentInstallation_Impl::get_implementation - ComponentInstallation_Impl::get_implementation for UUID %s, location %s\n", implUUID, s.c_str()));
      ACE_DEBUG ( (LM_DEBUG, "[%M] ComponentInstallation_Impl::get_implementation - finished\n"));
      return CORBA::string_dup (s.c_str());
    }
  else
    {
      ACE_ERROR ( (LM_ERROR, "[%M] ComponentInstallation_Impl::get_implementation - cannot find location for specified implementation UUID\n"));
      throw ::Components::Deployment::UnknownImplId();
      return 0;
    }
}


char *
ComponentInstallation_Impl::get_valuetypefactory_location (
  const char * implUUID,
  const char * repid
)
ACE_THROW_SPEC ( (
                   ::CORBA::SystemException,
                   ::Components::Deployment::UnknownImplId,
                   ::CdmwDeployment::UnknownValuetype,
                   ::Components::Deployment::InstallationFailure
                 ))
{
  ACE_DEBUG ( (LM_DEBUG, "[%M] ComponentInstallation_Impl::get_valuetypefactory_location - started\n"));
  ACE_CString key = ComponentInstallation_Impl::valuefactory_key (implUUID, repid);
  ACE_CString s;
  if (0 == this->locations_.find (implUUID, s))
    {
      ACE_DEBUG ( (LM_DEBUG, "[%M] ComponentInstallation_Impl::get_valuetypefactory_location - UUID:\"%s\" repid:\"%s\" -> location:\"%s\"\n", implUUID, repid, s.c_str()));
      return CORBA::string_dup (s.c_str());
    }
  else
    {
      ACE_ERROR ( (LM_ERROR, "[%M] ComponentInstallation_Impl::get_valuetypefactory_location - cannot find location for specified implementation UUID and repid.\n"));
      throw ::Components::Deployment::InstallationFailure();
      return 0;
    }
  ACE_DEBUG ( (LM_DEBUG, "[%M] ComponentInstallation_Impl::get_valuetypefactory_location - finished\n"));
}


void
ComponentInstallation_Impl::install_valuetypefactory_location (
  const char * implUUID,
  const char * repid,
  const char * loc
)
ACE_THROW_SPEC ( (
                   ::CORBA::SystemException,
                   ::Components::Deployment::InvalidLocation,
                   ::Components::Deployment::InstallationFailure
                 ))
{
  ACE_CString key = ComponentInstallation_Impl::valuefactory_key (implUUID, repid);
  if (0 != this->locations_.find (key))
    {
      ACE_CString location = loc;
      this->locations_.bind (implUUID, location);
    }
  else
    {
      // I don't certaint that we should throw exception here
      //throw ::Components::Deployment::InstallationFailure();
    }
}

