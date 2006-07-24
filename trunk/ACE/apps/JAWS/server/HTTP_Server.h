// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//   jaws
//
// = FILENAME
//    HTTP_Server.h
//
// = AUTHOR
//    James Hu
//
// ============================================================================

#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"
#include "ace/Service_Object.h"
#include "ace/Thread_Manager.h"
#include "ace/Acceptor.h"
#include "ace/LOCK_SOCK_Acceptor.h"
#include "ace/Task_T.h"
#include "ace/Asynch_IO.h"
#include "ace/svc_export.h"
#include "HTTP_Handler.h"
#include "ace/Synch_Traits.h"
#include "ace/Thread_Mutex.h"
#include "ace/Null_Mutex.h"
#include "ace/Global_Macros.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
// Forward declaration.
class ACE_Proactor;
ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_HAS_THREAD_SAFE_ACCEPT)
typedef ACE_LOCK_SOCK_Acceptor<ACE_SYNCH_NULL_MUTEX> HTTP_SOCK_Acceptor;
#else
typedef ACE_LOCK_SOCK_Acceptor<ACE_SYNCH_MUTEX> HTTP_SOCK_Acceptor;
#endif /* ACE_HAS_THREAD_SAFE_ACCEPT */

typedef HTTP_SOCK_Acceptor HTTP_Acceptor;

class ACE_Svc_Export HTTP_Server : public ACE_Service_Object
  // = TITLE
  //     This server is used to create HTTP Handlers for the Web
  //     server
  //
  // = DESCRIPTION
{
public:
  virtual int init (int argc, ACE_TCHAR *argv[]);
  // Initialization

  virtual int fini (void);
  // Exit hooks

protected:
  virtual int thread_per_request (HTTP_Handler_Factory &factory);
  // Thread Per Request implementation

  virtual int asynch_thread_pool (void);
  // Asynch Thread Pool implementation

  virtual int synch_thread_pool (HTTP_Handler_Factory &factory);
  // Synch Thread Pool implementation

private:
  // James, comment these data members.
  void parse_args (int argc, ACE_TCHAR **argv);
  int port_;
  int threads_;
  int strategy_;
  int backlog_;
  int throttle_;
  bool caching_;
  ACE_Thread_Manager tm_;
  HTTP_Acceptor acceptor_;
};

class Synch_Thread_Pool_Task : public ACE_Task<ACE_NULL_SYNCH>
  // = TITLE
  //     Used to implement Synch Thread Pool
  //
  // = DESCRIPTION
  //     Describe this and the others below.
  //	 NOTE: this class was modified to make caching disabling possible
{
public:
  Synch_Thread_Pool_Task (HTTP_Acceptor &acceptor,
                          ACE_Thread_Manager &tm,
                          int threads,
						  HTTP_Handler_Factory &factory);
  virtual int svc (void);

private:
  HTTP_Acceptor &acceptor_;
  HTTP_Handler_Factory &factory_;
};

class Thread_Per_Request_Task : public ACE_Task<ACE_NULL_SYNCH>
  // = TITLE
  //     Used to implement Thread Per Request.
  //
  // = DESCRIPTION
  //     Spawns a new thread for every new incoming connection.  The
  //     handle below is the socket stream of the incoming connection.
  //	 NOTE: this class was modified to make caching disabling possible
{
public:
  Thread_Per_Request_Task (ACE_HANDLE handle,
                           ACE_Thread_Manager &tm,
                           int &grp_id,
						   HTTP_Handler_Factory &factory);
  virtual int open (void *args = 0);
  virtual int close (u_long);
  virtual int svc (void);

private:
  ACE_HANDLE handle_;
  int &grp_id_;
  HTTP_Handler_Factory &factory_;
};

// This only works on Win32
#if defined (ACE_WIN32)
class Asynch_Thread_Pool_Task : public ACE_Task<ACE_NULL_SYNCH>
  // = TITLE
  //     Used to implement Asynch Thread Pool
  //
  // = DESCRIPTION
  //     The proactor below utilizes WaitForMultipleObjects.
{
public:
  Asynch_Thread_Pool_Task (ACE_Proactor &proactor,
                           ACE_Thread_Manager &tm);
  virtual int svc (void);

private:
  ACE_Proactor &proactor_;
};
#endif /* ACE_WIN32 */

ACE_SVC_FACTORY_DECLARE (HTTP_Server)

ACE_STATIC_SVC_DECLARE_EXPORT (ACE_Svc, HTTP_Server)

#endif /* HTTP_SERVER_H */
