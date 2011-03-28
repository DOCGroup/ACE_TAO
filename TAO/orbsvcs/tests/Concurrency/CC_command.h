
//=============================================================================
/**
 *  @file    CC_command.h
 *
 *  $Id$
 *
 *    This file implements the command possible to execute on the
 *    concurrency service's lock set interface. The file also contains
 *    an implementation of a command list used by the command parser to
 *    store the commands to be executed.
 *
 *
 *  @author Torben Worm <tworm@cs.wustl.edu>
 */
//=============================================================================


#include "orbsvcs/CosConcurrencyControlC.h"

#ifndef _CC_COMMAND_H_
#define _CC_COMMAND_H_

/**
 * @class CC_Command
 *
 * @brief Defines an abstact class for the commands towards the lock set
 *
 * This class declares an interface to run a test towards one or more
 * lock sets in the concurrency service. The class provides a virtual
 * execute method that is common for all its subclasses. It is also the
 * the base class for the auxillary commands start, wait, and sleep.
 */
class CC_Command
{
 public:
  /// Destructor
  virtual ~CC_Command(void);

  /// Abstract execute method
  virtual int execute() = 0;

 protected:
  /// Function to look up the lock set we are operating on, or if the
  /// name is "" return the global lock set variable
  CosConcurrencyControl::LockSet_var
    GetLockSet (const char *lock_set_name);

  /// Default constructor. We do not want instances of this class
  CC_Command(void);

  /**
   * The last exception raised in one of the test commands. This variable
   * is checked by all commands in order to determine if an exception has
   * been raised. This is necessary because sometimes we want to check that
   * an event caused an exception (done by the CC_Excep_Cmd command class)
   * as part of the testing.
   */
  static CORBA::Exception *excep_;

  /// This is the default lock set. The variable is either set by a create
  /// command without name or by the lookup command.
  static CosConcurrencyControl::LockSet_var cc_lockset_;

 private:
};

/**
 * @class CC_Start_Cmd
 *
 * @brief Defines a class for the start command
 *
 * This class represents the start command. The start command is used
 * to start a new process with another script file in order to test the
 * aspects of the concurrency service that requires several processes
 * running at the same time.
 */
class CC_Start_Cmd : public CC_Command
{
 public:
  /// Constructor
  CC_Start_Cmd (const char *config_file_name);

  /// Destructor
  virtual ~CC_Start_Cmd();

  /// Start the child process. The current version does not wait for the
  /// process to terminate.
  virtual int execute(void);

private:
  /// The name of the script file
  char *cfg_name_;
};

/**
 * @class CC_CreateLockSet_Cmd
 *
 * @brief Defines a class for the create command on the lock set factory
 *
 * This class represents the create command on the lock set factory.
 * The lock set is registered in the naming service with the provided
 * name.
 */
class CC_CreateLockSet_Cmd : public CC_Command
{
 public:
  /// Constructor
  CC_CreateLockSet_Cmd (const char *lock_set_name);

  /// Destructor
  virtual ~CC_CreateLockSet_Cmd ();

  /// Executes the command, i.e. creates the lock set and binds the name
  /// in the naming service.
  virtual int execute(void);

private:
  /// The name used to bind in the naming service.
  char *name_;
};

/**
 * @class CC_Lock_Cmd:public
 *
 * @brief Defines a class for the lock command on the lock set
 *
 * This class represents the lock command on the lock set. The lock set
 * is looked up in the naming service.
 */
class CC_Lock_Cmd:public CC_Command
{
 public:
  /// Constructor.
  CC_Lock_Cmd(const char *lock_set_name,
              CosConcurrencyControl::lock_mode mode);

  /// Destructor
  virtual ~CC_Lock_Cmd();

  /// Executes the command, i.e. looks up the lock set with the requested
  /// name in the naming server and executes the lock command on that lock set.
  virtual int execute(void);

private:
  /// The name to look up in the naming service.
  char *name_;

  /// The mode of the lock.
  CosConcurrencyControl::lock_mode mode_;
};

/**
 * @class CC_UnLock_Cmd:public
 *
 * @brief Defines a class for the unlock command on the lock set
 *
 * This class represents the unlock command on the lock set. The lock set
 * is looked up in the naming service.
 */
