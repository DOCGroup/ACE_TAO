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

#include "tao/ImR_Client/ServerObjectS.h" // ServerObject_AMIS.h

#include "ace/Vector_T.h"
#include "ace/SString.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "LiveCheck.h"

class ImR_Locator_i;
class ImR_ReplyHandler;
class UpdateableServerInfo;

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
    AAM_SERVER_READY,
    AAM_SERVER_DEAD
  };

class AsyncAccessManager
{
 public:
  AsyncAccessManager (UpdateableServerInfo &info, ImR_Locator_i &locator);
  ~AsyncAccessManager (void);

  void add_interest (ImR_ReplyHandler *rh);
  AAM_Status status (void) const;

  void activator_replied (void);
  void server_is_running (void);
  void ping_replied (bool is_alive);

  void add_ref (void);
  void remove_ref (void);

 private:
  UpdateableServerInfo &info_;
  ImR_Locator_i &locator_;

  ACE_Vector<ImR_ReplyHandler *> rh_list_;

  AAM_Status status_;

  int refcount_;
  TAO_SYNCH_MUTEX lock_;

};

//----------------------------------------------------------------------------
/*
 * @class ImR_Loc_ReplyHandler
 *
 * @brief specialized reply handler for Locator interface calls which have a
 * void return.
 */

class ImR_Loc_ReplyHandler : public ImR_ReplyHandler
{
public:
  ImR_Loc_ReplyHandler (ImplementationRepository::AMH_LocatorResponseHandler_ptr rh);
  virtual ~ImR_Loc_ReplyHandler (void);

  virtual void send_ior (const char *pior);
  virtual void send_exception (void);

private:
  ImplementationRepository::AMH_LocatorResponseHandler_var rh_;

};


//----------------------------------------------------------------------------
/*
 */

class AsyncLiveListener : public LiveListener
{
 public:
  AsyncLiveListener (AsyncAccessManager &aam, LiveCheck *pinger);
  ~AsyncLiveListener (void);

  void status_changed (LiveStatus status, bool may_retry);

 private:
  AsyncAccessManager &aam_;
  LiveCheck *pinger_;
  LiveStatus status_;
};





#endif /* IMR_ASYNCACCESSMANGER_H_  */
