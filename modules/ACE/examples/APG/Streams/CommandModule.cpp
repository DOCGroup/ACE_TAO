// $Id$

#include "CommandModule.h"

// Listing 01 code/ch18
CommandModule::CommandModule (const ACE_TCHAR *module_name,
                              CommandTask *writer,
                              CommandTask *reader,
                              ACE_SOCK_Stream *peer)
  : inherited(module_name, writer, reader, peer)
{ }
// Listing 01

// Listing 02 code/ch18
ACE_SOCK_Stream &CommandModule::peer (void)
{
  ACE_SOCK_Stream *peer = (ACE_SOCK_Stream *)this->arg ();
  return *peer;
}
// Listing 02