class CC_UnLock_Cmd:public CC_Command
{
 public:
  /// Constructor.
  CC_UnLock_Cmd(const char *lock_set_name,
                CosConcurrencyControl::lock_mode mode);

  /// Destructor
  virtual ~CC_UnLock_Cmd();

  /**
   * Executes the command, i.e. looks up the lock set with the requested
   * name in the naming server and executes the unlock command on that
   * lock set.
   */
  virtual int execute(void);

private:
  /// The name to look up in the naming service.
  char *name_;

  /// The mode of the lock.
  CosConcurrencyControl::lock_mode mode_;
};

/**
 * @class CC_TryLock_Cmd:public
 *
 * @brief Defines a class for the try_lock command on the lock set
 *
 * This class represents the try_lock command on the lock set. The lock set
 * is looked up in the naming service.
 */
class CC_TryLock_Cmd:public CC_Command
{
 public:
  /// Constructor
  CC_TryLock_Cmd(const char *lock_set_name,
                 CosConcurrencyControl::lock_mode mode);

  /// Destructor
  virtual ~CC_TryLock_Cmd();

  /**
   * Executes the command, i.e. looks up the lock set with the requested
   * name in the naming server and executes the try_lock command on that
   * lock set.
   */
  virtual int execute(void);

private:
  /// The name to look up in the naming service.
  char *name_;

  /// The mode of the lock.
  CosConcurrencyControl::lock_mode mode_;
};

/**
 * @class CC_ChangeMode_Cmd:public
 *
 * @brief Defines a class for the change_mode command on the lock set
 *
 * This class represents the change_mode command on the lock set.
 * The lock set is looked up in the naming service.
 */
class CC_ChangeMode_Cmd:public CC_Command
{
 public:
  /// Constructor
  CC_ChangeMode_Cmd (const char *lock_set_name,
                     CosConcurrencyControl::lock_mode held_mode,
                     CosConcurrencyControl::lock_mode new_mode);

  /// Destructor
  virtual ~CC_ChangeMode_Cmd();

  /**
   * Executes the command, i.e. looks up the lock set with the requested
   * name in the naming server and executes the change_mode command on that
   * lock set.
   */
  virtual int execute(void);

private:
  /// The name to look up in the naming service.
  char *name_;

  /// The mode of the held lock
  CosConcurrencyControl::lock_mode held_mode_;

  /// The new mode of the lock
  CosConcurrencyControl::lock_mode new_mode_;
};

/**
 * @class CC_Sleep_Cmd:public
 *
 * @brief Defines a class for the sleep command
 *
 * This class represents the sleep command. This command is used to make
 * the script pause for the requested number of second, e.g. to wait for
 * another process to start.
 */
class CC_Sleep_Cmd:public CC_Command
{
 public:
  /// Constructor.
  CC_Sleep_Cmd(int seconds);

  /// Destructor.
  virtual ~CC_Sleep_Cmd();

  /// Executes the command.
  virtual int execute(void);

 private:
  /// The number of seconds to sleep
  int time_;
};

/**
 * @class CC_Repeat_Cmd:public
 *
 * @brief Defines a class for the repeat command
 *
 * This class represents the repeat command. This command is used to make
 * the script repeat the test the requested number of times.
 * The command is curently NOT implemented.
 */
class CC_Repeat_Cmd:public CC_Command
{
 public:
  /// Constructor.
  CC_Repeat_Cmd(int times);

  /// Destructor.
  virtual ~CC_Repeat_Cmd();

  /// Executes the command.
  virtual int execute(void);
 private:
  /// The number of times the commands should be repeated
  int times_;
};

/**
 * @class CC_Wait_Cmd:public
 *
 * @brief Defines a class for the wait command
 *
 * This class represents the wait command. This command is used to make
 * the script wait for the user to press return. It is possible to print
 * different prompts, e.g. instructions.
 */
class CC_Wait_Cmd:public CC_Command
{
 public:
  /// Constructor.
  CC_Wait_Cmd (const char *prompt);

  /// Destructor.
  virtual ~CC_Wait_Cmd();

