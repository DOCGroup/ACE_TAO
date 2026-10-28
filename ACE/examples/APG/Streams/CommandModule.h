/* -*- C++ -*- */
#ifndef COMMAND_MODULE_H
#define COMMAND_MODULE_H

#include "ace/Module.h"
#include "ace/SOCK_Stream.h"
#include "CommandTask.h"

// Listing 01 code/ch18
class CommandModule : public ACE_Module<ACE_MT_SYNCH, ACE_System_Time_Policy>
{
public:
  using inherited = ACE_Module<ACE_MT_SYNCH, ACE_System_Time_Policy>;
  using Task = ACE_Task<ACE_MT_SYNCH, ACE_System_Time_Policy>;

  CommandModule (const ACE_TCHAR *module_name,
                   CommandTask *writer,
                   CommandTask *reader,
                   ACE_SOCK_Stream *peer);

  ACE_SOCK_Stream &peer ();
};
// Listing 01

#endif /* COMMAND_MODULE_H */
