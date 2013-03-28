// -*- C++ -*-
/*
 * @file AsyncAccessManager.h
 *
 * $Id$
 *
 * @author Phil Mesnier <mesnier_p@ociweb.com>
 */

#ifndef IMR_ASYNCACCESSMANAGER_H_
#define IMR_ASYNCACCESSMANAGER_H_

#include "locator_export.h"

#include "ImR_ActivatorS.h" // ImR_Activator_AMIS.h
#include "ace/Vector_T.h"
#include "ace/SString.h"

#include "Forwarder.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "LiveCheck.h"

class ImR_Locator_i;
class ImR_ReplyHandler;
struct Server_Info;

//----------------------------------------------------------------------------
/*
 * @class AsyncAccessManager
 *
 * @brief manages the state of a request for a server across multiple upcalls
 *
 * When a client implicitly or explicitly requests a server reference up to
 * three distinct actions must occur, each of which involves waiting for
 * externally triggered events.
 * 1) if not running, request activation of the server
 * 2) if activated, wait for server to call in with a ServerObject reference
 * 3) if not recently pinged, ping the ServerObject to confirm it is alive.
 *
 * A single AsyncAccessManager instance is used to track the progress of
 * an individual server activation. If multiple clients request the same
 * server instance, then each can be notified when the server is ready. If
 * a server is configured for per-client activation, each client request
 * will have its own AAM instance.
 */

enum AAM_Status
  {
    AAM_INIT,
    AAM_ACTIVATION_SENT,
    AAM_WAIT_FOR_RUNNING,
    AAM_WAIT_FOR_PING,
    AAM_WAIT_FOR_ALIVE,
    AAM_SERVER_READY,
    AAM_SERVER_DEAD,
    AAM_NOT_MANUAL,
    AAM_NO_ACTIVATOR
  };

class AsyncAccessManager
{
 public:
  AsyncAccessManager (const Server_Info &info,
                      bool manual,
                      ImR_Locator_i &locator);

  ~AsyncAccessManager (void);

  bool has_server (const char *name);

  void add_interest (ImR_ReplyHandler *rh);
  AAM_Status status (void) const;

  void activator_replied (bool success);
  void server_is_running (const char *partial_ior);
  void ping_replied (bool is_alive);

  void add_ref (void);
  void remove_ref (void);

 private:
  void final_state (void);
  void status (AAM_Status s);
  bool send_start_request (void);

  Server_Info *info_;
  bool manual_start_;
  ImR_Locator_i &locator_;
  PortableServer::POA_var poa_;
  ACE_Vector<ImR_ReplyHandler *> rh_list_;

  AAM_Status status_;

  int refcount_;
  TAO_SYNCH_MUTEX lock_;
};



//----------------------------------------------------------------------------
/*
 * @class ActivatorReceiver
 *
 * @brief callback for handling asynch server startup requests
 *
 */

class ActivatorReceiver :
  public virtual POA_ImplementationRepository::AMI_ActivatorHandler
{
public:
  ActivatorReceiver (AsyncAccessManager *aam, PortableServer::POA_ptr poa);
  virtual ~ActivatorReceiver (void);

  void start_server (void);
  void start_server_excep (Messaging::ExceptionHolder * excep_holder);

  void shutdown (void);
  void shutdown_excep (Messaging::ExceptionHolder * excep_holder);

private:
  AsyncAccessManager *aam_;
  PortableServer::POA_var poa_;
};


//----------------------------------------------------------------------------
/*
 */

class AsyncLiveListener : public LiveListener
{
 public:
  AsyncLiveListener (const char * server,
                     AsyncAccessManager &aam,
                     LiveCheck &pinger);
  virtual ~AsyncLiveListener (void);
  bool start (void);

  void status_changed (LiveStatus status, bool may_retry);

 private:
  AsyncAccessManager &aam_;
  LiveCheck &pinger_;
  LiveStatus status_;
};





#endif /* IMR_ASYNCACCESSMANGER_H_  */
