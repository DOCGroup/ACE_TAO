// -*- c++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Concurrency
//
// = FILENAME
//    CC_command.h
//
// = DESCRIPTION
//      This file implements the command possible to execute on the
//      concurrency service's lock set interface. The file also contains
//      an implementation of a command list used by the command parser to
//      store the commands to be executed.
//
// = AUTHORS
//    Torben Worm <tworm@cs.wustl.edu>
//
// ============================================================================

#include "orbsvcs/CosConcurrencyControlC.h"

#ifndef _CC_COMMAND_H_
#define _CC_COMMAND_H_

class CC_Command
  // = TITLE
  //    Defines an abstact class for the commands towards the lock set
  //
  // = DESCRIPTION
  //    This class declares an interface to run a test towards one or more
  //    lock sets in the concurrency service. The class provides a virtual
  //    execute method that is common for all its subclasses. It is also the
  //    the base class for the auxillary commands start, wait, and sleep.
{
 public:
  virtual ~CC_Command(void);
  // Destructor

  virtual int execute() = 0;
  // Abstract execute method

 protected:
  CosConcurrencyControl::LockSet_var
    GetLockSet (const char *lock_set_name
                TAO_ENV_ARG_DECL);
  // Function to look up the lock set we are operating on, or if the
  // name is "" return the global lock set variable

  CC_Command(void);
  // Default constructor. We do not want instances of this class

  static CORBA::Exception *excep_;
  // The last exception raised in one of the test commands. This variable
  // is checked by all commands in order to determine if an exception has
  // been raised. This is necessary because sometimes we want to check that
  // an event caused an exception (done by the CC_Excep_Cmd command class)
  // as part of the testing.

  static CosConcurrencyControl::LockSet_var cc_lockset_;
  // This is the default lock set. The variable is either set by a create
  // command without name or by the lookup command.

 private:
};

class CC_Start_Cmd : public CC_Command
  // = TITLE
  //    Defines a class for the start command
  //
  // = DESCRIPTION
  //   This class represents the start command. The start command is used
  //   to start a new process with another script file in order to test the
  //   aspects of the concurrency service that requires several processes
  //   running at the same time.
{
 public:
  CC_Start_Cmd (const char *config_file_name);
  // Constructor

  virtual ~CC_Start_Cmd();
  // Destructor

  virtual int execute(void);
  // Start the child process. The current version does not wait for the
  // process to terminate.

private:
  char *cfg_name_;
  // The name of the script file
};

class CC_CreateLockSet_Cmd : public CC_Command
  // = TITLE
  //    Defines a class for the create command on the lock set factory
  //
  // = DESCRIPTION
  //   This class represents the create command on the lock set factory.
  //   The lock set is registered in the naming service with the provided
  //   name.
{
 public:
  CC_CreateLockSet_Cmd (const char *lock_set_name);
  // Constructor

  virtual ~CC_CreateLockSet_Cmd ();
  // Destructor

  virtual int execute(void);
  // Executes the command, i.e. creates the lock set and binds the name
  // in the naming service.

private:
  char *name_;
  // The name used to bind in the naming service.
};

class CC_Lock_Cmd:public CC_Command
  // = TITLE
  //    Defines a class for the lock command on the lock set
  //
  // = DESCRIPTION
  //   This class represents the lock command on the lock set. The lock set
  //   is looked up in the naming service.
{
 public:
  CC_Lock_Cmd(const char *lock_set_name,
              CosConcurrencyControl::lock_mode mode);
  // Constructor.

  virtual ~CC_Lock_Cmd();
  // Destructor

  virtual int execute(void);
  // Executes the command, i.e. looks up the lock set with the requested
  // name in the naming server and executes the lock command on that lock set.

private:
  char *name_;
  // The name to look up in the naming service.

  CosConcurrencyControl::lock_mode mode_;
  // The mode of the lock.
};

