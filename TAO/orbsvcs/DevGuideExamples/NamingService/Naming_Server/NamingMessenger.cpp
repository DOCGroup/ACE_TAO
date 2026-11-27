#include "NamingTask.h"
#include "MessengerTask.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  // Start the Naming Service task
  NamingTask naming_service (argc, argv);
  naming_service.activate ();
  // Wait for the Naming Service initialized.
  naming_service.waitInit ();

  // Start the Messenger task
  MessengerTask messenger;
  messenger.activate ();

  // Wait the Messenger task finish.
  messenger.wait ();
  // Shutdown the Naming Service.
  naming_service.end ();

  return 0;
}
