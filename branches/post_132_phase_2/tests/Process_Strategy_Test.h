/* -*- C++ -*- */
// $Id$

// ============================================================================
// = FILENAME
//    Process_Strategy_Test.h
//
// = DESCRIPTION
//    This file contains the definition of Counting_Service and
//    Options.  Some compilers need it in a .h file for template
//    instantiation (such as AIX C Set ++).
//
// = AUTHOR
//    Doug Schmidt <schmidt@cs.wustl.edu> and 
//    Kevin Boyle <kboyle@sanwafp.com>
//
// ============================================================================

#ifndef ACE_TESTS_PROCESS_STRATEGY_TEST_H
#define ACE_TESTS_PROCESS_STRATEGY_TEST_H

#include "ace/Event_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/File_Lock.h"
#include "ace/SOCK_Stream.h"
#include "ace/Svc_Handler.h"
#include "ace/Strategies_T.h"

// Forward decl.
class ACE_Thread_Manager;

class Counting_Service : public ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH>
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

  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                            ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK);
  // Closing down
};

class Process_Strategy : public ACE_Process_Strategy<Counting_Service>
{
public:

  // Constructor
  Process_Strategy (size_t n_processes = 1,
                    ACE_Event_Handler *acceptor = 0,
                    ACE_Reactor * = 0,
                    int flags = 0);

  // Destructor
  ~Process_Strategy (void);

  // Overwrite the process creation method to include connection
  // counting
  virtual int activate_svc_handler (Counting_Service *svc_handler,
                                    void *arg = 0);
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

  int parse_args (int argc, ACE_TCHAR *argv[]);
  // Read command-line arguments and initialize options.

  enum Concurrency_Type
  {
    PROCESS,  // Run the test in separate processes.
    REACTIVE, // Run the test reactively in one thread.
    THREAD    // Run the test as in separate threads.
  };

  // = Get/set concurrency type.
  Concurrency_Type concurrency_type (void);
  void concurrency_type (Concurrency_Type);

  ACE_File_Lock &file_lock (void);
  // Returns the file lock.

  const ACE_TCHAR *filename (void);
  // Returns the filename that we're using as the lock.

  ACE_Concurrency_Strategy <Counting_Service> *concurrency_strategy (void);
  // Returns the concurrency strategy.

private:
  Concurrency_Type concurrency_type_;
  // Concurrency strategy that we're running.

  ACE_File_Lock file_lock_;
  // Lock for the counting file.

  ACE_Concurrency_Strategy<Counting_Service> *concurrency_strategy_;
  // Activation strategy that either forks a new process or spawns a
  // new thread for each client connection.

  ACE_TCHAR filename_[MAXPATHLEN + 1];
  // Name of the counting file.
};

#endif /* ACE_TESTS_PROCESS_STRATEGY_TEST_H */