class CC_UnLock_Cmd:public CC_Command
  // = TITLE
  //    Defines a class for the unlock command on the lock set
  //
  // = DESCRIPTION
  //   This class represents the unlock command on the lock set. The lock set
  //   is looked up in the naming service.
{
 public:
  CC_UnLock_Cmd(const char *lock_set_name,
                CosConcurrencyControl::lock_mode mode);
  // Constructor.

  virtual ~CC_UnLock_Cmd();
  // Destructor

  virtual int execute(void);
  // Executes the command, i.e. looks up the lock set with the requested
  // name in the naming server and executes the unlock command on that
  // lock set.

private:
  char *name_;
  // The name to look up in the naming service.

  CosConcurrencyControl::lock_mode mode_;
  // The mode of the lock.
};

class CC_TryLock_Cmd:public CC_Command
  // = TITLE
  //    Defines a class for the try_lock command on the lock set
  //
  // = DESCRIPTION
  //   This class represents the try_lock command on the lock set. The lock set
  //   is looked up in the naming service.
{
 public:
  CC_TryLock_Cmd(const char *lock_set_name,
                 CosConcurrencyControl::lock_mode mode);
  // Constructor

  virtual ~CC_TryLock_Cmd();
  // Destructor

  virtual int execute(void);
  // Executes the command, i.e. looks up the lock set with the requested
  // name in the naming server and executes the try_lock command on that
  // lock set.

private:
  char *name_;
  // The name to look up in the naming service.

  CosConcurrencyControl::lock_mode mode_;
  // The mode of the lock.
};

class CC_ChangeMode_Cmd:public CC_Command
  // = TITLE
  //    Defines a class for the change_mode command on the lock set
  //
  // = DESCRIPTION
  //   This class represents the change_mode command on the lock set.
  //   The lock set is looked up in the naming service.
{
 public:
  CC_ChangeMode_Cmd (const char *lock_set_name,
                     CosConcurrencyControl::lock_mode held_mode,
                     CosConcurrencyControl::lock_mode new_mode);
  // Constructor

  virtual ~CC_ChangeMode_Cmd();
  // Destructor

  virtual int execute(void);
  // Executes the command, i.e. looks up the lock set with the requested
  // name in the naming server and executes the change_mode command on that
  // lock set.

private:
  char *name_;
  // The name to look up in the naming service.

  CosConcurrencyControl::lock_mode held_mode_;
  // The mode of the held lock

  CosConcurrencyControl::lock_mode new_mode_;
  // The new mode of the lock
};

class CC_Sleep_Cmd:public CC_Command
  // = TITLE
  //    Defines a class for the sleep command
  //
  // = DESCRIPTION
  //   This class represents the sleep command. This command is used to make
  //   the script pause for the requested number of second, e.g. to wait for
  //   another process to start.
{
 public:
  CC_Sleep_Cmd(int seconds);
  // Constructor.

  virtual ~CC_Sleep_Cmd();
  // Destructor.

  virtual int execute(void);
  // Executes the command.

 private:
  int time_;
  // The number of seconds to sleep
};

class CC_Repeat_Cmd:public CC_Command
  // = TITLE
  //    Defines a class for the repeat command
  //
  // = DESCRIPTION
  //   This class represents the repeat command. This command is used to make
  //   the script repeat the test the requested number of times.
  //   The command is curently NOT implemented.
{
 public:
  CC_Repeat_Cmd(int times);
  // Constructor.

  virtual ~CC_Repeat_Cmd();
  // Destructor.

  virtual int execute(void);
  // Executes the command.
 private:
  int times_;
  // The number of times the commands should be repeated
};

class CC_Wait_Cmd:public CC_Command
  // = TITLE
  //    Defines a class for the wait command
  //
  // = DESCRIPTION
  //   This class represents the wait command. This command is used to make
  //   the script wait for the user to press return. It is possible to print
  //   different prompts, e.g. instructions.
{
 public:
  CC_Wait_Cmd (const char *prompt);
  // Constructor.

  virtual ~CC_Wait_Cmd();
  // Destructor.

  virtual int execute(void);
  // Executes the command.

private:
  char *prompt_;
  // The prompt to print on the screen
};

