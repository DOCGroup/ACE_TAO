/* -*- c++ -*- */
// $Id$

#if !defined (JAWS_IO_ACCEPTOR_H)
#define JAWS_IO_ACCEPTOR_H

// Use the Adapter pattern to encapsulate either a LOCK_SOCK_Acceptor or
// an ACE_Asynch_Acceptor

#include "ace/Asynch_Acceptor.h"
#include "ace/LOCK_SOCK_Acceptor.h"
#include "ace/Singleton.h"

#include "JAWS/Export.h"
#include "JAWS/IO.h"
#include "JAWS/IO_Handler.h"

// Forward declaration.
class ACE_Proactor;
class ACE_Reactor;

#if defined (JAWS_HAS_THREAD_SAFE_ACCEPT)
typedef ACE_LOCK_SOCK_Acceptor<ACE_SYNCH_NULL_MUTEX> JAWS_IO_SOCK_Acceptor;
#else
typedef ACE_LOCK_SOCK_Acceptor<ACE_SYNCH_MUTEX> JAWS_IO_SOCK_Acceptor;
#endif /* JAWS_HAS_THREAD_SAFE_ACCEPT */

class JAWS_Export JAWS_IO_Acceptor
{
public:

  JAWS_IO_Acceptor (void);
  virtual ~JAWS_IO_Acceptor (void);

  virtual int open (const ACE_INET_Addr &address);
  // Initiate a passive mode socket.

  virtual int accept (ACE_SOCK_Stream &new_stream,
                      ACE_Addr *remote_addr = 0,
                      ACE_Time_Value *timeout = 0,
                      int restart = 1,
                      int reset_new_handle = 0) const;
  // Synchronously accept the connection

  virtual int accept (size_t bytes_to_read = 0);
  // This initiates a new asynchronous accept through the AcceptEx call.

  enum { ASYNC = 0, SYNCH = 1 };
  // identify if this is being used for aynchronous or synchronous
  // accept calls

private:

};

class JAWS_Export JAWS_IO_Synch_Acceptor : public JAWS_IO_Acceptor
{
public:

  virtual int open (const ACE_INET_Addr &local_sap);
  // Initiate a passive mode socket.

  virtual int accept (ACE_SOCK_Stream &new_stream,
                      ACE_Addr *remote_addr = 0,
                      ACE_Time_Value *timeout = 0,
                      int restart = 1,
                      int reset_new_handle = 0) const;
  // Accept the connection

private:
  JAWS_IO_SOCK_Acceptor acceptor_;
};


class JAWS_Export JAWS_IO_Asynch_Acceptor : public JAWS_IO_Acceptor
{
public:

  virtual int open (const ACE_INET_Addr &address);
  // Initiate an asynchronous passive connection

  virtual int accept (size_t bytes_to_read = 0);
  // This initiates a new asynchronous accept through the AcceptEx call.

private:

#if defined (ACE_WIN32)
// This only works on Win32 platforms
  ACE_Asynch_Acceptor<JAWS_Asynch_IO_Handler_Factory> acceptor_;
#endif /* defined (ACE_WIN32) */
};

typedef ACE_Singleton<JAWS_IO_Synch_Acceptor, ACE_SYNCH_MUTEX>
        JAWS_IO_Synch_Acceptor_Singleton;

typedef ACE_Singleton<JAWS_IO_Asynch_Acceptor, ACE_SYNCH_MUTEX>
        JAWS_IO_Asynch_Acceptor_Singleton;

#endif /* !defined (JAWS_IO_ACCEPTOR_H) */
