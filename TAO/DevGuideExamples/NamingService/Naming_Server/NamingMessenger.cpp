#include "NamingTask.h"
#include "MessengerTask.h"
#include <ace/OS.h>

int main(int argc, char* argv[])
{
  // Start the Naming Service task
  NamingTask namingService(argc, argv);
  namingService.activate();
  // Wait for the Naming Service initialized.
  namingService.waitInit();

  // Start the Messenger task
  MessengerTask messenger;
  messenger.activate();

  // Wait the Messenger task finish. 
  messenger.wait();
  // Shutdown the Naming Service.
  namingService.end();

  return 0;
}