class CC_Excep_Cmd : public CC_Command
  // = TITLE
  //    Defines a class for the excep command
  //
  // = DESCRIPTION
  //   This class represents the excep command. This command is used to make
  //   the script capable of dealing with cases where an exception is raised
  //   as part of the testing.
{
 public:
  CC_Excep_Cmd (const char *excep);
  // Constructor.

  virtual ~CC_Excep_Cmd(void);
  // Destructor.

  virtual int execute(void);
  // Executes the command. Checks to see if the excep_ class variable is set,
  // and if that's the case check that it is of the expected type. If not the
  // test fails.

private:
  char *ex_;
  // The string representation of the expected exception
};

class CC_Dummy_Cmd: public CC_Command
  // = TITLE
  //    Defines a class for the dummy command
  //
  // = DESCRIPTION
  //   This class represents the dummy command. This command is used to
  //   put in a command in the chain that does nothing.
{
 public:
  CC_Dummy_Cmd(void);
  // Constructor.

  virtual ~CC_Dummy_Cmd(void);
  // Destructor.

  virtual int execute(void);
  // Executes the command, i.e. does nothing.

 private:
};

class CC_Print_Cmd: public CC_Command
  // = TITLE
  //    Defines a class for the print command
  //
  // = DESCRIPTION
  //   This class represents the print command. This command is used to
  //   print a message on stdout
{
public:
  CC_Print_Cmd (const char *message);
  // Constructor.

  virtual ~CC_Print_Cmd(void);
  // Destructor.

  virtual int execute(void);
  // Executes the command.

private:
  char *msg_;
  // Holds the message to print
};

class CC_Lookup_Cmd:public CC_Command
  // = TITLE
  //    Defines a class for the lookup command.
  //
  // = DESCRIPTION
  //   This class represents the lookup command. The lock set
  //   is looked up in the naming service and the class variable
  //   cc_lockset_ is set accordingly.
{
public:
  CC_Lookup_Cmd (const char *lock_set_name);
  // Constructor

  virtual ~CC_Lookup_Cmd();
  // Destructor

  virtual int execute(void);
  // Executes the command, i.e. looks up the lock set with the requested
  // name in the naming server and sets the cc_lockset_ variable.

private:
  char *name_;
  // The name to look up in the naming service.

};

class CC_CommandElem
  // = TITLE
  //    Defines a class for a command element
  //
  // = DESCRIPTION
  //    This class implements a command element used in the command list
  //    below. The class is a simple tupple holding a pointer to the command
  //    and a pointer to the next element in the list.
{
 public:
  CC_CommandElem (CC_Command *cmd, CC_CommandElem *next);
  // Constructor.

  ~CC_CommandElem(void);
  // Destructor.

  CC_Command *GetCommand(void);
  // Returns a pointer to the command in this element

  CC_CommandElem *GetNext(void);
  // Returns the pointer to the next element

  void SetNext(CC_CommandElem *next);
  // Sets the next pointer

 private:
  CC_CommandElem *next_;
  // Pointer to the next element

  CC_Command *cmd_;
  // Pointer to the command element
};

class CC_CommandList
  // = TITLE
  //    Defines a class for a command list
  //
  // = DESCRIPTION
  //    This class implements a command list used from the script parser
  //    to store the parsed commands The class implements a simple single
  //    linked list.
{
 public:
  CC_CommandList(void);
  // Constructor.

  ~CC_CommandList(void);
  // Destructor.

  int add(CC_Command *cmd);
  // Adds the command to the list

  int execute(void);
  // Executes all the commands in the list from head to tail

  void setrepeat(int times);
  // Sets the number of times to repeat the script

 private:
  CC_CommandElem *head_;
  //  The head of the command list

  CC_CommandElem *last_;
  // The last inserted element

  int times_;
  // The number of times the script should be repeated
};

#endif /* _CC_COMMAND_H_ */
