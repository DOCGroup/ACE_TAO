// $Id$

#include "ACEXML/compass/ComponentInstallation.h"
#include "ACEXML/common/ZipCharStream.h"

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
    {
      ACE_ERROR ((LM_ERROR, "Implementation id is an Empty string\n"));
      ACE_THROW (InstallationFailure());
    }
  else if (component_loc == empty_string)
    {
      ACE_ERROR ((LM_ERROR, "Component Location is an Empty string\n"));
      ACE_THROW (InvalidLocation());
    }
  else if (this->packages_->bind (implUUID, component_loc) != 0)
    {
      ACE_ERROR ((LM_ERROR, "Component %s already installed\n",
                  implUUID.c_str()));
      ACE_THROW (InstallationFailure());
    }
    ACEXML_StreamFactory factory;
  ACEXML_CharStream* stream = factory.create_stream (component_loc.c_str());
  if (!stream)
    {
      ACE_ERROR ((LM_ERROR, "Unable to create a stream for the Component"
                  "location %s\n", component_loc.c_str()));
      ACE_THROW (InstallationFailure());
    }
    ACEXML_Char temp[MAXNAMLEN + 1] = "acefileXXXXXX";
  if (mkdtemp (temp) == 0)
    {
      ACE_ERROR ((LM_ERROR, "Unable to create safe temporary directory\n"));
      ACE_THROW (InstallationFailure());
    }
  ACEXML_String dsoname = temp +
  FILE* dso = ACE_OS::fopen (temp, "w");
  if (dso == 0)
    {

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