  /// Executes the command.
  virtual int execute(void);

private:
  /// The prompt to print on the screen
  char *prompt_;
};

/**
 * @class CC_Excep_Cmd
 *
 * @brief Defines a class for the excep command
 *
 * This class represents the excep command. This command is used to make
 * the script capable of dealing with cases where an exception is raised
 * as part of the testing.
 */
class CC_Excep_Cmd : public CC_Command
{
 public:
  /// Constructor.
  CC_Excep_Cmd (const char *excep);

  /// Destructor.
  virtual ~CC_Excep_Cmd(void);

  /**
   * Executes the command. Checks to see if the excep_ class variable is set,
   * and if that's the case check that it is of the expected type. If not the
   * test fails.
   */
  virtual int execute(void);

private:
  /// The string representation of the expected exception
  char *ex_;
};

/**
 * @class CC_Dummy_Cmd:
 *
 * @brief Defines a class for the dummy command
 *
 * This class represents the dummy command. This command is used to
 * put in a command in the chain that does nothing.
 */
class CC_Dummy_Cmd: public CC_Command
{
 public:
  /// Constructor.
  CC_Dummy_Cmd(void);

  /// Destructor.
  virtual ~CC_Dummy_Cmd(void);

  /// Executes the command, i.e. does nothing.
  virtual int execute(void);

 private:
};

/**
 * @class CC_Print_Cmd:
 *
 * @brief Defines a class for the print command
 *
 * This class represents the print command. This command is used to
 * print a message on stdout
 */
class CC_Print_Cmd: public CC_Command
{
public:
  /// Constructor.
  CC_Print_Cmd (const char *message);

  /// Destructor.
  virtual ~CC_Print_Cmd(void);

  /// Executes the command.
  virtual int execute(void);

private:
  /// Holds the message to print
  char *msg_;
};

/**
 * @class CC_Lookup_Cmd:public
 *
 * @brief Defines a class for the lookup command.
 *
 * This class represents the lookup command. The lock set
 * is looked up in the naming service and the class variable
 * cc_lockset_ is set accordingly.
 */
class CC_Lookup_Cmd:public CC_Command
{
public:
  /// Constructor
  CC_Lookup_Cmd (const char *lock_set_name);

  /// Destructor
  virtual ~CC_Lookup_Cmd();

  /// Executes the command, i.e. looks up the lock set with the requested
  /// name in the naming server and sets the cc_lockset_ variable.
  virtual int execute(void);

private:
  /// The name to look up in the naming service.
  char *name_;

};

/**
 * @class CC_CommandElem
 *
 * @brief Defines a class for a command element
 *
 * This class implements a command element used in the command list
 * below. The class is a simple tupple holding a pointer to the command
 * and a pointer to the next element in the list.
 */
class CC_CommandElem
{
 public:
  /// Constructor.
  CC_CommandElem (CC_Command *cmd, CC_CommandElem *next);

  /// Destructor.
  ~CC_CommandElem(void);

  /// Returns a pointer to the command in this element
  CC_Command *GetCommand(void);

  /// Returns the pointer to the next element
  CC_CommandElem *GetNext(void);

  /// Sets the next pointer
  void SetNext(CC_CommandElem *next);

 private:
  /// Pointer to the next element
  CC_CommandElem *next_;

  /// Pointer to the command element
  CC_Command *cmd_;
};

/**
 * @class CC_CommandList
 *
 * @brief Defines a class for a command list
 *
 * This class implements a command list used from the script parser
 * to store the parsed commands The class implements a simple single
 * linked list.
 */
class CC_CommandList
{
 public:
  /// Constructor.
  CC_CommandList(void);

  /// Destructor.
  ~CC_CommandList(void);

  /// Adds the command to the list
  int add(CC_Command *cmd);

  /// Executes all the commands in the list from head to tail
  int execute(void);

  /// Sets the number of times to repeat the script
  void setrepeat(int times);

 private:
  ///  The head of the command list
  CC_CommandElem *head_;

  /// The last inserted element
  CC_CommandElem *last_;

  /// The number of times the script should be repeated
  int times_;
};

#endif /* _CC_COMMAND_H_ */
