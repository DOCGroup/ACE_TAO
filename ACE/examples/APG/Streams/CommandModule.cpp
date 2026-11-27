#include "CommandModule.h"

// Listing 01 code/ch18
CommandModule::CommandModule (ACE_TCHAR const *module_name,
                              CommandTask *writer,
                              CommandTask *reader,
                              ACE_SOCK_Stream *peer)
  : inherited(module_name, writer, reader, peer)
{ }
// Listing 01

// Listing 02 code/ch18
ACE_SOCK_Stream &CommandModule::peer ()
{
  ACE_SOCK_Stream *peer = static_cast<ACE_SOCK_Stream *> (this->arg ());
  return *peer;
}
// Listing 02
