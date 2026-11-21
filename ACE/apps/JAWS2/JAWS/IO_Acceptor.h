/* -*- c++ -*- */
#ifndef JAWS_IO_ACCEPTOR_H
#define JAWS_IO_ACCEPTOR_H

#include "ace/Asynch_Acceptor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/LOCK_SOCK_Acceptor.h"
#include "ace/Singleton.h"
#include <memory>

#include "JAWS/Export.h"
#include "JAWS/Jaws_IO.h"
#include "JAWS/IO_Handler.h"

// Forward declaration.
ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Proactor;
class ACE_Reactor;
ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (JAWS_HAS_THREAD_SAFE_ACCEPT)
using JAWS_IO_SOCK_Acceptor = ACE_LOCK_SOCK_Acceptor<ACE_SYNCH_NULL_MUTEX>;
#else
using JAWS_IO_SOCK_Acceptor = ACE_LOCK_SOCK_Acceptor<ACE_SYNCH_MUTEX>;
#endif /* JAWS_HAS_THREAD_SAFE_ACCEPT */

/// Adapts synchronous and asynchronous socket acceptors to one interface.
class JAWS_Export JAWS_IO_Acceptor
{
public:
  JAWS_IO_Acceptor ();
  virtual ~JAWS_IO_Acceptor ();
  JAWS_IO_Acceptor (JAWS_IO_Acceptor const &) = delete;
  JAWS_IO_Acceptor (JAWS_IO_Acceptor &&) = delete;
  JAWS_IO_Acceptor &operator= (JAWS_IO_Acceptor const &) = delete;
  JAWS_IO_Acceptor &operator= (JAWS_IO_Acceptor &&) = delete;

  /// Open a passive socket listener.
  virtual int open (const ACE_INET_Addr &address, int backlog = 20);

  /// Adopt a listener socket.
  virtual int open (const ACE_HANDLE &socket);

  /// Accept a connection synchronously.
  virtual int accept (ACE_SOCK_Stream &new_stream,
                      ACE_Addr *remote_addr = nullptr,
                      ACE_Time_Value *timeout = nullptr,
                      int restart = 1,
                      int reset_new_handle = 0) const;

  /// Initiate an asynchronous accept.
  virtual int accept (size_t bytes_to_read = 0, void const *act = nullptr);

  /// Return the listener's handle.
  virtual ACE_HANDLE get_handle ();

  /// Close the acceptor.
  virtual void close ();

  /// Identify whether the acceptor uses asynchronous or synchronous I/O.
  enum { ASYNC = 0, SYNCH = 1 };
};

class JAWS_Export JAWS_IO_Synch_Acceptor : public JAWS_IO_Acceptor
{
public:
  JAWS_IO_Synch_Acceptor () = default;
  JAWS_IO_Synch_Acceptor (JAWS_IO_Synch_Acceptor const &) = delete;
  JAWS_IO_Synch_Acceptor (JAWS_IO_Synch_Acceptor &&) = delete;
  JAWS_IO_Synch_Acceptor &operator= (JAWS_IO_Synch_Acceptor const &) = delete;
  JAWS_IO_Synch_Acceptor &operator= (JAWS_IO_Synch_Acceptor &&) = delete;

  /// Open a passive mode socket.
  int open (const ACE_INET_Addr &local_sap, int backlog = 20) override;

  /// Adopt a listener socket.
  int open (const ACE_HANDLE &socket) override;

  int accept (ACE_SOCK_Stream &new_stream,
              ACE_Addr *remote_addr = nullptr,
              ACE_Time_Value *timeout = nullptr,
              int restart = 1,
              int reset_new_handle = 0) const override;

  /// Return the listener's handle.
  ACE_HANDLE get_handle () override;

private:
  int accept (size_t bytes_to_read = 0, void const *act = nullptr) override;

private:
  JAWS_IO_SOCK_Acceptor acceptor_;
};

class JAWS_Export JAWS_IO_Asynch_Acceptor : public JAWS_IO_Acceptor
{
public:
  JAWS_IO_Asynch_Acceptor ();
  ~JAWS_IO_Asynch_Acceptor () override;
  JAWS_IO_Asynch_Acceptor (JAWS_IO_Asynch_Acceptor const &) = delete;
  JAWS_IO_Asynch_Acceptor (JAWS_IO_Asynch_Acceptor &&) = delete;
  JAWS_IO_Asynch_Acceptor &operator= (JAWS_IO_Asynch_Acceptor const &) = delete;
  JAWS_IO_Asynch_Acceptor &operator= (JAWS_IO_Asynch_Acceptor &&) = delete;

  /// Open an asynchronous passive listener.
  int open (const ACE_INET_Addr &address, int backlog = 20) override;

  /// Adopt a listener socket.
  int open (const ACE_HANDLE &socket) override;

  /// Initiate an asynchronous accept through AcceptEx.
  int accept (size_t bytes_to_read = 0, void const *act = nullptr) override;

  /// Return the listener's handle.
  ACE_HANDLE get_handle () override;

  /// Close the acceptor.
  void close () override;

private:
  int accept (ACE_SOCK_Stream &new_stream,
              ACE_Addr *remote_addr = nullptr,
              ACE_Time_Value *timeout = nullptr,
              int restart = 1,
              int reset_new_handle = 0) const override;

private:
#if defined (ACE_HAS_WIN32_OVERLAPPED_IO) || defined (ACE_HAS_AIO_CALLS)
  /// Owns the asynchronous acceptor.
  std::unique_ptr<ACE_Asynch_Acceptor<JAWS_Asynch_Handler>> acceptor_;
  ACE_HANDLE handle_ {ACE_INVALID_HANDLE};
#endif /* ACE_HAS_WIN32_OVERLAPPED_IO || ACE_HAS_AIO_CALLS */
};

using JAWS_IO_Synch_Acceptor_Singleton = ACE_Singleton<JAWS_IO_Synch_Acceptor, ACE_SYNCH_MUTEX>;

using JAWS_IO_Asynch_Acceptor_Singleton = ACE_Singleton<JAWS_IO_Asynch_Acceptor, ACE_SYNCH_MUTEX>;

#endif /* !defined (JAWS_IO_ACCEPTOR_H) */
