//$Id$

#include "Updates.h"

ACE_RCSID (Notify_Tests, Notify_Test_Updates_Client, "$Id$")

Notify_Test_Updates_Client::Notify_Test_Updates_Client (void)
{
}

Notify_Test_Updates_Client::~Notify_Test_Updates_Client ()
{
}

void
Notify_Test_Updates_Client::init_concrete (int argc, char *argv [], CORBA::Environment &ACE_TRY_ENV)
{
}

void
Notify_Test_Updates_Client::run_test (CORBA::Environment &ACE_TRY_ENV)
{
}

/***************************************************************************/
int
main (int argc, char* argv[])
{
  Notify_Test_Updates_Client client;

  return Notify_Test_Client::main (argc, argv, client);
}
