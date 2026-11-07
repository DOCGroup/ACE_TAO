/*
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#ifndef _TP_LOGGING_SERVER_H
#define _TP_LOGGING_SERVER_H

#include "ace/Singleton.h"
#include "ace/Synch.h"
#include "ace/Task.h"
#include "Logging_Acceptor.h"
#include "Logging_Event_Handler.h"
#include "Reactor_Logging_Server_T.h"
#include "TPLS_export.h"
#include <memory>

class TP_Logging_Task : public ACE_Task<ACE_SYNCH> {
         // Instantiated with an MT synchronization trait.
public:
  enum { MAX_THREADS = 4 };

  int open (void * = nullptr) override
  { return activate (THR_NEW_LWP, MAX_THREADS); }

  int put (ACE_Message_Block *mblk,
           ACE_Time_Value *timeout = nullptr) override
  { return putq (mblk, timeout); }

  int svc () override;
};

using TP_LOGGING_TASK = ACE_Unmanaged_Singleton<TP_Logging_Task, ACE_Null_Mutex>;

/*******************************************************/

class TP_Logging_Acceptor;

class TP_Logging_Handler : public Logging_Event_Handler {
  friend class TP_Logging_Acceptor;
protected:
  ~TP_Logging_Handler () override = default;

  // Number of pointers to this class instance that currently
  // reside in the <TP_LOGGING_TASK> singleton's message queue.
  int queued_count_;

  // Indicates whether <Logging_Event_Handler::handle_close()>
  // must be called to cleanup and delete this object.
  bool deferred_close_;

  // Serialize access to <queued_count_> and <deferred_close_>.
  ACE_SYNCH_MUTEX lock_;

public:
  explicit TP_Logging_Handler (ACE_Reactor *reactor)
    : Logging_Event_Handler (reactor),
      queued_count_ (0),
      deferred_close_ (false) {}

  TP_Logging_Handler (const TP_Logging_Handler &) = delete;
  TP_Logging_Handler (TP_Logging_Handler &&) = delete;
  TP_Logging_Handler &operator= (const TP_Logging_Handler &) = delete;
  TP_Logging_Handler &operator= (TP_Logging_Handler &&) = delete;

  // Called when input events occur, e.g., connection or data.
  int handle_input (ACE_HANDLE) override;

  // Called when this object is destroyed, e.g., when it's
  // removed from a reactor.
  int handle_close (ACE_HANDLE, ACE_Reactor_Mask) override;
};

/*******************************************************/

class TP_Logging_Acceptor : public Logging_Acceptor {
public:
  TP_Logging_Acceptor (ACE_Reactor *r = ACE_Reactor::instance ())
    : Logging_Acceptor (r) {}

  int handle_input (ACE_HANDLE) override {
    TP_Logging_Handler *peer_handler = nullptr;
    ACE_NEW_RETURN (peer_handler,
                    TP_Logging_Handler (reactor ()), -1);
    if (acceptor_.accept (peer_handler->peer ()) == -1) {
      delete peer_handler;
      return -1;
    } else if (peer_handler->open () == -1)
      peer_handler->handle_close (ACE_INVALID_HANDLE, 0);
    return 0;
  }
};

/*******************************************************/

class TP_Logging_Server : public ACE_Service_Object {
protected:
  // Contains the reactor, acceptor, and handlers.
  using LOGGING_DISPATCHER = Reactor_Logging_Server<TP_Logging_Acceptor>;
  std::unique_ptr<LOGGING_DISPATCHER> logging_dispatcher_;

public:
  TP_Logging_Server () = default;
  TP_Logging_Server (const TP_Logging_Server &) = delete;
  TP_Logging_Server (TP_Logging_Server &&) = delete;
  TP_Logging_Server &operator= (const TP_Logging_Server &) = delete;
  TP_Logging_Server &operator= (TP_Logging_Server &&) = delete;

  int init (int argc, ACE_TCHAR *argv[]) override {
    int i;
    char **array = nullptr;
    ACE_NEW_RETURN (array, char*[argc], -1);
    std::unique_ptr<char *[]> char_argv (array);

    for (i = 0; i < argc; ++i)
      char_argv[i] = ACE::strnew (ACE_TEXT_ALWAYS_CHAR (argv[i]));
    LOGGING_DISPATCHER *dispatcher = nullptr;
    ACE_NEW_NORETURN
      (dispatcher,
       TP_Logging_Server::LOGGING_DISPATCHER
       (i, char_argv.get (), ACE_Reactor::instance ()));
    logging_dispatcher_.reset (dispatcher);
    for (i = 0; i < argc; ++i) ACE::strdelete (char_argv[i]);
    if (!logging_dispatcher_) return -1;
    else return TP_LOGGING_TASK::instance ()->open ();
  }

  int fini () override {
    TP_LOGGING_TASK::instance ()->flush ();
    TP_LOGGING_TASK::instance ()->wait ();
    TP_LOGGING_TASK::close ();
    logging_dispatcher_.reset ();
    return 0;
  }
};

ACE_FACTORY_DECLARE (TPLS, TP_Logging_Handler)

#endif /* _TP_LOGGING_SERVER_H */
