// $Id$

#include "ACEXML/compass/ComponentInstallation.h"

using namespace Deployment;

static const empty_string[] = {0};

ComponentInstallation::ComponentInstallation()
  : packages_()
{}

ComponentInstallation::~ComponentInstallation()
{}

void
ComponentInstallation::install (const UUID& implUUID,
                                const Location& component_loc)
  ACE_THROW_SPEC ((InvalidLocation,InstallationFailure))
{
  if (implUUID == empty_string)
    ACE_THROW (InstallationFailure());
  else if (component_loc == empty_string)
    ACE_THROW (InvalidLocation());
  else if (this->packages_->bind (implUUID, component_loc) != 0)
    ACE_THROW (InstallationFailure());
}

void
ComponentInstallation::replace (const UUID& implUUID,
                                const Location& component_loc)
  ACE_THROW_SPEC ((InvalidLocation,InstallationFailure))
{
  if (implUUID == empty_string)
    ACE_THROW (InstallationFailure());
  else if (component_loc == empty_string)
    ACE_THROW (InvalidLocation());
  else
    {
      if (this->packages_->unbind (implUUID) != 0)
        ACE_THROW (InstallationFailure());
      else if (this->packages_->bind (implUUID, component_loc) != 0)
        ACE_THROW (InstallationFailure());
    }
}

void
ComponentInstallation::remove (const UUID& implUUID)
  ACE_THROW_SPEC ((UnknownImplId, RemoveFailure));
{
  if (implUUID == empty_string)
    ACE_THROW (UnknownImplId());
  else if (this->packages_->unbind (implUUID) != 0)
    ACE_THROW (RemoveFailure());
}
