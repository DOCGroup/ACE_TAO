// $Id$

#include "ACEXML/compass/ComponentInstallation.h"
#include "ACEXML/common/ZipCharStream.h"

using namespace Deployment;

static const empty_string[] = {0};

ComponentInstallation::ComponentInstallation()
  : packages_()
{}

ComponentInstallation::~ComponentInstallation()
{

}

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
  ACEXML_Char* file = ACE_OS::strrchr (component_loc.c_str(), '/');
  if (!file)
    file = component_loc.c_str();
  else
    file += 1;
  ACEXML_String dsoname = temp + '/' + file;
  ACE_HANDLE dso = ACE_OS::open (dsoname.c_str(),
                                 O_WRONLY| O_CREAT|O_EXCL, S_IRWXU);
  if (dso == 0)
    {
      ACE_ERROR ((LM_ERROR, "Unable to unpack the implementation %s : %m\n",
                  dsoname.c_str()));
      ACE_THROW (InstallationFailure());
    }
  ACEXML_Char buf[65535];
  int bytes = 0;
  while ((bytes = stream.read (buf, sizeof(buf))) > 0)
    {
      if (ACE_OS::write (dso, buf, bytes) != bytes)
        {
          ACE_ERROR ((LM_ERROR, "Unable to unpack the implementation %s: %m\n",
                      dsoname.c_str()));
          ACE_THROW (InstallationFailure());
        }
    }
  ACE_OS::close (dso);
  if (this->packages_->bind (implUUID, dsoname) != 0)
    {
      ACE_ERROR ((LM_ERROR, "Component %s already installed\n",
                  implUUID.c_str()));
      ACE_THROW (InstallationFailure());
    }
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
      this->install (implUUID, component_loc);
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
