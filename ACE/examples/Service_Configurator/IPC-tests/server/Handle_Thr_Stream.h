/* -*- C++ -*- */
#ifndef _HANDLE_THR_STREAM_H
#define _HANDLE_THR_STREAM_H

#include "ace/Acceptor.h"
#include "ace/Svc_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Types.h"

#if defined (ACE_HAS_THREADS)

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1>
/**
 * @brief Creates SVC_HANDLER objects and runs them with ACE_Thread_Strategy.
 */
class Handle_Thr_Acceptor : public ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>
{
public:
  Handle_Thr_Acceptor () = default;
  ~Handle_Thr_Acceptor ();
  Handle_Thr_Acceptor (Handle_Thr_Acceptor const &) = delete;
  Handle_Thr_Acceptor (Handle_Thr_Acceptor &&) = delete;
  Handle_Thr_Acceptor &operator= (Handle_Thr_Acceptor const &) = delete;
  Handle_Thr_Acceptor &operator= (Handle_Thr_Acceptor &&) = delete;

  /// @name Dynamic linking hooks
  /// @{
  int init (int argc, ACE_TCHAR *argv[]) override;
  int info (ACE_TCHAR **, size_t) const override;
  int fini () override;
  /// @}

private:
  /// Thread manager.
  ACE_Thread_Manager thr_mgr_;

  /// Threading strategy.
  ACE_Thread_Strategy<SVC_HANDLER> thr_strategy_;

  /// Threading flags.
  int thr_flags_ { THR_DETACHED | THR_NEW_LWP };
};

template <ACE_PEER_STREAM_1>
/**
 * @brief Interacts with clients on separate threads and handles remote
 * TCP/IP connections.
 */
class CLI_Stream : public ACE_Svc_Handler<ACE_PEER_STREAM_2, ACE_MT_SYNCH>
{
public:
  explicit CLI_Stream (ACE_Thread_Manager * = nullptr);
  CLI_Stream (CLI_Stream const &) = delete;
  CLI_Stream (CLI_Stream &&) = delete;
  CLI_Stream &operator= (CLI_Stream const &) = delete;
  CLI_Stream &operator= (CLI_Stream &&) = delete;

  /// Opens the service.
  int open (void *) override;

  /// Closes the service.
  int close (u_long) override;

  /// Executes the service.
  int svc () override;

protected:
  using inherited = ACE_Svc_Handler<ACE_PEER_STREAM_2, ACE_MT_SYNCH>;
};

extern ACE_Service_Object_Type rts;

#include "Handle_Thr_Stream.cpp"
#endif /* ACE_HAS_THREADS */
#endif /* _HANDLE_THR_STREAM_H */
