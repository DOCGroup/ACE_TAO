/* -*- c++ -*- */
// $Id$

#if !defined (JAWS_IO_ACCEPTOR_H)
#define JAWS_IO_ACCEPTOR_H

// Use the Adapter pattern to encapsulate either a LOCK_SOCK_Acceptor or
// an ACE_Asynch_Acceptor

#include "ace/Asynch_Acceptor.h"
#include "ace/LOCK_SOCK_Acceptor.h"

#include "JAWS/IO.h"

// Forward declaration.
class ACE_Proactor;
class ACE_Reactor;

#if defined (ACE_HAS_THREAD_SAFE_ACCEPT)
typedef ACE_LOCK_SOCK_Acceptor<ACE_SYNCH_NULL_MUTEX> JAWS_IO_SOCK_Acceptor;
#else
typedef ACE_LOCK_SOCK_Acceptor<ACE_SYNCH_MUTEX> JAWS_IO_SOCK_Acceptor;
#endif /* ACE_HAS_THREAD_SAFE_ACCEPT */

class JAWS_IO_Acceptor
{
public:

  JAWS_IO_Acceptor (void);
  virtual ~JAWS_IO_Acceptor (void);

  virtual int open (const ACE_Addr &local_sap,
                    int reuse_addr = 0,
                    int protocol_family = PF_INET,
                    int backlog = 5,
                    int protocol = 0) = 0;
  // Initiate a synchronous passive mode socket.

  virtual int accept (ACE_SOCK_Stream &new_stream,
                      ACE_Addr *remote_addr = 0,
                      ACE_Time_Value *timeout = 0,
                      int restart = 1,
                      int reset_new_handle = 0) const = 0;
  // Synchronously accept the connection

  virtual int open (const ACE_INET_Addr &address,
                    size_t bytes_to_read = 0,
                    int pass_addresses = 0,
                    int backlog = 5,
                    int reuse_addr = 1,
                    ACE_Proactor *proactor = 0) = 0;
  // Initiate an asynchronous passive connection

  virtual int accept (size_t bytes_to_read = 0) = 0;
  // This initiates a new asynchronous accept through the AcceptEx call.

  enum { ASYNC = 0, SYNCH = 1 };
  // identify if this is being used for aynchronous or synchronous
  // accept calls

private:

};

class JAWS_IO_Synch_Acceptor : public JAWS_IO_Acceptor
{
public:

  virtual int open (const ACE_Addr &local_sap,
                    int reuse_addr = 0,
                    int protocol_family = PF_INET,
                    int backlog = 5,
                    int protocol = 0);
  // Initiate a passive mode socket.

  virtual int accept (ACE_SOCK_Stream &new_stream,
                      ACE_Addr *remote_addr = 0,
                      ACE_Time_Value *timeout = 0,
                      int restart = 1,
                      int reset_new_handle = 0) const;
  // Accept the connection

private:
  JAWS_IO_SOCK_Acceptor *acceptor_;
};


#if defined (ACE_WIN32)
// This only works on Win32 platforms

class JAWS_IO_Asynch_Acceptor : public JAWS_IO_Acceptor
{
public:

  virtual int open (const ACE_INET_Addr &address,
                    size_t bytes_to_read = 0,
                    int pass_addresses = 0,
                    int backlog = 5,
                    int reuse_addr = 1,
                    ACE_Proactor *proactor = 0);
  // Initiate an asynchronous passive connection

  virtual int accept (size_t bytes_to_read = 0);
  // This initiates a new asynchronous accept through the AcceptEx call.

private:
  ACE_Asynch_Acceptor<JAWS_IO_Handler> *acceptor_;
};

#endif /* defined (ACE_WIN32) */

#endif /* !defined (JAWS_IO_ACCEPTOR_H) */
