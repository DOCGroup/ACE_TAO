/* -*- C++ -*- */
// $Id$

// ============================================================================
// = FILENAME
//    Process_Strategy_Test.h
//
// = DESCRIPTION
//    This file contains the definition of Counting_Service and Options.
//    Some compilers need it in a .h file for template instantiation
//    (such as AIX C Set ++)
//
// = AUTHOR
//    Doug Schmidt and Kevin Boyle <kboyle@sanwafp.com>
// 
// ============================================================================

#ifndef PROCESS_STRATEGY_TEST_H
#define PROCESS_STRATEGY_TEST_H

#include "ace/Event_Handler.h"
#include "ace/Svc_Handler.h"
class ACE_Thread_Manager;

typedef ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH> SVC_HANDLER;

class Counting_Service : public SVC_HANDLER
  // = TITLE
  //     Reads and increments the count in a shared file.
  //
  // = DESCRIPTION
  //     Objects of this class execute in a separate process as a
  //     result of the <ACE_Strategy_Acceptor> and
  //     <ACE_Process_Strategy>.
{
public:
  Counting_Service (ACE_Thread_Manager * = 0);
  // Constructor.

  virtual int open (void *v);
  // Hook that is used to initialize the service (called by the
  // <ACE_Strategy_Acceptor::handle_input> Template Method).

protected:
  // = Methods invoked via "pointer to method" table entry.

  virtual int svc (void);
  // Handle the THREAD case.

  // = Operations corresponding to requests from the client.
  int read (void);
  // Execute the read operation on the file.

  int inc (void);
  // Execute the increment operation on the file.

  // = Hooks called by <Reactor> and <Strategy_Acceptor>.

  virtual int handle_input (ACE_HANDLE p = ACE_INVALID_HANDLE);
  // Hook called by the <Reactor> when data arrives from the client.
};



class Options : public ACE_Event_Handler
  // = TITLE
  //     Maintains the options for this program.
{
public:
  Options (void);
  // Constructor.

  ~Options (void);
  // Destructor.

  int parse_args (int argc, char *argv[]);

  enum Concurrency_Type
  {
    PROCESS,
    REACTIVE,
    THREAD
  };

  // = Get/set concurrency type.
  Concurrency_Type concurrency_type (void);
  void concurrency_type (Concurrency_Type);

  ACE_File_Lock &file_lock (void);
  // Returns the file lock.

  const char *filename (void);
  // Returns the filename.

  ACE_Concurrency_Strategy <Counting_Service> *concurrency_strategy (void);
  // Returns the concurrency strategy.

  virtual int handle_signal (int, siginfo_t *, ucontext_t *);
  // Catch the SIGCHLD signal and reap the exiting child processes.

private:
  Concurrency_Type concurrency_type_;
  // Concurrency strategy that we're running.

  ACE_File_Lock file_lock_;
  // Lock for the counting file.

  ACE_Concurrency_Strategy<Counting_Service> *concurrency_strategy_;
  // Activation strategy that either forks a new process or spawns a
  // new thread for each client connection.

  const char *filename_;
  // Name of the counting file.
};


#endif /* PROCESS_STRATEGY_TEST_H */
