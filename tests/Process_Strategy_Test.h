/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Process_Strategy_Test.h
 *
 *  $Id$
 *
 *  This file contains the definition of Counting_Service and
 *  Options.  Some compilers need it in a .h file for template
 *  instantiation (such as AIX C Set ++).
 *
 *
 *  @author Doug Schmidt <schmidt@cs.wustl.edu> and Kevin Boyle <kboyle@sanwafp.com>
 */
//=============================================================================


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

/**
 * @class Counting_Service
 *
 * @brief Reads and increments the count in a shared file.
 *
 * Objects of this class execute in a separate process as a
 * result of the <ACE_Strategy_Acceptor> and
 * <ACE_Process_Strategy>.
 */
class Counting_Service : public ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  /// Constructor.
  Counting_Service (ACE_Thread_Manager * = 0);

  /// Hook that is used to initialize the service (called by the
  /// <ACE_Strategy_Acceptor::handle_input> Template Method).
  virtual int open (void *v);

protected:
  // = Methods invoked via "pointer to method" table entry.

  /// Handle the THREAD case.
  virtual int svc (void);

  // = Operations corresponding to requests from the client.
  /// Execute the read operation on the file.
  int read (void);

  /// Execute the increment operation on the file.
  int inc (void);

  // = Hooks called by <Reactor> and <Strategy_Acceptor>.

  /// Hook called by the <Reactor> when data arrives from the client.
  virtual int handle_input (ACE_HANDLE p = ACE_INVALID_HANDLE);

  /// Closing down
  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                            ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK);
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

/**
 * @class Options
 *
 * @brief Maintains the options for this program.
 */
class Options : public ACE_Event_Handler
{
public:
  /// Constructor.
  Options (void);

  /// Destructor.
  ~Options (void);

  /// Read command-line arguments and initialize options.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  enum Concurrency_Type
  {
    PROCESS,  // Run the test in separate processes.
    REACTIVE, // Run the test reactively in one thread.
    THREAD    // Run the test as in separate threads.
  };

  // = Get/set concurrency type.
  Concurrency_Type concurrency_type (void);
  void concurrency_type (Concurrency_Type);

  /// Returns the file lock.
  ACE_File_Lock &file_lock (void);

  /// Returns the filename that we're using as the lock.
  const ACE_TCHAR *filename (void);

  /// Returns the concurrency strategy.
  ACE_Concurrency_Strategy <Counting_Service> *concurrency_strategy (void);

private:
  /// Concurrency strategy that we're running.
  Concurrency_Type concurrency_type_;

  /// Lock for the counting file.
  ACE_File_Lock file_lock_;

  /// Activation strategy that either forks a new process or spawns a
  /// new thread for each client connection.
  ACE_Concurrency_Strategy<Counting_Service> *concurrency_strategy_;

  /// Name of the counting file.
  ACE_TCHAR filename_[MAXPATHLEN + 1];
};

#endif /* ACE_TESTS_PROCESS_STRATEGY_TEST_H */
