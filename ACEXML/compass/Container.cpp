// $Id$

#include "ACEXML/compass/Container.h"

static ACE_TCHAR empty_string[] = { 0 };

using namespace Deployment;

Container::Container (ComponentServer* server)
  : config_ (0), homes_ (0), parent_ (server)
{}

Container::~Container()
{
  this->remove();
}

CCMHome*
Container::install_home (const UUID& id, const string& entrypt,
                         const ConfigValues* config)
  ACE_THROW_SPEC ((UnknownImplId, ImplEntryPointNotFound,
                   InstallationFailure, InvalidConfiguration))
{
  if (COMP
