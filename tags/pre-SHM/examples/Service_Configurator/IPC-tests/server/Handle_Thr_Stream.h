/* -*- C++ -*- */
// $Id$

#ifndef _HANDLE_THR_STREAM_H
#define _HANDLE_THR_STREAM_H

#include "ace/Acceptor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Types.h"

#if defined (ACE_HAS_THREADS)

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1>
class Handle_Thr_Acceptor : public ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>
  // = TITLE
  //   This factory creates new <SVC_HANDLERS> and runs them with the
  //   configured <ACE_Thread_Strategy>.
{
public:
  // = Initialization and termination.
  Handle_Thr_Acceptor (void);
  ~Handle_Thr_Acceptor (void);

  // = Dynamic linking hooks.
  virtual int init (int argc, char *argv[]);
  virtual int info (char **, size_t) const;
  virtual int fini (void);

private:
  ACE_Thread_Manager thr_mgr_;
  // Thread manager.

  ACE_Thread_Strategy<SVC_HANDLER> thr_strategy_;
  // Threading strategy.

  int thr_flags_;
  // Threading flags.
};

template <ACE_PEER_STREAM_1>
class CLI_Stream : public ACE_Svc_Handler<ACE_PEER_STREAM_2, ACE_MT_SYNCH>
  // = TITLE
  //   This class interacts with the client, running in a separate
  //   thread and handles connections from remote TCP/IP connections.
{
public:
  CLI_Stream (ACE_Thread_Manager * = 0);

  virtual int open (void *);
  // Open the service.

  virtual int close (u_long);
  // Close down the service.

  virtual int svc (void);
  // Execute the service.

protected:
  typedef ACE_Svc_Handler<ACE_PEER_STREAM_2, ACE_MT_SYNCH> inherited;
};

extern ACE_Service_Object_Type rts;

#if defined (__ACE_INLINE__)
#define ACE_INLINE inline
#include "Handle_Thr_Stream.i"
#else
#define ACE_INLINE
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Handle_Thr_Stream.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */
#endif /* ACE_HAS_THREADS */
#endif /* _HANDLE_THR_STREAM_H */
