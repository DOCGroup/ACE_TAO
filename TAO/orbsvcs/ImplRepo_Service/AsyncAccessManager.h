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

#include "tao/Intrusive_Ref_Count_Handle_T.h"
#include "LiveCheck.h"
#include "UpdateableServerInfo.h"
#include "ImR_LocatorC.h"

class ImR_Locator_i;
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

class Locator_Export AsyncAccessManager
{
 public:
  AsyncAccessManager (UpdateableServerInfo &info,
                      bool manual,
                      ImR_Locator_i &locator);

  ~AsyncAccessManager (void);

  void started_running (void);

  bool has_server (const char *name);
  void remote_state (ImplementationRepository::AAM_Status s);

  void add_interest (ImR_ResponseHandler *rh);
  ImplementationRepository::AAM_Status status (void) const;

  void activator_replied (bool success);
  void server_is_running (const char *partial_ior,
                          ImplementationRepository::ServerObject_ptr ref);
  void server_is_shutting_down (void);
  void notify_child_death (void);
  void ping_replied (LiveStatus server);

  AsyncAccessManager *_add_ref (void);
  void _remove_ref (void);
  static const ACE_TCHAR *status_name (ImplementationRepository::AAM_Status s);
  static bool is_final (ImplementationRepository::AAM_Status s);

 private:
  void final_state (bool active = true);
  void notify_waiters (void);
  void status (ImplementationRepository::AAM_Status s);
  void update_status (ImplementationRepository::AAM_Status s);
  bool send_start_request (void);

  UpdateableServerInfo info_;
  bool manual_start_;
  ImR_Locator_i &locator_;
  PortableServer::POA_var poa_;
  ACE_Vector<ImR_ResponseHandler *> rh_list_;

  ImplementationRepository::AAM_Status status_;

  int refcount_;
  TAO_SYNCH_MUTEX lock_;
};

typedef TAO_Intrusive_Ref_Count_Handle<AsyncAccessManager> AsyncAccessManager_ptr;

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
  ActivatorReceiver (AsyncAccessManager *aam,
                     PortableServer::POA_ptr poa);
  virtual ~ActivatorReceiver (void);

  void start_server (void);
  void start_server_excep (Messaging::ExceptionHolder * excep_holder);

  void shutdown (void);
  void shutdown_excep (Messaging::ExceptionHolder * excep_holder);

  void kill_server (CORBA::Boolean);
  void kill_server_excep (Messaging::ExceptionHolder * excep_holder);

private:
  AsyncAccessManager_ptr aam_;
  PortableServer::POA_var poa_;
};


//----------------------------------------------------------------------------
/*
 */

class AccessLiveListener : public LiveListener
{
 public:
  AccessLiveListener (const char * server,
                     AsyncAccessManager *aam,
                     LiveCheck &pinger,
                     ImplementationRepository::ServerObject_ptr ref);

  AccessLiveListener (const char * server,
                     AsyncAccessManager *aam,
                     LiveCheck &pinger);

  virtual ~AccessLiveListener (void);
  bool start (void);

  bool status_changed (LiveStatus status);

 private:
  AsyncAccessManager_ptr aam_;
  LiveCheck &pinger_;
  LiveStatus status_;
  bool per_client_;
  ImplementationRepository::ServerObject_var srv_ref_;
};





#endif /* IMR_ASYNCACCESSMANGER_H_  */
