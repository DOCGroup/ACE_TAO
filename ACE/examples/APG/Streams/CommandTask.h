/* -*- C++ -*- */
#ifndef COMMAND_TASK_H
#define COMMAND_TASK_H

#include "ace/Task.h"
#include "ace/Module.h"

#include "Command.h"

// Listing 01 code/ch18
class CommandTask : public ACE_Task<ACE_MT_SYNCH>
{
public:
  using inherited = ACE_Task<ACE_MT_SYNCH>;

  virtual ~CommandTask () = default;

  int open (void * = nullptr) override;

  int put (ACE_Message_Block *message,
           ACE_Time_Value *timeout);

  int svc () override;

  int close (u_long flags) override;

protected:
  explicit CommandTask (int command);

  virtual int process (Command *message);

  int command_;
};
// Listing 01


#endif /* COMMAND_TASK_H */
